# Yocto/Kas CI Quick Reference

## Overview
This project uses GitHub Actions to automatically build Yocto images for the XENSIV BGT60TRxx radar sensor library using Kas (Kernel, Application, and System) build tool.

## Build Matrix
The CI builds 4 different configurations:

| Configuration | Machine | Yocto Version | Description |
|---------------|---------|---------------|-------------|
| QEMU x86-64 (Kirkstone) | qemux86-64 | kirkstone | QEMU emulation, LTS |
| Raspberry Pi 4 (Kirkstone) | raspberrypi4-64 | kirkstone | RPi4 64-bit, LTS |
| QEMU x86-64 (Scarthgap) | qemux86-64 | scarthgap | QEMU emulation, Latest LTS |
| Raspberry Pi 4 (Scarthgap) | raspberrypi4-64 | scarthgap | RPi4 64-bit, Latest LTS |

## Kas Configuration Files

### CI Configurations (Used by GitHub Actions)
- `kas/xensiv-bgt60trxx-test-kirkstone-ci.yml` - QEMU x86-64 (Kirkstone)
- `kas/xensiv-bgt60trxx-rpi4-kirkstone-ci.yml` - Raspberry Pi 4 (Kirkstone)
- `kas/xensiv-bgt60trxx-test-scarthgap-ci.yml` - QEMU x86-64 (Scarthgap)
- `kas/xensiv-bgt60trxx-rpi4-scarthgap-ci.yml` - Raspberry Pi 4 (Scarthgap)

### Local Development Configurations
- `kas/xensiv-bgt60trxx-test.yml` - QEMU x86-64 (Kirkstone, local repo)
- `kas/xensiv-bgt60trxx-rpi4.yml` - Raspberry Pi 4 (Kirkstone, local repo)
- `kas/xensiv-bgt60trxx-test-scarthgap.yml` - QEMU x86-64 (Scarthgap, local repo)
- `kas/xensiv-bgt60trxx-rpi4-scarthgap.yml` - Raspberry Pi 4 (Scarthgap, local repo)

### Base Configurations
- `kas/base-image.yml` - Common settings for Kirkstone builds
- `kas/base-image-scarthgap.yml` - Common settings for Scarthgap builds

## CI Workflow Triggers

### Automatic Triggers
- **Push** to branches: `main`, `master`, `develop`, `feature/*`
- **Pull Request** to branches: `main`, `master`, `develop`

### Manual Trigger
- **Workflow Dispatch**: Allows manual execution with configuration selection

## Build Process

### 1. Environment Setup
- Ubuntu 22.04 runner
- Disk space cleanup (removes ~14GB of unused software)
- Install Yocto dependencies and Kas
- Set up locale (en_US.UTF-8)

### 2. Validation
- Validate Kas configuration files using `validate-kas.sh`
- Check YAML syntax and required fields
- Perform dry-run validation with Kas

### 3. Caching Strategy
- **Downloads Cache**: Yocto source downloads
- **Sstate Cache**: Shared state cache for faster builds
- **Repository Cache**: Git repositories (poky, meta-openembedded, etc.)
- **Pip Cache**: Python packages

### 4. Build Execution
- Build main image using `kas build`
- Build SDK (QEMU x86-64 only)
- Test QEMU boot (QEMU x86-64 only, with 5-minute timeout)

### 5. Artifact Collection
- Images (*.wic, *.rootfs.tar.*)
- SDK installers (*.sh)
- XENSIV packages (RPM files)
- Build information and logs

## Local Development

### Prerequisites
```bash
# Install Kas
pip3 install kas

# Verify installation
kas --version
```

### Build Commands
```bash
# Build with specific configuration
kas build kas/xensiv-bgt60trxx-test.yml

# Build with build script (recommended)
./kas-build.sh -c kas/xensiv-bgt60trxx-test.yml

# Build with Docker
./kas-build.sh -c kas/xensiv-bgt60trxx-test.yml -d

# Build with SDK
./kas-build.sh -c kas/xensiv-bgt60trxx-test.yml -s

# Build and test in QEMU
./kas-build.sh -c kas/xensiv-bgt60trxx-test.yml -q
```

### Validation
```bash
# Validate all configurations
./validate-kas.sh

# Validate specific configuration
./validate-kas.sh kas/xensiv-bgt60trxx-test.yml
```

## Troubleshooting

### Common Issues
1. **Disk Space**: CI includes aggressive cleanup, but local builds may need manual cleanup
2. **Memory Issues**: Limited to 4 parallel threads in CI
3. **Network Issues**: Check repository URLs and network connectivity
4. **Cache Issues**: Clear cache directories if builds fail unexpectedly

### Debug Commands
```bash
# Check build logs
ls build/tmp/log/

# Check specific package logs
ls build/tmp/work/*/xensiv-bgt60trxx/*/temp/

# Shell into build environment
kas shell kas/config.yml

# Build specific package with verbose output
kas shell kas/config.yml -c "bitbake -v xensiv-bgt60trxx"
```

### Log Locations
- **CI Logs**: Available as GitHub Actions artifacts
- **Local Logs**: `build/tmp/log/` directory
- **Package Logs**: `build/tmp/work/*/temp/` directories

## Artifacts

### Build Artifacts (30-day retention)
- **Images**: Bootable images for target hardware
- **SDK**: Cross-compilation toolchain
- **Packages**: RPM packages for XENSIV library
- **Build Info**: Configuration and version information

### Build Logs (7-day retention, failure only)
- **BitBake Logs**: Complete build logs
- **Package Logs**: Individual package build logs
- **Configuration**: Build configuration files

## Performance Optimization

### CI Optimizations
- Multi-level caching strategy
- Parallel builds (limited to 4 threads)
- Disk space monitoring
- Aggressive cleanup of temporary files

### Local Optimizations
```bash
# Use shared state cache
export SSTATE_DIR=/path/to/shared/sstate-cache

# Use shared downloads
export DL_DIR=/path/to/shared/downloads

# Enable parallel builds
export BB_NUMBER_THREADS=8
export PARALLEL_MAKE="-j8"
```

## Configuration Management

### Version Control
- All configurations are version controlled
- Use semantic versioning for releases
- Tag stable configurations

### Testing
- Test locally before pushing changes
- Use feature branches for experimental changes
- Validate configurations before merging

## Monitoring

### Build Status
- Check GitHub Actions for build status
- Monitor build times for performance regression
- Set up notifications for build failures

### Resource Usage
- Monitor disk space usage
- Track cache hit rates
- Monitor artifact sizes

## Support

### Documentation
- [CI Troubleshooting Guide](CI_TROUBLESHOOTING.md)
- [Kas Implementation Details](KAS_IMPLEMENTATION.md)
- [Project Review](PROJECT_REVIEW.md)

### Community Resources
- [Yocto Project Documentation](https://docs.yoctoproject.org/)
- [Kas Documentation](https://kas.readthedocs.io/)
- [OpenEmbedded Layer Index](https://layers.openembedded.org/)

### Getting Help
1. Check build logs first
2. Try to reproduce locally
3. Validate configurations
4. Check community resources
5. Create GitHub issue with logs and configuration