# Kas Build Support for XENSIV™ BGT60TRxx

This directory contains [Kas](https://kas.readthedocs.io/) configuration files for building Yocto Embedded Linux images with the XENSIV™ BGT60TRxx radar sensor library.

## Prerequisites

1. **Install Kas**:
   ```bash
   pip3 install kas
   ```

2. **System Requirements**:
   - Linux host system (Ubuntu 20.04+ recommended)
   - At least 50GB free disk space
   - 8GB+ RAM recommended
   - Docker (optional, for containerized builds)

## Available Configurations

### 1. QEMU x86-64 Test Image
**File**: `xensiv-bgt60trxx-test.yml`
- **Target**: QEMU x86-64 virtual machine
- **Purpose**: Development and testing
- **Features**: Full development environment with debugging tools

### 2. Raspberry Pi 4 Image
**File**: `xensiv-bgt60trxx-rpi4.yml`
- **Target**: Raspberry Pi 4 (64-bit)
- **Purpose**: Hardware testing with real SPI/GPIO interfaces
- **Features**: SPI and GPIO enabled, device tree overlays

## Quick Start

### Build QEMU Test Image

```bash
# Clone the repository
git clone https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git
cd sensor-xensiv-bgt60trxx

# Build the image
kas build kas/xensiv-bgt60trxx-test.yml

# Run in QEMU
kas shell kas/xensiv-bgt60trxx-test.yml -c "runqemu qemux86-64 nographic"
```

### Build Raspberry Pi 4 Image

```bash
# Build the image
kas build kas/xensiv-bgt60trxx-rpi4.yml

# Flash to SD card (replace /dev/sdX with your SD card device)
sudo dd if=build/tmp/deploy/images/raspberrypi4-64/xensiv-bgt60trxx-test-image-raspberrypi4-64.wic \
        of=/dev/sdX bs=4M status=progress conv=fsync
```

## Using Docker (Recommended)

For a clean, reproducible build environment:

```bash
# Build with Docker
kas-container build kas/xensiv-bgt60trxx-test.yml

# Shell access with Docker
kas-container shell kas/xensiv-bgt60trxx-test.yml
```

## Build Artifacts

After a successful build, you'll find the following artifacts in `build/tmp/deploy/`:

- **Images**: `images/<machine>/xensiv-bgt60trxx-test-image-<machine>.*`
- **Packages**: `rpm/<arch>/xensiv-bgt60trxx-*`
- **SDK**: `sdk/poky-glibc-*-toolchain-*.sh`

## Testing the Library

### In QEMU

1. **Boot the image**:
   ```bash
   kas shell kas/xensiv-bgt60trxx-test.yml -c "runqemu qemux86-64 nographic"
   ```

2. **Test the library** (in the QEMU guest):
   ```bash
   # Check library installation
   pkg-config --modversion xensiv_bgt60trxx
   
   # Run basic example (will fail without hardware, but tests linking)
   /usr/bin/examples/basic_example --help
   
   # Check library files
   ls -la /usr/lib/libxensiv_bgt60trxx.*
   ls -la /usr/include/xensiv_bgt60trxx*
   ```

### On Raspberry Pi 4

1. **Flash and boot the image**
2. **Connect BGT60TRxx sensor** via SPI
3. **Run examples**:
   ```bash
   # Basic sensor detection
   /usr/bin/examples/basic_example
   
   # FIFO data reading
   /usr/bin/examples/fifo_example
   
   # Configuration example
   /usr/bin/examples/config_example
   ```

## Customization

### Adding Packages

Edit the image recipe at `meta-xensiv-bgt60trxx/recipes-core/images/xensiv-bgt60trxx-test-image.bb`:

```bitbake
IMAGE_INSTALL += "\
    your-additional-package \
"
```

### Changing Target Machine

Modify the `machine:` field in the Kas YAML file:

```yaml
machine: qemuarm64  # or beaglebone-yocto, etc.
```

### Custom Layer Integration

Add your custom layer to the `repos:` section:

```yaml
repos:
  my-custom-layer:
    url: https://github.com/user/my-layer.git
    refspec: main
    layers:
      custom: .
```

## Troubleshooting

### Build Failures

1. **Clean build**:
   ```bash
   kas build --force-checkout kas/xensiv-bgt60trxx-test.yml
   ```

2. **Check disk space**:
   ```bash
   df -h
   ```

3. **Update Kas**:
   ```bash
   pip3 install --upgrade kas
   ```

### QEMU Issues

1. **Graphics problems**: Use `nographic` option
2. **Network issues**: Check host firewall settings
3. **Performance**: Increase RAM allocation in local.conf

### Hardware Issues

1. **SPI not working**: Check device tree overlays
2. **GPIO permissions**: Ensure user is in `gpio` group
3. **Sensor detection**: Verify wiring and power supply

## Development Workflow

### Interactive Development

```bash
# Start development shell
kas shell kas/xensiv-bgt60trxx-test.yml

# Inside the shell, you can:
bitbake xensiv-bgt60trxx -c compile
bitbake xensiv-bgt60trxx -c install
bitbake xensiv-bgt60trxx-test-image
```

### SDK Generation

```bash
# Build SDK for cross-compilation
kas shell kas/xensiv-bgt60trxx-test.yml -c "bitbake xensiv-bgt60trxx-test-image -c populate_sdk"

# Install SDK
./build/tmp/deploy/sdk/poky-glibc-*-toolchain-*.sh
```

## Integration with CI/CD

Example GitHub Actions workflow:

```yaml
name: Yocto Build
on: [push, pull_request]
jobs:
  yocto-build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4
      - name: Install Kas
        run: pip3 install kas
      - name: Build Image
        run: kas-container build kas/xensiv-bgt60trxx-test.yml
      - name: Upload Artifacts
        uses: actions/upload-artifact@v4
        with:
          name: yocto-image
          path: build/tmp/deploy/images/
```

## Support

For issues related to:
- **Kas configuration**: Check [Kas documentation](https://kas.readthedocs.io/)
- **Yocto builds**: Check [Yocto Project documentation](https://docs.yoctoproject.org/)
- **XENSIV library**: Open an issue in this repository

## License

This Kas configuration is provided under the same MIT license as the XENSIV™ BGT60TRxx library.