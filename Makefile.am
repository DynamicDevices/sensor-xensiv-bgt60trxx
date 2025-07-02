ACLOCAL_AMFLAGS = -I m4

lib_LIBRARIES = libxensiv_bgt60trxx.a

# Core sources - always include the main source
libxensiv_bgt60trxx_a_SOURCES = xensiv_bgt60trxx.c

# Platform-specific sources
if ENABLE_LINUX_SUPPORT
libxensiv_bgt60trxx_a_SOURCES += xensiv_bgt60trxx_linux.c
endif

# Headers to install
include_HEADERS = \
    xensiv_bgt60trxx.h \
    xensiv_bgt60trxx_regs.h \
    xensiv_bgt60trxx_platform.h

if ENABLE_LINUX_SUPPORT
include_HEADERS += xensiv_bgt60trxx_linux.h
endif

# Compiler flags
AM_CFLAGS = -Wall -Wextra -std=c99

# pkg-config file
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = xensiv_bgt60trxx.pc

# Subdirectories - build examples after the main library
SUBDIRS = .
if ENABLE_EXAMPLES
SUBDIRS += examples
endif

EXTRA_DIST = \
    README.md \
    LICENSE \
    EULA \
    RELEASE.md \
    IMPLEMENTATION_SUMMARY.md \
    FINAL_STATUS.md \
    CONTRIBUTING.md \
    xensiv_bgt60trxx.pc.in \
    CMakeLists.txt \
    cmake/ \
    yocto/ \
    .github/ \
    .clang-format \
    .clang-tidy \
    test_integration.c \
    build.sh