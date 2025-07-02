#!/bin/bash

# XENSIV BGT60TRxx Cross-Platform Build Script
# Supports multiple build systems and platforms

set -e

# Default values
BUILD_TYPE="Release"
BUILD_SYSTEM="cmake"
ENABLE_EXAMPLES="ON"
ENABLE_TESTS="OFF"
ENABLE_LINUX="ON"
ENABLE_MTB="OFF"
INSTALL_PREFIX="/usr/local"
BUILD_DIR="build"
JOBS=$(nproc)

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

print_usage() {
    echo "Usage: $0 [options]"
    echo ""
    echo "Options:"
    echo "  -s, --build-system <cmake|autotools>  Build system to use (default: cmake)"
    echo "  -t, --build-type <Debug|Release>      Build type (default: Release)"
    echo "  -p, --prefix <path>                   Install prefix (default: /usr/local)"
    echo "  -b, --build-dir <dir>                 Build directory (default: build)"
    echo "  -j, --jobs <num>                      Number of parallel jobs (default: $(nproc))"
    echo "  --enable-examples                     Enable example applications (default)"
    echo "  --disable-examples                    Disable example applications"
    echo "  --enable-tests                        Enable test applications"
    echo "  --disable-tests                       Disable test applications (default)"
    echo "  --enable-linux                        Enable Linux support (default)"
    echo "  --disable-linux                       Disable Linux support"
    echo "  --enable-mtb                          Enable ModusToolbox support"
    echo "  --disable-mtb                         Disable ModusToolbox support (default)"
    echo "  --clean                               Clean build directory before building"
    echo "  --install                             Install after building"
    echo "  -h, --help                            Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                                    # Build with default settings"
    echo "  $0 -s autotools -t Debug --install   # Build with autotools in debug mode and install"
    echo "  $0 --enable-tests --clean             # Clean build with tests enabled"
}

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

check_dependencies() {
    log_info "Checking dependencies..."
    
    if [ "$BUILD_SYSTEM" = "cmake" ]; then
        if ! command -v cmake &> /dev/null; then
            log_error "CMake is required but not installed"
            exit 1
        fi
    elif [ "$BUILD_SYSTEM" = "autotools" ]; then
        if ! command -v autoconf &> /dev/null || ! command -v automake &> /dev/null; then
            log_error "Autotools (autoconf, automake) are required but not installed"
            exit 1
        fi
    fi
    
    if ! command -v gcc &> /dev/null && ! command -v clang &> /dev/null; then
        log_error "A C compiler (gcc or clang) is required but not installed"
        exit 1
    fi
    
    log_success "Dependencies check passed"
}

build_cmake() {
    log_info "Building with CMake..."
    
    mkdir -p "$BUILD_DIR"
    cd "$BUILD_DIR"
    
    CMAKE_ARGS=(
        "-DCMAKE_BUILD_TYPE=$BUILD_TYPE"
        "-DCMAKE_INSTALL_PREFIX=$INSTALL_PREFIX"
        "-DBUILD_EXAMPLES=$ENABLE_EXAMPLES"
        "-DBUILD_TESTS=$ENABLE_TESTS"
        "-DENABLE_LINUX_SUPPORT=$ENABLE_LINUX"
        "-DENABLE_MTB_SUPPORT=$ENABLE_MTB"
    )
    
    log_info "Running cmake with args: ${CMAKE_ARGS[*]}"
    cmake .. "${CMAKE_ARGS[@]}"
    
    log_info "Building with $JOBS parallel jobs..."
    make -j"$JOBS"
    
    cd ..
    log_success "CMake build completed"
}

build_autotools() {
    log_info "Building with Autotools..."
    
    # Generate configure script if it doesn't exist
    if [ ! -f configure ]; then
        log_info "Generating configure script..."
        autoreconf -fiv
    fi
    
    CONFIGURE_ARGS=()
    
    if [ "$ENABLE_EXAMPLES" = "ON" ]; then
        CONFIGURE_ARGS+=(--enable-examples)
    else
        CONFIGURE_ARGS+=(--disable-examples)
    fi
    
    if [ "$BUILD_TYPE" = "Debug" ]; then
        CONFIGURE_ARGS+=(--enable-debug)
    fi
    
    if [ "$ENABLE_LINUX" = "OFF" ]; then
        CONFIGURE_ARGS+=(--disable-linux-support)
    fi
    
    CONFIGURE_ARGS+=(--prefix="$INSTALL_PREFIX")
    
    log_info "Running configure with args: ${CONFIGURE_ARGS[*]}"
    ./configure "${CONFIGURE_ARGS[@]}"
    
    log_info "Building with $JOBS parallel jobs..."
    make -j"$JOBS"
    
    log_success "Autotools build completed"
}

