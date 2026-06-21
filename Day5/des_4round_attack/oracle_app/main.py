from __future__ import annotations

import hashlib
import logging
import os
from typing import Final, Dict

from fastapi import FastAPI, Form, HTTPException, Request
from fastapi.responses import HTMLResponse, JSONResponse
from pydantic import BaseModel, Field, validator
from slowapi import Limiter
from slowapi.errors import RateLimitExceeded
from slowapi.util import get_remote_address

# --- Import modules from the same package ---
try:
    from . import custom_DES
    from .config import BASE_DIR, Settings, get_settings
except ImportError:
    # Fallback for running directly (not as a module)
    import sys
    from pathlib import Path

    sys.path.insert(0, str(Path(__file__).parent))
    import custom_DES  # type: ignore
    from config import BASE_DIR, Settings, get_settings  # type: ignore

# --- Logging Setup ---
logs_dir = BASE_DIR / "logs"
logs_dir.mkdir(exist_ok=True)
logger = logging.getLogger("oracle")
logger.setLevel(logging.INFO)
if not logger.handlers:
    handler = logging.FileHandler(logs_dir / "requests.log")
    handler.setFormatter(logging.Formatter("%(asctime)s %(levelname)s: %(message)s"))
    logger.addHandler(handler)

# --- Rate limiting ---
# One shared key for everyone; users are distinguished by client IP.
# 1000 queries per day per IP (rolling window, in-memory). The encryption
# form POST and the JSON API share the same per-IP budget via a common scope.
# Override with RATE_LIMIT env var (e.g. "1000/day", "2000/day") if needed,
# for example when many students share one campus NAT IP.
RATE_LIMIT: Final[str] = os.environ.get("RATE_LIMIT", "1000/day")
limiter = Limiter(key_func=get_remote_address, default_limits=[])
# Shared budget: the form POST and the JSON API draw from the same per-IP
# counter so a user gets 1000 encryptions per day total, not per endpoint.
oracle_limit = limiter.shared_limit(RATE_LIMIT, scope="oracle")

# --- FastAPI App ---
app = FastAPI(
    title="4-Round DES Oracle",
    version="1.0.0",
    docs_url=None,
    redoc_url=None,
)
app.state.limiter = limiter

PLAINTEXT_LEN: Final[int] = 16
settings: Settings | None = None


# --- 429 handlers (HTML for the form, JSON for the API) ---
@app.exception_handler(RateLimitExceeded)
async def rate_limit_handler(request: Request, exc: RateLimitExceeded):
    detail = f"Query limit exceeded. The oracle allows {RATE_LIMIT} per user."
    if request.url.path.startswith("/api/"):
        return JSONResponse(status_code=429, content={"detail": detail})
    return render_form(
        message=f'<div class="message error">{detail} Try again later.</div>',
        status_code=429,
    )


# --- Pydantic Model ---
class EncryptRequest(BaseModel):
    plaintext: str = Field(..., min_length=PLAINTEXT_LEN, max_length=PLAINTEXT_LEN)

    @validator("plaintext")
    def plaintext_must_be_hex(cls, v):
        v = v.strip().upper()
        if len(v) != PLAINTEXT_LEN:
            raise ValueError(f"Plaintext must be exactly {PLAINTEXT_LEN} hex characters")
        if not all(c in "0123456789ABCDEF" for c in v):
            raise ValueError("Plaintext must contain only hexadecimal characters (0-9, A-F)")
        return v


