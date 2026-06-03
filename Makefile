# PrometheusClient Makefile
# Tested on Linux only

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2
CXXFLAGS_DEBUG = -std=c++17 -Wall -Wextra -g -O0

# Directories
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Source files
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Library names
LIB_SHARED = libprom.so
LIB_STATIC = libprom.a

# Installation paths
PREFIX = /usr/local
INSTALL_LIB = $(PREFIX)/lib
INSTALL_INC = $(PREFIX)/include/prom

# Default target
.PHONY: all
all: shared static

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile object files (Position Independent Code for shared library)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -fPIC -c $< -o $@ -I$(INC_DIR)

# Build shared library
.PHONY: shared
shared: $(BUILD_DIR)/$(LIB_SHARED)

$(BUILD_DIR)/$(LIB_SHARED): $(OBJECTS)
	$(CXX) -shared -o $@ $^
	@echo ""
	@echo "==> Built shared library: $(BUILD_DIR)/$(LIB_SHARED)"
	@echo ""

# Build static library
.PHONY: static
static: $(BUILD_DIR)/$(LIB_STATIC)

$(BUILD_DIR)/$(LIB_STATIC): $(OBJECTS)
	ar rcs $@ $^
	@echo ""
	@echo "==> Built static library: $(BUILD_DIR)/$(LIB_STATIC)"
	@echo ""

# Debug build
.PHONY: debug
debug: CXXFLAGS = $(CXXFLAGS_DEBUG)
debug: clean shared static
	@echo "==> Debug build complete"

# Install to system
.PHONY: install
install: shared static
	@echo "Installing to $(PREFIX)..."
	install -d $(INSTALL_LIB)
	install -d $(INSTALL_INC)
	install -m 644 $(BUILD_DIR)/$(LIB_SHARED) $(INSTALL_LIB)/
	install -m 644 $(BUILD_DIR)/$(LIB_STATIC) $(INSTALL_LIB)/
	install -m 644 $(INC_DIR)/*.hpp $(INSTALL_INC)/
	ldconfig
	@echo ""
	@echo "==> Installation complete!"
	@echo "    Libraries: $(INSTALL_LIB)"
	@echo "    Headers:   $(INSTALL_INC)"
	@echo ""

# Uninstall from system
.PHONY: uninstall
uninstall:
	@echo "Uninstalling from $(PREFIX)..."
	rm -f $(INSTALL_LIB)/$(LIB_SHARED)
	rm -f $(INSTALL_LIB)/$(LIB_STATIC)
	rm -rf $(INSTALL_INC)
	ldconfig
	@echo "==> Uninstall complete"

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)
	@echo "==> Clean complete"

# Show help
.PHONY: help
help:
	@echo "PrometheusClient Makefile"
	@echo ""
	@echo "Usage: make [target]"
	@echo ""
	@echo "Targets:"
	@echo "  all        Build shared and static libraries (default)"
	@echo "  shared     Build shared library (libprom.so)"
	@echo "  static     Build static library (libprom.a)"
	@echo "  debug      Build with debug symbols"
	@echo "  install    Install to system (requires sudo)"
	@echo "  uninstall  Remove from system (requires sudo)"
	@echo "  clean      Remove build artifacts"
	@echo "  help       Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make                    # Build both libraries"
	@echo "  make shared             # Build only shared library"
	@echo "  sudo make install       # Install to /usr/local"
	@echo "  make PREFIX=/opt/prom install  # Install to custom location"
	@echo ""
	@echo "After building, link your project with:"
	@echo "  g++ -std=c++17 -o myapp myapp.cpp -I$(INC_DIR) -L$(BUILD_DIR) -lprom"
	@echo ""
