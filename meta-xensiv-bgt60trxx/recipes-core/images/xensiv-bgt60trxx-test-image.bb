SUMMARY = "XENSIV BGT60TRxx Test Image"
DESCRIPTION = "A test image containing the XENSIV BGT60TRxx radar sensor library and examples"

LICENSE = "MIT"

inherit core-image

# Base image features
IMAGE_FEATURES += "ssh-server-openssh"
IMAGE_FEATURES += "debug-tweaks"
IMAGE_FEATURES += "tools-debug"
IMAGE_FEATURES += "tools-profile"
IMAGE_FEATURES += "package-management"

# Core packages
IMAGE_INSTALL = "\
    packagegroup-core-boot \
    packagegroup-core-ssh-openssh \
    ${CORE_IMAGE_EXTRA_INSTALL} \
"

# XENSIV BGT60TRxx packages
IMAGE_INSTALL += "\
    xensiv-bgt60trxx \
    xensiv-bgt60trxx-examples \
    xensiv-bgt60trxx-dev \
"

# Development and testing tools
IMAGE_INSTALL += "\
    spitools \
    i2c-tools \
    libgpiod-tools \
    strace \
    gdb \
    gdbserver \
    valgrind \
    htop \
    nano \
    vim \
    wget \
    curl \
    git \
"

# Kernel modules for SPI and GPIO
IMAGE_INSTALL += "\
    kernel-module-spi-dev \
    kernel-module-gpio-mockup \
"

# Set root password for testing (insecure - for development only)
EXTRA_USERS_PARAMS = "usermod -P root root;"

# Enable systemd
DISTRO_FEATURES:append = " systemd"
VIRTUAL-RUNTIME_init_manager = "systemd"

# Image size
IMAGE_ROOTFS_EXTRA_SPACE = "1048576"