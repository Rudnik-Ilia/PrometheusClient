# PrometheusClient

A lightweight C++17 library for instrumenting applications with Prometheus-compatible metrics.

> **Note:** This library is developed and tested on **Linux only**.

## Features

- **Counter** - Monotonically increasing metric
- **Gauge** - Metric that can increase and decrease, with built-in time tracking
- **Histogram** - Observations distributed into configurable buckets
- Thread-safe atomic operations
- Prometheus text exposition format output

## Requirements

- **Operating System:** Linux (Ubuntu 20.04+, Debian 11+, CentOS 8+, Fedora 34+)
- **Compiler:** GCC 9+ or Clang 10+ with C++17 support
- **Build System:** CMake 3.14+ or Make
- **Internet:** Required for downloading Google Test on first build (CMake only)

## Installing Dependencies

### Ubuntu / Debian

```bash
# Update package list
sudo apt update

# Install build essentials (GCC, G++, make)
sudo apt install -y build-essential

# Install CMake
sudo apt install -y cmake

# Install Git (needed for FetchContent to download Google Test)
sudo apt install -y git

# Verify installations
g++ --version      # Should show GCC 9+ 
cmake --version    # Should show 3.14+
```

### Fedora / RHEL / CentOS

```bash
# Install development tools
sudo dnf groupinstall -y "Development Tools"

# Install CMake
sudo dnf install -y cmake

# Install Git
sudo dnf install -y git

# Verify installations
g++ --version
cmake --version
```

### Arch Linux

```bash
sudo pacman -S base-devel cmake git
```

---

## Building with CMake (Recommended)

### Step 1: Clone or Download the Repository

```bash
# If using git
git clone <repository-url>
cd PrometheusClient

# Or if you have the source code
cd /path/to/PrometheusClient
```

### Step 2: Create Build Directory

```bash
mkdir -p build
cd build
```

### Step 3: Configure with CMake

```bash
# Basic configuration
cmake ..

# Or with specific options
cmake -DCMAKE_BUILD_TYPE=Release ..
```

#### CMake Configuration Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | `Debug` | Build type: `Debug`, `Release`, `RelWithDebInfo` |
| `BUILD_TESTS` | `ON` | Build unit tests with Google Test |

Examples:

```bash
# Release build without tests
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_TESTS=OFF ..

# Debug build with tests (default)
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_TESTS=ON ..
```

### Step 4: Compile

```bash
# Compile using all available CPU cores
cmake --build . -- -j$(nproc)

# Or using make directly
make -j$(nproc)
```

### Step 5: Verify Build

After successful compilation, you should see:

```
build/
├── libprom.so           # Shared library
├── libprom_static.a     # Static library
└── prom_tests           # Test executable (if BUILD_TESTS=ON)
```

---

## Building with Make (Alternative)

If you prefer not to use CMake, use the provided Makefile:

```bash
cd PrometheusClient

# Build shared library (.so)
make

# Build static library (.a)
make static

# Build both
make all

# Clean build artifacts
make clean

# Install to system (requires sudo)
sudo make install

# Uninstall
sudo make uninstall
```

---

## Building Manually with g++ (No Build System)

### Build Shared Library (.so)

```bash
cd PrometheusClient

# Step 1: Compile source files to object files with Position Independent Code (-fPIC)
g++ -std=c++17 -fPIC -c src/holder.cpp -o holder.o -Iinclude
g++ -std=c++17 -fPIC -c src/anxilary_functions.cpp -o anxilary_functions.o -Iinclude

# Step 2: Link object files into shared library
g++ -shared -o libprom.so holder.o anxilary_functions.o

# Step 3: Clean up object files (optional)
rm -f *.o

# Verify the library was created
ls -la libprom.so
file libprom.so
```

### Build Static Library (.a)

```bash
cd PrometheusClient

# Step 1: Compile source files to object files
g++ -std=c++17 -c src/holder.cpp -o holder.o -Iinclude
g++ -std=c++17 -c src/anxilary_functions.cpp -o anxilary_functions.o -Iinclude

# Step 2: Create static archive
ar rcs libprom.a holder.o anxilary_functions.o

# Step 3: Clean up object files (optional)
rm -f *.o

# Verify the library was created
ls -la libprom.a
ar -t libprom.a
```

### One-liner Commands

```bash
# Shared library (one command)
g++ -std=c++17 -shared -fPIC -o libprom.so src/*.cpp -Iinclude

# Static library (two commands)
g++ -std=c++17 -c src/*.cpp -Iinclude && ar rcs libprom.a *.o && rm -f *.o
```

---

## Installing the Shared Library System-Wide

### Option 1: Install to /usr/local (Recommended)

```bash
# Copy library
sudo cp libprom.so /usr/local/lib/

# Copy headers
sudo mkdir -p /usr/local/include/prom
sudo cp include/*.hpp /usr/local/include/prom/

# Update library cache
sudo ldconfig

# Verify installation
ldconfig -p | grep prom
```

