from __future__ import annotations

import os
from dataclasses import dataclass
from functools import lru_cache
from pathlib import Path

from dotenv import load_dotenv


BASE_DIR = Path(__file__).resolve().parent.parent
ENV_PATH = BASE_DIR / ".env"


def load_environment() -> None:
    if ENV_PATH.exists():
        load_dotenv(ENV_PATH)


@dataclass(frozen=True)
class Settings:
    oracle_key: str


@lru_cache(maxsize=1)
def get_settings() -> Settings:
    load_environment()

    try:
        oracle_key = os.environ["ORACLE_KEY"]
    except KeyError as exc:
        raise RuntimeError(f"Missing required environment variable: {exc.args[0]}") from exc

    if len(oracle_key) != 14 or any(ch not in "0123456789ABCDEFabcdef" for ch in oracle_key):
        raise RuntimeError("ORACLE_KEY must be a 14-character hexadecimal string (56-bit key).")

    return Settings(oracle_key=oracle_key.upper())