# --- HTML Form Rendering ---
def render_form(message: str = "", plaintext: str = "", status_code: int = 200) -> HTMLResponse:
    html = f"""
    <!DOCTYPE html>
    <html lang="en">
    <head>
        <meta charset="UTF-8" />
        <title>4-Round DES Oracle</title>
        <style>
            body {{ font-family: Arial, sans-serif; max-width: 560px; margin: 4rem auto; }}
            form {{ display: flex; flex-direction: column; gap: 0.75rem; }}
            label {{ font-weight: bold; }}
            input[type="text"] {{ padding: 0.6rem; font-family: monospace; font-size: 1rem; }}
            button {{ padding: 0.65rem; font-weight: bold; cursor: pointer; }}
            .message {{ margin-top: 1rem; padding: 0.8rem; border-left: 4px solid #ccc; background: #f5f5f5; }}
            .error {{ border-color: #c62828; background: #ffebee; }}
            .cipher {{ border-color: #2e7d32; background: #e8f5e9; }}
            code {{ background: #eee; padding: 0.1rem 0.3rem; }}
        </style>
    </head>
    <body>
        <h1>4-Round DES Encryption Oracle</h1>
        <p>Enter a 64-bit plaintext (16 hexadecimal characters) to encrypt under the
        oracle's secret key. The same key is used for all users.</p>

        {message}

        <form method="post" action="/" novalidate style="margin-top: 1rem;">
            <label for="plaintext">Plaintext (16 hexadecimal characters)</label>
            <input type="text" id="plaintext" name="plaintext" value="{plaintext}"
                   maxlength="{PLAINTEXT_LEN}" required />
            <button type="submit">Encrypt</button>
        </form>

        <hr style="margin-top: 2rem; margin-bottom: 1rem;">
        <p style="font-size: 0.9rem; color: #555;">
            For automated analysis, POST JSON to <code>/api/encrypt</code>:
            <code>{{"plaintext": "0123456789ABCDEF"}}</code>.
            Each user is limited to <strong>{RATE_LIMIT}</strong>.
        </p>
    </body>
    </html>
    """
    return HTMLResponse(html, status_code=status_code)


# --- Startup ---
@app.on_event("startup")
def startup() -> None:
    global settings
    try:
        settings = get_settings()
        logger.info(f"Oracle started with key ending in ...{settings.oracle_key[-4:]}")
    except RuntimeError as e:
        logger.error(f"FATAL: Configuration error on startup: {e}")
        settings = None


# --- Web Form Routes ---
@app.get("/", response_class=HTMLResponse, include_in_schema=False)
def show_form() -> HTMLResponse:
    return render_form()


@app.post("/", include_in_schema=False)
@oracle_limit
def handle_form(request: Request, plaintext: str = Form(None)):
    if settings is None:
        return render_form(
            message='<div class="message error">Oracle is not configured (missing key).</div>',
            status_code=503,
        )

    pt_clean = (plaintext or "").strip().upper()
    error_message = None
    if not pt_clean:
        error_message = '<div class="message error">Plaintext is required.</div>'
    elif len(pt_clean) != PLAINTEXT_LEN:
        error_message = f'<div class="message error">Plaintext must be exactly {PLAINTEXT_LEN} hexadecimal characters.</div>'
    elif any(ch not in "0123456789ABCDEF" for ch in pt_clean):
        error_message = '<div class="message error">Plaintext contains non-hex characters (allowed: 0-9, A-F).</div>'

    if error_message:
        return render_form(message=error_message, plaintext=pt_clean)

    try:
        ciphertext = custom_DES.des_encrypt_hex(pt_clean, settings.oracle_key)
        pt_hash = hashlib.sha256(bytes.fromhex(pt_clean)).hexdigest()[:10]
        logger.info(f"FORM Request - IP: {get_remote_address(request)}, PT Hash: {pt_hash}, CT: {ciphertext}")
        success_message = f'<div class="message cipher"><strong>Ciphertext:</strong> {ciphertext}</div>'
        return render_form(message=success_message, plaintext="")
    except Exception:
        logger.exception(f"FORM Encryption failed for input {pt_clean}")
        return render_form(
            message='<div class="message error">Encryption failed due to an internal error.</div>',
            plaintext=pt_clean,
        )


# --- JSON API Endpoint ---
@app.post("/api/encrypt", response_model=Dict[str, str])
@oracle_limit
async def api_encrypt(request: Request, request_data: EncryptRequest) -> Dict[str, str]:
    if settings is None:
        logger.error("API request received but oracle settings not loaded.")
        raise HTTPException(status_code=503, detail="Oracle service unavailable due to configuration error.")

    plaintext_hex = request_data.plaintext  # Already validated by Pydantic
    try:
        ciphertext = custom_DES.des_encrypt_hex(plaintext_hex, settings.oracle_key)
        pt_hash = hashlib.sha256(bytes.fromhex(plaintext_hex)).hexdigest()[:10]
        logger.info(f"API Request - IP: {get_remote_address(request)}, PT Hash: {pt_hash}, CT: {ciphertext}")
        return {"ciphertext": ciphertext}
    except ValueError as ve:
        logger.warning(f"API Bad Input (post-validation?): {plaintext_hex} - Error: {ve}")
        raise HTTPException(status_code=400, detail=f"Invalid input format: {ve}")
    except Exception:
        logger.exception(f"API Encryption failed for input {plaintext_hex}")
        raise HTTPException(status_code=500, detail="Internal server error during encryption.")
