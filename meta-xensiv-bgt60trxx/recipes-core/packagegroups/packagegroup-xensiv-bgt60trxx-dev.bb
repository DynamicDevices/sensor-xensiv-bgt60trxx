SUMMARY = "Packagegroup for XENSIV BGT60TRxx development tools"
DESCRIPTION = "A packagegroup that installs all necessary tools and libraries for XENSIV BGT60TRxx radar sensor development"

LICENSE = "MIT"

inherit packagegroup

RDEPENDS:${PN} = "\
    xensiv-bgt60trxx \
    xensiv-bgt60trxx-dev \
    xensiv-bgt60trxx-examples \
    spitools \
    i2c-tools \
    libgpiod-tools \
    cmake \
    gcc \
    g++ \
    make \
    pkgconfig \
    git \
    strace \
    gdb \
    gdbserver \
    valgrind \
"