### Option 2: Install to /usr/lib

```bash
# Copy library
sudo cp libprom.so /usr/lib/

# Copy headers
sudo mkdir -p /usr/include/prom
sudo cp include/*.hpp /usr/include/prom/

# Update library cache
sudo ldconfig
```

### Option 3: Install to Custom Directory

```bash
# Create installation directory
sudo mkdir -p /opt/prom/{lib,include}

# Copy files
sudo cp libprom.so /opt/prom/lib/
sudo cp include/*.hpp /opt/prom/include/

# Add to library path (add to ~/.bashrc for persistence)
export LD_LIBRARY_PATH=/opt/prom/lib:$LD_LIBRARY_PATH
```

---

## Using the Library in Your Project

### Method 1: Using Installed Library

After installing to `/usr/local`:

```bash
# Compile your application
g++ -std=c++17 -o myapp myapp.cpp -I/usr/local/include/prom -lprom

# Run
./myapp
```

### Method 2: Using Library from Build Directory

```bash
# Set library path for this session
export LD_LIBRARY_PATH=/path/to/PrometheusClient:$LD_LIBRARY_PATH

# Compile
g++ -std=c++17 -o myapp myapp.cpp \
    -I/path/to/PrometheusClient/include \
    -L/path/to/PrometheusClient \
    -lprom

# Run
./myapp
```

### Method 3: Using Static Library (No Runtime Dependencies)

```bash
# Compile with static library (library embedded in executable)
g++ -std=c++17 -o myapp myapp.cpp \
    -I/path/to/PrometheusClient/include \
    /path/to/PrometheusClient/libprom.a

# Run (no LD_LIBRARY_PATH needed)
./myapp
```

### Method 4: Using rpath (Embed Library Path in Executable)

```bash
# Compile with rpath - executable remembers where to find library
g++ -std=c++17 -o myapp myapp.cpp \
    -I/path/to/PrometheusClient/include \
    -L/path/to/PrometheusClient \
    -lprom \
    -Wl,-rpath,/path/to/PrometheusClient

# Run (no LD_LIBRARY_PATH needed)
./myapp
```

### Method 5: Using $ORIGIN rpath (Portable)

```bash
# Library must be in same directory as executable or ./lib/
g++ -std=c++17 -o myapp myapp.cpp \
    -I/path/to/PrometheusClient/include \
    -L/path/to/PrometheusClient \
    -lprom \
    -Wl,-rpath,'$ORIGIN'

# Copy library next to executable
cp /path/to/PrometheusClient/libprom.so ./

# Run
./myapp
```

---

## Verifying Library Linkage

```bash
# Check what libraries your executable needs
ldd myapp

# Should show something like:
#   libprom.so => /usr/local/lib/libprom.so (0x00007f...)
#   libstdc++.so.6 => /lib/x86_64-linux-gnu/libstdc++.so.6 (0x00007f...)
#   ...

# Check library symbols
nm -D libprom.so

# Check if library is found
ldconfig -p | grep prom
```

---

## Complete Example: Build and Use

### Step 1: Build the Library

```bash
cd /home/user/PrometheusClient
g++ -std=c++17 -shared -fPIC -o libprom.so src/*.cpp -Iinclude
```

### Step 2: Create Your Application

Create `myapp.cpp`:

```cpp
#include "prometheus.hpp"
#include <iostream>

int main()
{
    // Create a counter
    auto requests = prometheus<Counter, int64_t>::GetMetric(
        "http_requests_total",
        "Total HTTP requests",
        {"method", "status"},
        {"GET", "200"});

    // Increment counter
    requests->Inc();
    requests->Inc(99);

    // Create a gauge
    auto temperature = prometheus<Gauge, double>::GetMetric(
        "room_temperature",
        "Current room temperature",
        {"room"},
        {"living_room"});

    temperature->Set(22.5);

    // Create histogram
    auto latency = prometheus<Histogram, double>::GetMetric(
        "request_latency",
        "Request latency in seconds",
        {"endpoint"},
        {"/api/users"});

    latency->Buckets({0.01, 0.05, 0.1, 0.5, 1.0});
    latency->Observe(0.042);
    latency->Observe(0.089);

    // Collect and print all metrics
    auto holder = Singleton<Holder>::GetInstance();
    std::cout << holder->CollectData() << std::endl;

    return 0;
}
```

### Step 3: Compile Your Application

```bash
# Using shared library with rpath
g++ -std=c++17 -o myapp myapp.cpp \
    -I/home/user/PrometheusClient/include \
    -L/home/user/PrometheusClient \
    -lprom \
    -Wl,-rpath,/home/user/PrometheusClient
```

### Step 4: Run

```bash
./myapp
```

Expected output:

