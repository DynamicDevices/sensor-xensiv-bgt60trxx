# Release Notes - XENSIV™ BGT60TRxx Library v1.1.1

**Release Date:** January 2025  
**Type:** Patch Release

## 🔧 Bug Fixes & Improvements

### CI/CD Pipeline Fixes
- **Fixed build script test failures**: Corrected command line arguments in CI workflows
  - Changed `--examples` to `--enable-examples` 
  - Changed `--debug` to `-t Debug`
- **Removed duplicate integration test**: Eliminated redundant integration test execution in build script test job
- **Fixed static analysis warnings**: Reduced variable scope in `examples/config_example.c` to resolve cppcheck warnings
- **Fixed library linking**: Corrected library paths for integration tests in Autotools builds
- **Improved CMake builds**: Added `BUILD_SHARED_LIBS=OFF` for consistent static library builds

### Platform Support Updates
- **Ubuntu 20.04 retirement**: Migrated all CI workflows from Ubuntu 20.04 to Ubuntu 24.04
  - Ubuntu 20.04 LTS scheduled for retirement on April 15, 2025
  - Updated to Ubuntu 22.04 and 24.04 for better future compatibility
  - Updated compiler versions: GCC 13 and Clang 18 for Ubuntu 24.04

### Code Quality Improvements
- **Applied consistent formatting**: Used clang-format across all source files
- **Enhanced build reliability**: Multiple fixes to ensure CI pipeline stability

## 🧪 Testing & Validation

- All CI workflows now pass successfully
- Integration tests validated on multiple Ubuntu versions
- Static analysis warnings resolved
- Memory leak detection continues to pass
- Cross-compilation for ARM platforms verified

## 📦 Build System

- **CMake**: Improved configuration and build reliability
- **Autotools**: Fixed library path issues and enhanced compatibility
- **Build Script**: Corrected argument handling and improved usability

## 🔄 Compatibility

- **Backward Compatible**: No breaking changes to the API
- **Platform Support**: Linux, Yocto, ModusToolbox (unchanged)
- **Compiler Support**: Enhanced with newer GCC and Clang versions

## 📋 Files Changed

- `.github/workflows/ci.yml` - CI pipeline fixes and Ubuntu migration
- `.github/workflows/release.yml` - Ubuntu version updates
- `examples/config_example.c` - Variable scope optimization
- Various source files - Code formatting improvements

## 🚀 Upgrade Notes

This is a patch release with no breaking changes. Users can upgrade directly from v1.1.0 without any code modifications.

## 🔗 Links

- **Repository**: https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx
- **Documentation**: See README.md and IMPLEMENTATION_SUMMARY.md
- **Issues**: https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx/issues

---

**Full Changelog**: https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx/compare/v1.1.0...v1.1.1