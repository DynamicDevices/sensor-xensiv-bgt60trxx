# Contributing to XENSIV™ BGT60TRxx Cross-Platform Library

Thank you for your interest in contributing to the XENSIV™ BGT60TRxx cross-platform library! This document provides guidelines and information for contributors.

## 🤝 How to Contribute

### Reporting Issues
- Use the [GitHub Issues](https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx/issues) page
- Choose the appropriate issue template:
  - **Bug Report**: For reporting bugs or unexpected behavior
  - **Feature Request**: For suggesting new features or improvements
  - **Platform Support**: For requesting support for new platforms
- Provide as much detail as possible, including:
  - Environment information (OS, architecture, compiler)
  - Steps to reproduce the issue
  - Expected vs. actual behavior
  - Code samples and error messages

### Submitting Pull Requests
1. **Fork the Repository**
   ```bash
   git clone https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git
   cd sensor-xensiv-bgt60trxx
   ```

2. **Create a Feature Branch**
   ```bash
   git checkout -b feature/your-feature-name
   # or
   git checkout -b bugfix/issue-description
   ```

3. **Make Your Changes**
   - Follow the coding standards (see below)
   - Add tests for new functionality
   - Update documentation as needed

4. **Test Your Changes**
   ```bash
   # Test both build systems
   ./build.sh --clean --examples
   ./build.sh -s autotools --clean --examples
   
   # Run integration tests
   ./test_integration
   
   # Test examples
   ./build/examples/basic_example --help
   ```

5. **Commit Your Changes**
   ```bash
   git add .
   git commit -m "feat: add new feature description"
   # or
   git commit -m "fix: resolve issue with specific component"
   ```

6. **Push and Create Pull Request**
   ```bash
   git push origin feature/your-feature-name
   ```
   Then create a pull request through GitHub.

## 📋 Development Guidelines

### Code Style
- **Language**: C99 standard
- **Formatting**: Use the provided `.clang-format` configuration
- **Naming Conventions**:
  - Functions: `snake_case`
  - Variables: `snake_case`
  - Constants: `UPPER_CASE`
  - Types: `snake_case_t`
  - Macros: `UPPER_CASE`

### Code Quality Standards
- **Compiler Warnings**: Code must compile without warnings using `-Wall -Wextra -Wpedantic`
- **Static Analysis**: Must pass clang-tidy checks (see `.clang-tidy`)
- **Memory Safety**: No memory leaks or buffer overflows
- **Error Handling**: Comprehensive error checking and recovery

### Documentation Requirements
- **Public APIs**: All public functions must have documentation comments
- **Code Comments**: Complex logic should be well-commented
- **Examples**: New features should include usage examples
- **README Updates**: Update README.md for significant changes

### Testing Requirements
- **Integration Tests**: Add tests to `test_integration.c` for new functionality
- **Example Programs**: Ensure examples demonstrate new features
- **Platform Testing**: Test on multiple platforms when possible
- **Hardware Testing**: Test with actual hardware when applicable

## 🏗️ Development Setup

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt-get install build-essential cmake autoconf automake libtool pkg-config

# CentOS/RHEL/Fedora
sudo yum install gcc cmake autoconf automake libtool pkgconfig-devel
```

### Development Tools (Optional)
```bash
# Static analysis and formatting
sudo apt-get install clang-tools cppcheck valgrind

# Documentation generation
sudo apt-get install doxygen graphviz
```

### Building for Development
```bash
# Quick development build
./build.sh --clean --examples --debug

# Full validation build
./build.sh --clean --examples
./build.sh -s autotools --clean --examples
```

### Running Tests
```bash
# Integration test
./test_integration

