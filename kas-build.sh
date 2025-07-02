#!/bin/bash

# XENSIV BGT60TRxx Kas Build Script
# This script provides a convenient wrapper around Kas for building Yocto images

set -e

# Default configuration
DEFAULT_CONFIG="kas/xensiv-bgt60trxx-test.yml"
USE_DOCKER=false
CLEAN_BUILD=false
BUILD_SDK=false
RUN_QEMU=false
VERBOSE=false

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Logging functions
log_info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

log_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

log_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

log_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Print usage information
print_usage() {
    cat << EOF
XENSIV BGT60TRxx Kas Build Script

Usage: $0 [OPTIONS] [CONFIG_FILE]

OPTIONS:
    -d, --docker        Use Docker for containerized builds
    -c, --clean         Clean build (force checkout)
    -s, --sdk           Build SDK after image
    -q, --qemu          Run QEMU after successful build (x86-64 only)
    -v, --verbose       Enable verbose output
    -h, --help          Show this help message

CONFIG_FILE:
    Path to Kas configuration file (default: $DEFAULT_CONFIG)

Available configurations:
    kas/xensiv-bgt60trxx-test.yml    - QEMU x86-64 test image
    kas/xensiv-bgt60trxx-rpi4.yml    - Raspberry Pi 4 image

Examples:
    $0                                          # Build default QEMU image
    $0 -d                                       # Build with Docker
    $0 -d -c kas/xensiv-bgt60trxx-rpi4.yml     # Clean build for RPi4 with Docker
    $0 -s -q                                    # Build, create SDK, and run QEMU
    $0 --docker --clean --sdk                  # Full build with Docker, clean, and SDK

EOF
}

# Check if Kas is installed
check_kas() {
    if ! command -v kas &> /dev/null; then
        log_error "Kas is not installed. Please install it with: pip3 install kas"
        exit 1
    fi
    
    log_info "Using Kas version: $(kas --version)"
}

# Check system requirements
check_requirements() {
    log_info "Checking system requirements..."
    
    # Check available disk space (minimum 50GB)
    available_space=$(df . | tail -1 | awk '{print $4}')
    required_space=$((50 * 1024 * 1024)) # 50GB in KB
    
    if [ "$available_space" -lt "$required_space" ]; then
        log_warning "Available disk space: $(($available_space / 1024 / 1024))GB"
        log_warning "Recommended minimum: 50GB"
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            exit 1
        fi
    fi
    
    # Check if Docker is available when requested
    if [ "$USE_DOCKER" = true ]; then
        if ! command -v docker &> /dev/null; then
            log_error "Docker is not installed or not in PATH"
            exit 1
        fi
        
        if ! docker info &> /dev/null; then
            log_error "Docker daemon is not running or not accessible"
            exit 1
        fi
        
        log_info "Docker is available and running"
    fi
}

# Build the image
build_image() {
    local config_file="$1"
    local kas_args=""
    
    if [ "$USE_DOCKER" = true ]; then
        kas_args="$kas_args --docker"
    fi
    
    if [ "$CLEAN_BUILD" = true ]; then
        kas_args="$kas_args --force-checkout"
    fi
    
    if [ "$VERBOSE" = true ]; then
        kas_args="$kas_args --verbose"
    fi
    
    log_info "Building image with configuration: $config_file"
    log_info "Kas arguments: $kas_args"
    
    # Record start time
    start_time=$(date +%s)
    
    # Run the build
    if kas build $kas_args "$config_file"; then
        end_time=$(date +%s)
        duration=$((end_time - start_time))
        log_success "Image build completed in $((duration / 60))m $((duration % 60))s"
        return 0
    else
        log_error "Image build failed"
        return 1
    fi
}

# Build SDK
build_sdk() {
    local config_file="$1"
    local kas_args=""
    
    if [ "$USE_DOCKER" = true ]; then
        kas_args="$kas_args --docker"
    fi
    
    log_info "Building SDK..."
    
    if kas shell $kas_args "$config_file" -c "bitbake xensiv-bgt60trxx-test-image -c populate_sdk"; then
        log_success "SDK build completed"
        
        # Find and display SDK installer
        sdk_installer=$(find build/tmp/deploy/sdk/ -name "*.sh" 2>/dev/null | head -1)
        if [ -n "$sdk_installer" ]; then
            log_info "SDK installer: $sdk_installer"
            log_info "To install SDK: $sdk_installer"
        fi
        return 0
    else
        log_error "SDK build failed"
        return 1
    fi
}

