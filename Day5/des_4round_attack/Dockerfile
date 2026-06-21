# Use an official Python runtime as a parent image
FROM python:3.11-slim

# Set the working directory inside the container
WORKDIR /app

# Copy the requirements file and install dependencies
COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# --- FIX ---
# Copy the *entire* app package. This now includes
# main.py, config.py, and custom_DES.py
COPY oracle_app/ ./oracle_app

# Make port 8000 available
EXPOSE 8000

# Run the app. --proxy-headers + --forwarded-allow-ips lets the app read the
# real client IP from X-Forwarded-For set by the nginx front proxy (the app
# port is not published to the host, so only the proxy can set this header).
CMD ["uvicorn", "oracle_app.main:app", \
     "--host", "0.0.0.0", "--port", "8000", \
     "--proxy-headers", "--forwarded-allow-ips", "*"]

