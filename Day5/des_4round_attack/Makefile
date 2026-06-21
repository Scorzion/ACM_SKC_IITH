# Makefile for the C++ oracle client (query.cpp).
#
# Quick start:
#   make deps     # install the libcurl development library (needs sudo)
#   make          # build ./query
#   make run      # build and run the demo
#   make clean    # remove the built binary
#
# If libcurl is installed in a non-standard location, pkg-config is used
# automatically to find the right compiler/linker flags.

CXX      ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra

TARGET := query
SRC    := query.cpp

# Prefer pkg-config flags if available; otherwise fall back to -lcurl.
CURL_CFLAGS := $(shell pkg-config --cflags libcurl 2>/dev/null)
CURL_LIBS   := $(shell pkg-config --libs libcurl 2>/dev/null)
ifeq ($(strip $(CURL_LIBS)),)
CURL_LIBS := -lcurl
endif

.PHONY: all run deps clean

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(CURL_CFLAGS) $(SRC) -o $(TARGET) $(CURL_LIBS)

run: $(TARGET)
	./$(TARGET)

# Install the libcurl development headers/library.
# Auto-detects the platform's package manager.
deps:
	@if command -v apt-get >/dev/null 2>&1; then \
		echo ">> Debian/Ubuntu: installing libcurl4-openssl-dev"; \
		sudo apt-get update && sudo apt-get install -y libcurl4-openssl-dev; \
	elif command -v dnf >/dev/null 2>&1; then \
		echo ">> Fedora/RHEL: installing libcurl-devel"; \
		sudo dnf install -y libcurl-devel; \
	elif command -v yum >/dev/null 2>&1; then \
		echo ">> CentOS/RHEL: installing libcurl-devel"; \
		sudo yum install -y libcurl-devel; \
	elif command -v pacman >/dev/null 2>&1; then \
		echo ">> Arch: installing curl"; \
		sudo pacman -S --noconfirm curl; \
	elif command -v zypper >/dev/null 2>&1; then \
		echo ">> openSUSE: installing libcurl-devel"; \
		sudo zypper install -y libcurl-devel; \
	elif command -v brew >/dev/null 2>&1; then \
		echo ">> macOS (Homebrew): installing curl"; \
		brew install curl; \
		echo ">> If linking fails, build with: PKG_CONFIG_PATH=$$(brew --prefix curl)/lib/pkgconfig make"; \
	else \
		echo "No supported package manager found. Install the libcurl dev package manually."; \
		exit 1; \
	fi

clean:
	rm -f $(TARGET)