# Parse command line arguments
CLEAN=false
INSTALL=false

while [[ $# -gt 0 ]]; do
    case $1 in
        -s|--build-system)
            BUILD_SYSTEM="$2"
            shift 2
            ;;
        -t|--build-type)
            BUILD_TYPE="$2"
            shift 2
            ;;
        -p|--prefix)
            INSTALL_PREFIX="$2"
            shift 2
            ;;
        -b|--build-dir)
            BUILD_DIR="$2"
            shift 2
            ;;
        -j|--jobs)
            JOBS="$2"
            shift 2
            ;;
        --enable-examples)
            ENABLE_EXAMPLES="ON"
            shift
            ;;
        --disable-examples)
            ENABLE_EXAMPLES="OFF"
            shift
            ;;
        --enable-tests)
            ENABLE_TESTS="ON"
            shift
            ;;
        --disable-tests)
            ENABLE_TESTS="OFF"
            shift
            ;;
        --enable-linux)
            ENABLE_LINUX="ON"
            shift
            ;;
        --disable-linux)
            ENABLE_LINUX="OFF"
            shift
            ;;
        --enable-mtb)
            ENABLE_MTB="ON"
            shift
            ;;
        --disable-mtb)
            ENABLE_MTB="OFF"
            shift
            ;;
        --clean)
            CLEAN=true
            shift
            ;;
        --install)
            INSTALL=true
            shift
            ;;
        -h|--help)
            print_usage
            exit 0
            ;;
        *)
            log_error "Unknown option: $1"
            print_usage
            exit 1
            ;;
    esac
done

# Validate build system
if [ "$BUILD_SYSTEM" != "cmake" ] && [ "$BUILD_SYSTEM" != "autotools" ]; then
    log_error "Invalid build system: $BUILD_SYSTEM (must be cmake or autotools)"
    exit 1
fi

# Validate build type
if [ "$BUILD_TYPE" != "Debug" ] && [ "$BUILD_TYPE" != "Release" ]; then
    log_error "Invalid build type: $BUILD_TYPE (must be Debug or Release)"
    exit 1
fi

# Print configuration
log_info "Build configuration:"
echo "  Build system: $BUILD_SYSTEM"
echo "  Build type: $BUILD_TYPE"
echo "  Install prefix: $INSTALL_PREFIX"
echo "  Build directory: $BUILD_DIR"
echo "  Parallel jobs: $JOBS"
echo "  Examples: $ENABLE_EXAMPLES"
echo "  Tests: $ENABLE_TESTS"
echo "  Linux support: $ENABLE_LINUX"
echo "  MTB support: $ENABLE_MTB"
echo ""

# Clean if requested
if [ "$CLEAN" = true ]; then
    log_info "Cleaning build directory..."
    rm -rf "$BUILD_DIR"
    if [ "$BUILD_SYSTEM" = "autotools" ]; then
        make distclean 2>/dev/null || true
    fi
    log_success "Clean completed"
fi

# Check dependencies
check_dependencies

# Build
case $BUILD_SYSTEM in
    cmake)
        build_cmake
        ;;
    autotools)
        build_autotools
        ;;
esac

# Install if requested
if [ "$INSTALL" = true ]; then
    log_info "Installing..."
    if [ "$BUILD_SYSTEM" = "cmake" ]; then
        cd "$BUILD_DIR"
        sudo make install
        cd ..
    else
        sudo make install
    fi
    log_success "Installation completed"
fi

log_success "Build script completed successfully!"

# Print next steps
echo ""
log_info "Next steps:"
if [ "$INSTALL" = false ]; then
    echo "  - Run '$0 --install' to install the library"
fi
if [ "$ENABLE_EXAMPLES" = "ON" ]; then
    if [ "$BUILD_SYSTEM" = "cmake" ]; then
        echo "  - Run examples from: $BUILD_DIR/examples/"
    else
        echo "  - Run examples from: examples/"
    fi
fi
echo "  - Check the README.md for usage instructions"