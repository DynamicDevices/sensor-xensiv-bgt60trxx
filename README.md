# XENSIV™ BGT60TRxx Radar Sensor

This library provides functions for interfacing with the XENSIV™ BGT60TRxx 60 GHz FMCW Radar Sensors. This library can be set up to use the ModusToolbox™ HAL interface, or using user-provided communication functions. For more information about the XENSIV™ Radar Sensor please visit the [Infineon web page on radar sensors](https://www.infineon.com/cms/en/product/sensor/radar-sensors/radar-sensors-for-iot/60ghz-radar/).

## 🚀 Cross-Platform Implementation

This is a **cross-platform implementation** of the XENSIV™ BGT60TRxx library that supports:

- **Linux Systems**: Native Linux implementation with SPI and GPIO support
- **Yocto/OpenEmbedded**: Complete BitBake recipes for embedded Linux deployment  
- **ModusToolbox**: Maintained compatibility with existing MTB projects
- **Multiple Build Systems**: CMake and Autotools support

## 📋 Features

### Core Functionality
- **Multi-device Support**: BGT60TR13C, BGT60UTR13D, BGT60UTR11
- **SPI Communication**: Configurable SPI interface with burst mode support
- **GPIO Control**: Reset and chip-select pin management
- **FIFO Processing**: Advanced FIFO data handling with interrupt support
- **Error Handling**: Comprehensive error detection and recovery

### Platform Support
- **Linux**: Native implementation using spidev and GPIO character devices
- **Cross-compilation**: Support for ARM, ARM64, x86_64 architectures
- **Yocto Integration**: Ready-to-use BitBake recipes
- **Package Management**: pkg-config and CMake integration

## 🛠️ Quick Start

### Prerequisites

**For Linux development:**
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake pkg-config

# CentOS/RHEL/Fedora  
sudo yum install gcc cmake pkgconfig-devel
# or
sudo dnf install gcc cmake pkgconfig-devel
```

**For Yocto/OpenEmbedded:**
- Yocto Project setup (Kirkstone or later recommended)
- BitBake environment configured

### Building the Library

#### Option 1: Using the Build Script (Recommended)
```bash
# Clone the repository
git clone https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git
cd sensor-xensiv-bgt60trxx

# Build with CMake (default)
./build.sh --clean

# Or build with Autotools
./build.sh -s autotools --clean

# Install the library
sudo ./build.sh --install
```

#### Option 2: Manual CMake Build
```bash
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_EXAMPLES=ON ..
make -j$(nproc)
sudo make install
```

#### Option 3: Manual Autotools Build
```bash
autoreconf -fiv
./configure --enable-examples --prefix=/usr/local
make -j$(nproc)
sudo make install
```

### Build Options

The build script supports extensive configuration:

```bash
# Show all options
./build.sh --help

# Common configurations
./build.sh --clean --examples --debug          # Debug build with examples
./build.sh -s autotools --prefix=/opt/xensiv   # Autotools with custom prefix
./build.sh --no-examples --install             # Release build without examples
```

## 📖 Usage Examples

### Basic Sensor Initialization

```c
#include "xensiv_bgt60trxx.h"
#include "xensiv_bgt60trxx_linux.h"

int main() {
    // Initialize Linux platform interface
    xensiv_bgt60trxx_linux_t linux_interface = {
        .spi_device = "/dev/spidev0.0",
        .gpio_chip = "/dev/gpiochip0", 
        .reset_gpio = 18,
        .cs_gpio = 24
    };
    
    xensiv_bgt60trxx_linux_obj_t sensor;
    
    // Initialize sensor
    int result = xensiv_bgt60trxx_linux_init(&linux_interface, &sensor);
    if (result != XENSIV_BGT60TRXX_STATUS_OK) {
        printf("Sensor initialization failed: %d\n", result);
        return -1;
    }
    
    // Configure and use sensor...
    
    return 0;
}
```

### Running Examples

The library includes comprehensive examples:

```bash
# Basic sensor operation
./build/examples/basic_example -s /dev/spidev0.0 -g /dev/gpiochip0 -r 18 -c 24

# FIFO data processing  
./build/examples/fifo_example -s /dev/spidev1.0 -i 25

# Advanced configuration
./build/examples/config_example --help
```

## 🔧 Hardware Setup

### SPI Configuration
- **Bus Speed**: Up to 10 MHz supported
- **Mode**: SPI Mode 0 (CPOL=0, CPHA=0)
- **Bit Order**: MSB first
- **Word Size**: 8 bits

### GPIO Requirements
- **Reset Pin**: Active-low reset control
- **Chip Select**: SPI chip select (optional if using hardware CS)
- **Interrupt Pin**: For FIFO and status interrupts (optional)

### Typical Connections
```
Raspberry Pi 4 Example:
- SPI0 MOSI (GPIO 10) → BGT60TRxx SPI_DI
- SPI0 MISO (GPIO 9)  → BGT60TRxx SPI_DO  
- SPI0 SCLK (GPIO 11) → BGT60TRxx SPI_CLK
- GPIO 18             → BGT60TRxx RESET (active low)
- GPIO 24             → BGT60TRxx SPI_CS (optional)
- GPIO 25             → BGT60TRxx IRQ (optional)
```

## 🐧 Yocto Integration

### Adding to Your Yocto Build

1. **Add the layer to your build:**
```bash
# Copy recipes to your meta layer or create a new layer
mkdir -p meta-xensiv/recipes-support/xensiv-bgt60trxx
cp yocto/* meta-xensiv/recipes-support/xensiv-bgt60trxx/

# Add to bblayers.conf
bitbake-layers add-layer meta-xensiv
```

2. **Include in your image:**
```bash
# Add to local.conf or image recipe
IMAGE_INSTALL += "xensiv-bgt60trxx xensiv-bgt60trxx-examples"
```

3. **Build the package:**
```bash
# Build development version
bitbake xensiv-bgt60trxx-git

# Build release version  
bitbake xensiv-bgt60trxx
```

### Yocto Configuration Options

The `yocto/xensiv-bgt60trxx.conf` file provides default settings:
```bash
# SPI device configuration
XENSIV_BGT60TRXX_SPI_DEVICE ?= "/dev/spidev0.0"
XENSIV_BGT60TRXX_GPIO_CHIP ?= "/dev/gpiochip0"
XENSIV_BGT60TRXX_RESET_GPIO ?= "18"
XENSIV_BGT60TRXX_CS_GPIO ?= "24"
```

## 🔗 Integration with Other Projects

### CMake Integration
```cmake
find_package(xensiv_bgt60trxx REQUIRED)
target_link_libraries(your_app xensiv_bgt60trxx::xensiv_bgt60trxx)
```

### pkg-config Integration
```bash
# Compile flags
gcc $(pkg-config --cflags xensiv_bgt60trxx) -c your_app.c

# Link flags  
gcc your_app.o $(pkg-config --libs xensiv_bgt60trxx) -o your_app
```

### Autotools Integration
```makefile
# In Makefile.am
your_app_CFLAGS = $(XENSIV_BGT60TRXX_CFLAGS)
your_app_LDADD = $(XENSIV_BGT60TRXX_LIBS)
```

## 🧪 Testing

### Integration Test
```bash
# Build and run integration test
gcc -I. -o test_integration test_integration.c libxensiv_bgt60trxx.a
./test_integration
```

### Hardware Validation
```bash
# Test with actual hardware
./build/examples/basic_example -s /dev/spidev0.0 -g /dev/gpiochip0 -r 18 -c 24

# Expected output:
# XENSIV BGT60TRxx Basic Example
# Initializing sensor...
# Sensor initialized successfully
# Device ID: 0x...
```

## 📚 API Reference

### Core Functions
- `xensiv_bgt60trxx_linux_init()` - Initialize sensor with Linux platform
- `xensiv_bgt60trxx_linux_init_sensor()` - Initialize sensor object
- `xensiv_bgt60trxx_linux_deinit()` - Cleanup and deinitialize

### Configuration Functions  
- `xensiv_bgt60trxx_set_config()` - Set sensor configuration
- `xensiv_bgt60trxx_get_config()` - Get current configuration
- `xensiv_bgt60trxx_reset()` - Reset sensor

### Data Acquisition
- `xensiv_bgt60trxx_get_fifo_data()` - Read FIFO data
- `xensiv_bgt60trxx_get_register()` - Read register value
- `xensiv_bgt60trxx_set_register()` - Write register value

## 🐛 Troubleshooting

### Common Issues

**Permission Denied on SPI/GPIO:**
```bash
# Add user to required groups
sudo usermod -a -G spi,gpio $USER
# Or run with sudo for testing
sudo ./examples/basic_example
```

**SPI Device Not Found:**
```bash
# Check available SPI devices
ls -la /dev/spi*
# Enable SPI in device tree or kernel config
```

**GPIO Access Issues:**
```bash
# Check GPIO chip availability
ls -la /dev/gpiochip*
# Verify GPIO numbers with:
gpioinfo
```

**Build Errors:**
```bash
# Install missing dependencies
sudo apt-get install build-essential cmake pkg-config

# Clean and rebuild
./build.sh --clean
```

### Debug Mode
```bash
# Build with debug symbols
./build.sh --debug --clean

# Run with GDB
gdb ./build/examples/basic_example
```

## 📄 Documentation

- **API Reference**: See header files for detailed function documentation
- **Examples**: Check `examples/` directory for usage patterns
- **Build System**: Refer to `CMakeLists.txt` and `configure.ac` for build options
- **Yocto Integration**: See `yocto/` directory for BitBake recipes

## 🤝 Contributing

1. Fork the repository: https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Commit your changes: `git commit -m 'Add amazing feature'`
4. Push to the branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

### Development Guidelines
- Follow existing code style and conventions
- Add tests for new functionality
- Update documentation for API changes
- Ensure cross-platform compatibility

## 📜 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🙏 Acknowledgments

- **Infineon Technologies**: Original XENSIV™ BGT60TRxx sensor and ModusToolbox library
- **Dynamic Devices**: Cross-platform implementation and Linux support
- **Community Contributors**: Bug reports, feature requests, and improvements

## 📞 Support

- **Issues**: Report bugs and request features via [GitHub Issues](https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx/issues)
- **Discussions**: Join the community discussions
- **Commercial Support**: Contact Dynamic Devices for commercial support and consulting

---

**Note**: This library provides a cross-platform abstraction for the XENSIV™ BGT60TRxx sensors. For the original ModusToolbox-specific implementation, please refer to the official Infineon documentation and libraries.