# ==============================================================================
# Makefile Wrapper untuk CMake
# ==============================================================================

BUILD_DIR = build
CMAKE = cmake

.PHONY: all configure build clean run-master run-bot run-relay test help

all: build

# 1. Konfigurasi CMake
configure:
	@mkdir -p $(BUILD_DIR)
	@echo "==> Configuring CMake..."
	@cd $(BUILD_DIR) && $(CMAKE) ..

# 2. Kompilasi Binary (ddos-bot, ddos-master, ddos-relay)
build: configure
	@echo "==> Building binaries..."
	@cmake --build $(BUILD_DIR) -j$$(nproc 2>/dev/null || echo 4)

# 3. Membersihkan artefak build
clean:
	@echo "==> Cleaning build artifacts..."
	@rm -rf $(BUILD_DIR)

# 4. Menjalankan Master Node
run-master: build
	@echo "==> Starting Master Node..."
	@./$(BUILD_DIR)/ddos-master --config config/master.yaml

# 5. Menjalankan Bot Instance
run-bot: build
	@echo "==> Starting Bot Instance..."
	@./$(BUILD_DIR)/ddos-bot --config config/bot.yaml

# 6. Menjalankan Relay Node
run-relay: build
	@echo "==> Starting Relay Node..."
	@./$(BUILD_DIR)/ddos-relay --config config/relay.yaml

# 7. Menjalankan Unit Testing
test: build
	@echo "==> Running tests..."
	@cd $(BUILD_DIR) && ctest --output-on-failure

# 8. Menampilkan Panduan Perintah
help:
	@echo "Available commands:"
	@echo "  make configure   - Menyiapkan direktori build dan konfigurasi CMake"
	@echo "  make build       - Mengompilasi seluruh binary (default)"
	@echo "  make clean       - Menghapus folder build"
	@echo "  make run-master  - Menjalankan binary ddos-master"
	@echo "  make run-bot     - Menjalankan binary ddos-bot"
	@echo "  make run-relay   - Menjalankan binary ddos-relay"
	@echo "  make test        - Menjalankan unit tests via ctest"
	@echo "  make help        - Menampilkan informasi bantuan ini"