```
# HELP http_requests_total Total HTTP requests
# TYPE http_requests_total counter
http_requests_total{method="GET",status="200"} 100
# HELP room_temperature Current room temperature
# TYPE room_temperature gauge
room_temperature{room="living_room"} 22.500000
# HELP request_latency Request latency in seconds
# TYPE request_latency histogram
request_latency_bucket{endpoint="/api/users",le="0.010000"} 0
request_latency_bucket{endpoint="/api/users",le="0.050000"} 1
request_latency_bucket{endpoint="/api/users",le="0.100000"} 2
request_latency_bucket{endpoint="/api/users",le="0.500000"} 2
request_latency_bucket{endpoint="/api/users",le="1.000000"} 2
request_latency_bucket{endpoint="/api/users",le="+Inf"} 2
request_latency_sum 0.131000
request_latency_count 2
```

---

## Running Tests

Tests use Google Test framework (automatically downloaded during cmake configuration).

```bash
# From the build directory
cd build

# Run all tests with CTest
ctest --output-on-failure

# Run with verbose output
ctest -V

# Run specific test
ctest -R CounterTest

# Or run test executable directly
./prom_tests

# Run specific test suite
./prom_tests --gtest_filter="CounterTest.*"

# Run specific test case
./prom_tests --gtest_filter="CounterTest.ThreadSafety"
```

### Test Coverage

| Test Suite | Test Cases |
|------------|------------|
| `CounterTest` | DefaultValueIsZero, IncrementByOne, IncrementByValue, MultipleIncrements, DoubleCounter, Reset, Labels, ThreadSafety, GetValueAsString |
| `GaugeTest` | DefaultValueIsZero, Increment, Decrement, Set, IncAndDec, DoubleGauge, NegativeValues, Labels, Timer, GetValueAsString |
| `HistogramTest` | DefaultBucketIsInfinity, ManualBuckets, LinearBuckets, ExponentialBuckets, ObserveUpdatesSum, Labels, Reset, Int64Histogram |
| `HolderTest` | RegisterCounter, RegisterGauge, CollectDataContainsHelp, CollectDataContainsType, GroupWithMultipleMetrics, HistogramSerialization |

---

## Troubleshooting

### Library not found at runtime

```
./myapp: error while loading shared libraries: libprom.so: cannot open shared object file
```

**Solutions:**

```bash
# Option 1: Set LD_LIBRARY_PATH
export LD_LIBRARY_PATH=/path/to/lib:$LD_LIBRARY_PATH

# Option 2: Add to system library path
echo "/path/to/lib" | sudo tee /etc/ld.so.conf.d/prom.conf
sudo ldconfig

# Option 3: Recompile with rpath
g++ -std=c++17 -o myapp myapp.cpp -I... -L... -lprom -Wl,-rpath,/path/to/lib
```

### Undefined reference errors

```
undefined reference to `Holder::CollectData()'
```

**Solution:** Make sure you're linking the library:

```bash
# Wrong (missing -lprom)
g++ -std=c++17 -o myapp myapp.cpp -Iinclude

# Correct
g++ -std=c++17 -o myapp myapp.cpp -Iinclude -L. -lprom
```

### CMake version too old

```
CMake Error at CMakeLists.txt:1:
  CMake 3.14 or higher is required.  You are running version 3.10
```

**Solution:** Install newer CMake or use Make/manual compilation:

```bash
# Use Makefile instead
make

# Or compile manually
g++ -std=c++17 -shared -fPIC -o libprom.so src/*.cpp -Iinclude
```

### Compiler does not support C++17

```
error: unrecognized command line option '-std=c++17'
```

**Solution:** Update GCC:

```bash
# Ubuntu/Debian
sudo apt install -y g++-9
g++-9 -std=c++17 -shared -fPIC -o libprom.so src/*.cpp -Iinclude
```

---

## Project Structure

```
PrometheusClient/
├── include/
│   ├── prometheus.hpp       # Main API entry point
│   ├── counter.hpp          # Counter metric
│   ├── gauge.hpp            # Gauge metric  
│   ├── histogram.hpp        # Histogram metric
│   ├── group.hpp            # Metric grouping
│   ├── holder.hpp           # Global metric registry
│   ├── singleton.hpp        # Singleton pattern
│   ├── IBaseMetric.hpp      # Metric interface
│   ├── IBaseGroup.hpp       # Group interface
│   ├── init.hpp             # Types and constants
│   ├── anxilary_functions.hpp
│   └── outScope.hpp
├── src/
│   ├── holder.cpp
│   └── anxilary_functions.cpp
├── tests/
│   ├── test_counter.cpp
│   ├── test_gauge.cpp
│   ├── test_histogram.cpp
│   └── test_holder.cpp
├── CMakeLists.txt
├── Makefile
├── README.md
└── .gitignore
```

## License

[Add your license here]
