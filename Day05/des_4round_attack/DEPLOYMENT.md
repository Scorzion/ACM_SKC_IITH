# Deploying the 4-Round DES Oracle

A single oracle serves all students. Everyone is encrypted under the **same**
secret key (`ORACLE_KEY`). Users are distinguished by **client IP** and limited
to **1000 queries/day** (configurable).

```
client --> nginx (host port, e.g. 80) --> oracle container (internal :8000)
```

nginx rewrites `X-Forwarded-For` to the real client IP and the oracle container
is not published to the host, so the per-IP limit cannot be spoofed.

## Files

- `Dockerfile` / `oracle_app/` - the FastAPI oracle (real DES S-boxes, 4 rounds)
- `docker-compose.yml` - oracle + nginx stack
- `deploy/nginx.conf` - front proxy config
- `.env` - secrets/config (copy from `.env.example`)

## 1. Provision a server

Any Linux VM with a public IP works (AWS EC2, GCP, Azure, a campus VM, etc.).

- Open the inbound port you intend to expose (default TCP **80**).
- Note the server's IP; you'll hand this to students later as
  `http://<SERVER_IP>/`. Nothing in the app hardcodes the IP, so you can pick
  the host at any time.

## 2. Install Docker + Compose

```bash
curl -fsSL https://get.docker.com | sh
sudo usermod -aG docker "$USER"   # re-login afterwards
docker compose version            # verify the compose plugin is present
```

## 3. Configure

```bash
git clone <this-repo> des-oracle && cd des-oracle
cp .env.example .env
# edit .env: set ORACLE_KEY (14 hex chars), optionally RATE_LIMIT / LISTEN_PORT
```

`.env`:

```
ORACLE_KEY=3A5B7C9D2E4F6A     # 14 hex chars = 56-bit key, SAME for all students
# RATE_LIMIT=1000/day         # raise if many students share one NAT IP
# LISTEN_PORT=80              # host port nginx listens on
```

## 4. Build and run

```bash
docker compose up -d --build
docker compose ps
docker compose logs -f oracle   # should show "Application startup complete"
```

## 5. Smoke test

From the server:

```bash
curl -s -X POST http://localhost/api/encrypt \
  -H 'Content-Type: application/json' \
  -d '{"plaintext":"0123456789ABCDEF"}'
# -> {"ciphertext":"..."}
```

From another machine, replace `localhost` with the server IP. Confirm per-IP
limiting by sending more than `RATE_LIMIT` requests; further requests return
HTTP `429`.

## 6. Give students the URL

- Web form:   `http://<SERVER_IP>/`
- JSON API:   `POST http://<SERVER_IP>/api/encrypt` with body
  `{"plaintext": "0123456789ABCDEF"}`

## Operations

- **Change the key / limit:** edit `.env`, then `docker compose up -d` (the
  oracle restarts; the in-memory query counters reset).
- **Logs:** request logs are written inside the container at
  `/app/logs/requests.log` (and via `docker compose logs oracle`).
- **TLS (optional):** put a TLS terminator (Caddy, or certbot + nginx) in front,
  or add a `443` server block to `deploy/nginx.conf`. Plain HTTP is sufficient
  for an in-class assignment.

## Notes / tradeoffs

- **IP-based identity:** students behind the same campus NAT or VPN share one
  1000/day budget. If that is too tight, raise `RATE_LIMIT` (e.g. `4000/day`)
  or have those students connect from different networks.
- **Daily window, in-memory:** counters are per-IP rolling 24h and reset on
  container restart. This matches a soft "don't hammer the oracle" policy
  rather than a hard lifetime cap.
