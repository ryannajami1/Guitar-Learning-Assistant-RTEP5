#!/bin/bash

# --- Configuration ---
ANGULAR_APP_DIR="$HOME/frontend"  # Path to the Angular application directory
NGINX_CONF_DIR="/etc/nginx"
NGINX_SITES_ENABLED="$NGINX_CONF_DIR/sites-enabled"
NGINX_DEFAULT_CONF="$NGINX_CONF_DIR/nginx.conf"
APP_NAME="guitar-trainer-angular-app"
NGINX_APP_CONF="$NGINX_CONF_DIR/conf.d/${APP_NAME}.conf"
WEB_ROOT="/var/www/${APP_NAME}"
NGINX_USER="www-data" # Default Nginx user

# --- Helper Functions ---
log_info() {
  echo "[INFO] $1"
}

log_error() {
  echo "[ERROR] $1" >&2
}

check_command() {
  if ! command -v "$1" &> /dev/null; then
    log_error "$1 is not installed. Please install it before running this script."
    exit 1
  fi
}

run_command() {
  log_info "Executing: $1"
  if ! "$1"; then
    log_error "Command '$1' failed."
    exit 1
  fi
}

# --- Script Start ---
log_info "Starting Angular application deployment script..."

# Check for necessary commands
check_command apt
check_command nginx
check_command ng
check_command systemctl
check_command mkdir
check_command mv
check_command sed
check_command cp

# --- Step 1: Install Nginx (if not already installed) ---
if ! dpkg -s nginx >/dev/null 2>&1; then
  log_info "Nginx not found. Installing..."
  run_command "sudo apt update"
  run_command "sudo apt install -y nginx"
else
  log_info "Nginx is already installed."
fi

# --- Step 2: Build Angular App ---
log_info "Navigating to Angular app directory: $ANGULAR_APP_DIR"
cd "$ANGULAR_APP_DIR" || {
  log_error "Could not navigate to Angular app directory: $ANGULAR_APP_DIR"
  exit 1
}

log_info "Building Angular application..."
run_command "ng build --configuration production" # Assuming you want a production build

# --- Step 3: Prepare Web Directory ---
log_info "Creating web root directory: $WEB_ROOT"
run_command "sudo mkdir -p \"$WEB_ROOT/browser\""
run_command "sudo chown -R \"$USER\":\"$NGINX_USER\" \"$WEB_ROOT\"" # Set appropriate permissions

log_info "Moving built files to web root..."
run_command "sudo mv \"$ANGULAR_APP_DIR/dist/${APP_NAME}/browser\"/* \"$WEB_ROOT/browser/\""

# --- Step 4: Configure Nginx ---
log_info "Configuring Nginx..."

# Comment out the default sites-enabled include
if grep -q "include $NGINX_SITES_ENABLED/*;" "$NGINX_DEFAULT_CONF"; then
  log_info "Commenting out 'include $NGINX_SITES_ENABLED/*;' in $NGINX_DEFAULT_CONF"
  run_command "sudo sed -i 's/include $NGINX_SITES_ENABLED\\/*;/\\#include $NGINX_SITES_ENABLED\\/*;/' \"$NGINX_DEFAULT_CONF\""
else
  log_info "'include $NGINX_SITES_ENABLED/*;' already commented out or not found in $NGINX_DEFAULT_CONF"
fi

# Create the new Nginx configuration file
log_info "Creating Nginx configuration file: $NGINX_APP_CONF"
cat <<EOF | sudo tee "$NGINX_APP_CONF"
listen 80;
server_name _;  # Or localhost

location / {  # Serve Angular from the root path
  root $WEB_ROOT/browser;
  index index.html;
  try_files \$uri \$uri/ /index.html;
}
EOF

# --- Step 5: Restart Nginx ---
log_info "Restarting Nginx service..."
run_command "sudo systemctl restart nginx"

# --- Step 6: Output Instructions for User ---
log_info "Angular application deployed successfully!"
log_info "You can access it in your browser at http://<your-raspberry-pi-ip>/"
log_info "To find your Raspberry Pi's IP address, run: hostname -I"

exit 0