# Run QEMU
run_qemu() {
    local config_file="$1"
    local kas_args=""
    
    if [ "$USE_DOCKER" = true ]; then
        kas_args="$kas_args --docker"
    fi
    
    # Check if this is a QEMU-compatible configuration
    if ! grep -q "qemu" "$config_file"; then
        log_warning "Configuration doesn't appear to be QEMU-compatible"
        read -p "Continue anyway? (y/N): " -n 1 -r
        echo
        if [[ ! $REPLY =~ ^[Yy]$ ]]; then
            return 0
        fi
    fi
    
    log_info "Starting QEMU..."
    log_info "Use Ctrl+A, X to exit QEMU"
    
    # Determine machine type from config
    machine=$(grep "^machine:" "$config_file" | cut -d' ' -f2)
    
    kas shell $kas_args "$config_file" -c "runqemu $machine nographic"
}

# Display build results
show_results() {
    local config_file="$1"
    
    log_info "Build artifacts:"
    
    # Find machine name
    machine=$(grep "^machine:" "$config_file" | cut -d' ' -f2 || echo "unknown")
    
    # Show images
    if [ -d "build/tmp/deploy/images/$machine" ]; then
        echo "Images:"
        ls -lh "build/tmp/deploy/images/$machine"/*.wic* 2>/dev/null || true
        ls -lh "build/tmp/deploy/images/$machine"/*.rootfs.* 2>/dev/null || true
    fi
    
    # Show packages
    if [ -d "build/tmp/deploy/rpm" ]; then
        echo -e "\nXENSIV BGT60TRxx packages:"
        find build/tmp/deploy/rpm -name "*xensiv*" -type f 2>/dev/null | head -5
    fi
    
    # Show SDK
    if [ -d "build/tmp/deploy/sdk" ]; then
        echo -e "\nSDK:"
        ls -lh build/tmp/deploy/sdk/*.sh 2>/dev/null || true
    fi
}

# Main function
main() {
    local config_file="$DEFAULT_CONFIG"
    
    # Parse command line arguments
    while [[ $# -gt 0 ]]; do
        case $1 in
            -d|--docker)
                USE_DOCKER=true
                shift
                ;;
            -c|--clean)
                CLEAN_BUILD=true
                shift
                ;;
            -s|--sdk)
                BUILD_SDK=true
                shift
                ;;
            -q|--qemu)
                RUN_QEMU=true
                shift
                ;;
            -v|--verbose)
                VERBOSE=true
                shift
                ;;
            -h|--help)
                print_usage
                exit 0
                ;;
            -*)
                log_error "Unknown option: $1"
                print_usage
                exit 1
                ;;
            *)
                config_file="$1"
                shift
                ;;
        esac
    done
    
    # Validate configuration file
    if [ ! -f "$config_file" ]; then
        log_error "Configuration file not found: $config_file"
        exit 1
    fi
    
    log_info "Starting XENSIV BGT60TRxx Yocto build"
    log_info "Configuration: $config_file"
    log_info "Docker: $USE_DOCKER"
    log_info "Clean build: $CLEAN_BUILD"
    log_info "Build SDK: $BUILD_SDK"
    log_info "Run QEMU: $RUN_QEMU"
    
    # Run checks
    check_kas
    check_requirements
    
    # Build image
    if ! build_image "$config_file"; then
        exit 1
    fi
    
    # Build SDK if requested
    if [ "$BUILD_SDK" = true ]; then
        if ! build_sdk "$config_file"; then
            log_warning "SDK build failed, but continuing..."
        fi
    fi
    
    # Show results
    show_results "$config_file"
    
    # Run QEMU if requested
    if [ "$RUN_QEMU" = true ]; then
        run_qemu "$config_file"
    fi
    
    log_success "Build process completed successfully!"
}

# Run main function with all arguments
main "$@"