# Static analysis
cppcheck --enable=all *.c *.h examples/*.c

# Memory leak check
valgrind --leak-check=full ./test_integration

# Format check
clang-format --dry-run --Werror *.c *.h examples/*.c
```

## 🎯 Contribution Areas

### High Priority
- **Bug Fixes**: Issues affecting core functionality
- **Platform Support**: New platform implementations
- **Performance Improvements**: Optimization of critical paths
- **Documentation**: API documentation and usage guides

### Medium Priority
- **Feature Enhancements**: New sensor features or capabilities
- **Build System Improvements**: Better cross-compilation support
- **Testing**: Additional test coverage and validation
- **Examples**: More comprehensive usage examples

### Future Enhancements
- **Language Bindings**: Python, C++, Rust bindings
- **Framework Integration**: ROS, Arduino, ESP-IDF support
- **Advanced Features**: Multi-sensor management, calibration algorithms
- **Platform Ports**: Windows, macOS, RTOS support

## 🔧 Platform-Specific Contributions

### Linux Platform
- **File Location**: `xensiv_bgt60trxx_linux.c/h`
- **Requirements**: SPI and GPIO interface implementation
- **Testing**: Validate on multiple Linux distributions

### ModusToolbox Platform
- **File Location**: `xensiv_bgt60trxx_mtb.c/h`
- **Requirements**: Maintain compatibility with existing MTB projects
- **Testing**: Validate with ModusToolbox development environment

### New Platform Support
1. **Create Platform Files**: `xensiv_bgt60trxx_<platform>.c/h`
2. **Implement Interface**: Follow the platform abstraction pattern
3. **Add Build Support**: Update CMakeLists.txt and configure.ac
4. **Create Examples**: Platform-specific usage examples
5. **Documentation**: Platform setup and usage guide

## 📝 Commit Message Guidelines

Use conventional commit format:
- `feat:` New features
- `fix:` Bug fixes
- `docs:` Documentation changes
- `style:` Code style changes (formatting, etc.)
- `refactor:` Code refactoring
- `test:` Adding or updating tests
- `build:` Build system changes
- `ci:` CI/CD changes

Examples:
```
feat: add support for BGT60UTR11 sensor variant
fix: resolve SPI communication timeout on ARM platforms
docs: update README with Yocto integration examples
build: add cross-compilation support for RISC-V
```

## 🔍 Code Review Process

### Review Criteria
- **Functionality**: Does the code work as intended?
- **Code Quality**: Follows coding standards and best practices?
- **Testing**: Adequate test coverage and validation?
- **Documentation**: Proper documentation and comments?
- **Compatibility**: Maintains backward compatibility?
- **Performance**: No significant performance regressions?

### Review Timeline
- **Initial Review**: Within 48 hours for acknowledgment
- **Detailed Review**: Within 1 week for feedback
- **Merge Decision**: Based on review feedback and CI results

## 🚀 Release Process

### Version Numbering
- **Major**: Breaking changes (x.0.0)
- **Minor**: New features, backward compatible (0.x.0)
- **Patch**: Bug fixes, backward compatible (0.0.x)

### Release Checklist
- [ ] All CI checks pass
- [ ] Documentation updated
- [ ] CHANGELOG.md updated
- [ ] Version numbers updated
- [ ] Release notes prepared
- [ ] Tagged release created

## 📞 Getting Help

### Communication Channels
- **GitHub Issues**: Bug reports and feature requests
- **GitHub Discussions**: General questions and community support
- **Pull Request Comments**: Code-specific discussions

### Maintainer Contact
- **Primary Maintainer**: Dynamic Devices team
- **Response Time**: Typically within 48 hours
- **Commercial Support**: Available for enterprise users

## 📜 License and Legal

### Contribution License
By contributing to this project, you agree that your contributions will be licensed under the same license as the project (MIT License).

### Copyright
- **Original Work**: Copyright 2022 Infineon Technologies AG
- **Cross-Platform Implementation**: Copyright 2024 Dynamic Devices
- **Your Contributions**: You retain copyright but grant usage rights

### Code of Conduct
- Be respectful and inclusive
- Focus on constructive feedback
- Help create a welcoming environment for all contributors
- Follow professional communication standards

## 🎉 Recognition

### Contributors
All contributors will be recognized in:
- **README.md**: Contributors section
- **Release Notes**: Acknowledgment of contributions
- **Git History**: Permanent record of contributions

### Significant Contributions
Major contributions may be recognized through:
- **Special Acknowledgment**: In project documentation
- **Maintainer Status**: For ongoing significant contributors
- **Commercial Opportunities**: Potential collaboration opportunities

---

Thank you for contributing to the XENSIV™ BGT60TRxx cross-platform library! Your contributions help make this library better for the entire embedded systems community.

**Repository**: https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx  
**Maintainer**: Dynamic Devices  
**License**: MIT