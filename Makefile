PORT ?= /dev/ttyACM0
BOARD = arduino:renesas_uno:unor4wifi
SKETCH = arduino_pong.ino
CACHE_DIR = $(shell pwd)/.arduino_cache
BIN_DIR = $(shell pwd)/bin
CLI = $(BIN_DIR)/arduino-cli --config-file $(CACHE_DIR)/arduino-cli.yaml

YELLOW := \033[0;33m
GREEN  := \033[0;32m
CLEAR  := \033[0m

prepare:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(CACHE_DIR)
	@if [ ! -f $(BIN_DIR)/arduino-cli ]; then \
		echo -e "📥 $(YELLOW)Downloading arduino-cli...$(CLEAR)"; \
		curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=$(BIN_DIR) sh; \
	fi
	@if [ ! -f $(CACHE_DIR)/arduino-cli.yaml ]; then \
		echo -e "⚙️ $(YELLOW)Initializing local config...$(CLEAR)"; \
		$(CLI) config init --dest-dir $(CACHE_DIR); \
		$(CLI) config set directories.data $(CACHE_DIR)/data; \
		$(CLI) config set directories.downloads $(CACHE_DIR)/downloads; \
		$(CLI) config set directories.user $(CACHE_DIR)/user; \
	fi
	@if ! $(CLI) core list | grep -q "arduino:renesas_uno"; then \
		echo -e "$(YELLOW)📥 Installing board core...$(CLEAR)"; \
		$(CLI) core update-index; \
		$(CLI) core install arduino:renesas_uno; \
	fi

compile:
	@echo -e "$(GREEN)🛠️  Compiling $(SKETCH)...$(CLEAR)"
	@$(CLI) compile -b $(BOARD) $(SKETCH)

prepare_and_compile:
	make prepare --no-print-directory
	make compile --no-print-directory

upload:
	@echo -e "$(GREEN)🚀  Uploading $(SKETCH)...$(CLEAR)"
	@$(CLI) upload -b $(BOARD) -p $(PORT)

upload_verbose:
	@echo -e "$(GREEN)🚀  Uploading (Verbose) $(SKETCH)...$(CLEAR)"
	@$(CLI) upload -b $(BOARD) -p $(PORT) -v

monitor:
	@$(CLI) monitor -p $(PORT)

run:
	make compile --no-print-directory
	make upload --no-print-directory
	make monitor --no-print-directory

run_init:
	make prepare --no-print-directory
	make run --no-print-directory
