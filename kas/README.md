# Kas Integration for XENSIV BGT60TRxx

This directory contains [Kas](https://kas.readthedocs.io/) configuration files for building Yocto/OpenEmbedded images with the XENSIV BGT60TRxx radar sensor library.

## 🚀 Quick Start

### Prerequisites

Install Kas:
```bash
# Using pip (in virtual environment)
pip3 install kas

# Or using pipx (recommended)
pipx install kas

# Or using system package manager
sudo apt install kas  # Ubuntu/Debian

## 📋 Available Configurations

### Development Configurations (Local Use)
These configurations use `url: "."` to reference the current repository:

#### Kirkstone LTS (Yocto 4.0)
- **`xensiv-bgt60trxx-test.yml`** - QEMU x86-64 test image
- **`xensiv-bgt60trxx-rpi4.yml`** - Raspberry Pi 4 image

#### Scarthgap LTS (Yocto 5.0)
- **`xensiv-bgt60trxx-test-scarthgap.yml`** - QEMU x86-64 test image
- **`xensiv-bgt60trxx-rpi4-scarthgap.yml`** - Raspberry Pi 4 image

### CI/CD Configurations (Automated Builds)
These configurations use the GitHub repository URL for CI environments:

#### Kirkstone LTS (Yocto 4.0)
- **`xensiv-bgt60trxx-test-kirkstone-ci.yml`** - QEMU x86-64 test image (CI)
- **`xensiv-bgt60trxx-rpi4-kirkstone-ci.yml`** - Raspberry Pi 4 image (CI)

#### Scarthgap LTS (Yocto 5.0)
- **`xensiv-bgt60trxx-test-scarthgap-ci.yml`** - QEMU x86-64 test image (CI)
- **`xensiv-bgt60trxx-rpi4-scarthgap-ci.yml`** - Raspberry Pi 4 image (CI)

### Base Configurations
- **`base-image.yml`** - Common base configuration for Kirkstone
- **`base-image-scarthgap.yml`** - Common base configuration for Scarthgap
```

### Build an Image

```bash
# Build QEMU test image (Kirkstone LTS)
./kas-build.sh

# Build Raspberry Pi 4 image (Kirkstone LTS)
./kas-build.sh kas/xensiv-bgt60trxx-rpi4.yml

# Build with Docker (containerized)
./kas-build.sh --docker

# Build Scarthgap LTS version
./kas-build.sh kas/xensiv-bgt60trxx-test-scarthgap.yml
```

## 📋 Available Configurations

### Kirkstone LTS (Yocto 4.0)
- **`xensiv-bgt60trxx-test.yml`** - QEMU x86-64 test image
- **`xensiv-bgt60trxx-rpi4.yml`** - Raspberry Pi 4 image

### Scarthgap LTS (Yocto 5.0)
- **`xensiv-bgt60trxx-test-scarthgap.yml`** - QEMU x86-64 test image  
- **`xensiv-bgt60trxx-rpi4-scarthgap.yml`** - Raspberry Pi 4 image

### Base Configurations
- **`base-image.yml`** - Common base configuration for Kirkstone
- **`base-image-scarthgap.yml`** - Common base configuration for Scarthgap

## 🛠️ Build Scripts

### kas-build.sh
Main build script with comprehensive options:

```bash
./kas-build.sh [OPTIONS] [CONFIG_FILE]

Options:
  -d, --docker    Use Docker for containerized builds
  -c, --clean     Clean build (force checkout)
  -s, --sdk       Build SDK after image
  -q, --qemu      Run QEMU after successful build (x86-64 only)
  -v, --verbose   Enable verbose output
  -h, --help      Show help message
```

Examples:
```bash
# Basic build
./kas-build.sh

# Docker build with clean checkout
./kas-build.sh --docker --clean

# Build with SDK and run QEMU
./kas-build.sh --sdk --qemu

# Build Raspberry Pi 4 image
./kas-build.sh kas/xensiv-bgt60trxx-rpi4.yml
```

### validate-kas.sh
Validation script for Kas configurations:

```bash
./validate-kas.sh [CONFIG_FILE...]

# Validate all configurations
./validate-kas.sh

# Validate specific file
./validate-kas.sh kas/xensiv-bgt60trxx-test.yml
```

## 🏗️ Manual Kas Usage

### Basic Commands

```bash
# Build an image
kas build kas/xensiv-bgt60trxx-test.yml

# Build with Docker
kas build --docker kas/xensiv-bgt60trxx-test.yml

# Shell into build environment
kas shell kas/xensiv-bgt60trxx-test.yml

# Build SDK
kas shell kas/xensiv-bgt60trxx-test.yml -c "bitbake xensiv-bgt60trxx-test-image -c populate_sdk"

# Run QEMU (for x86-64 images)
kas shell kas/xensiv-bgt60trxx-test.yml -c "runqemu qemux86-64 nographic"
```

### Advanced Usage

```bash
# Force clean checkout
kas build --force-checkout kas/xensiv-bgt60trxx-test.yml

# Use specific build directory
kas build --build-dir /path/to/build kas/xensiv-bgt60trxx-test.yml

# Dump resolved configuration
kas dump kas/xensiv-bgt60trxx-test.yml
```

## 🎯 Target Images

All configurations build the `xensiv-bgt60trxx-test-image` which includes:

### Core Components
- **xensiv-bgt60trxx** - Main library
- **xensiv-bgt60trxx-examples** - Example applications
- **xensiv-bgt60trxx-dev** - Development headers and tools

### System Features
- **systemd** - Modern init system
- **SPI/I2C/GPIO support** - Hardware interfaces
- **Development tools** - Debugging and profiling tools
- **Package management** - Runtime package installation

### Raspberry Pi Specific
- **Device tree overlays** - SPI interface configuration
- **WiFi firmware** - Broadcom wireless support
- **GPIO utilities** - Hardware control tools

## 🔧 Configuration Structure

### Repository Layout
```yaml
repos:
  # Yocto core layers
  poky:
    url: https://git.yoctoproject.org/poky
    branch: kirkstone  # or scarthgap
    layers:
      meta:
      meta-poky:
      meta-yocto-bsp:

  # OpenEmbedded layers
  meta-openembedded:
    url: https://git.openembedded.org/meta-openembedded
    branch: kirkstone  # or scarthgap
    layers:
      meta-oe:
      meta-python:

  # Hardware-specific layers (RPi4 only)
  meta-raspberrypi:
    url: https://git.yoctoproject.org/meta-raspberrypi
    branch: kirkstone  # or scarthgap
    layers:
      meta-raspberrypi:

  # Local XENSIV layer
  xensiv-bgt60trxx:
    url: "."  # Current repository
    branch: master
    layers:
      meta-xensiv-bgt60trxx:
```

### Local Configuration
Each configuration includes optimized settings for:
- **Build performance** - Parallel builds and caching
- **Hardware support** - Device-specific features
- **Development tools** - Debugging and testing utilities
- **Package selection** - Minimal but functional image

## 🚀 CI/CD Integration

### GitHub Actions
The repository includes automated CI/CD with:
- **Multi-configuration builds** - All supported platforms
- **Caching optimization** - Downloads, sstate, and repositories
- **Artifact collection** - Images, SDKs, and packages
- **QEMU testing** - Boot verification for x86-64 images

### Build Matrix
- Kirkstone LTS (QEMU x86-64, Raspberry Pi 4)
- Scarthgap LTS (QEMU x86-64, Raspberry Pi 4)

## 📦 Build Artifacts

After a successful build, you'll find:

### Images
- **`*.wic`** - Disk images for flashing
- **`*.rootfs.tar.*`** - Root filesystem archives

### SDK
- **`*.sh`** - Self-extracting SDK installer

### Packages
- **RPM packages** - Individual component packages

### Locations
```
build/tmp/deploy/images/<MACHINE>/  # Images
build/tmp/deploy/sdk/               # SDK installers
build/tmp/deploy/rpm/               # RPM packages
```

## 🐛 Troubleshooting

### Common Issues

**Repository reference errors in CI:**
- Fixed by using native `kas` instead of `kas-container`
- Local repository properly mounted in Docker builds

**Disk space issues:**
- Minimum 50GB recommended
- Use `--clean` flag to remove previous builds
- Configure shared sstate cache for multiple builds

**Docker permission issues:**
- Ensure user is in `docker` group
- Use `sudo` if necessary for Docker commands

**Missing dependencies:**
- Install required host packages (see GitHub Actions workflow)
- Use containerized builds to avoid host dependency issues

### Debug Commands

```bash
# Check configuration syntax
./validate-kas.sh

# Verbose build output
./kas-build.sh --verbose

# Shell into build environment
kas shell kas/xensiv-bgt60trxx-test.yml

# Check build logs
less build/tmp/log/cooker/*/console-latest.log
```

## 📚 Additional Resources

- [Kas Documentation](https://kas.readthedocs.io/)
- [Yocto Project Documentation](https://docs.yoctoproject.org/)
- [XENSIV BGT60TRxx Library](../README.md)
- [BitBake User Manual](https://docs.yoctoproject.org/bitbake/)

## 🤝 Contributing

When adding new Kas configurations:

1. **Follow naming convention**: `xensiv-bgt60trxx-<target>-<version>.yml`
2. **Validate syntax**: Run `./validate-kas.sh` 
3. **Test builds**: Verify both native and Docker builds
4. **Update documentation**: Add to this README
5. **Update CI**: Add to GitHub Actions matrix if needed