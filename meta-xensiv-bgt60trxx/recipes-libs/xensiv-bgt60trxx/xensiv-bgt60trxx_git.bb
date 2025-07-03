SUMMARY = "Cross-platform library for XENSIV BGT60TRxx 60 GHz FMCW radar sensors"
DESCRIPTION = "This library provides functions for interfacing with the XENSIV™ BGT60TRxx 60 GHz FMCW Radar Sensors with support for Linux, Yocto, and ModusToolbox platforms."
HOMEPAGE = "https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx"
SECTION = "libs"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=0835ade698e0bcf8506ecda2f7b4f302"

DEPENDS = ""
RDEPENDS:${PN} = ""

SRC_URI = "git://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git;protocol=https;branch=master"
SRCREV = "${AUTOREV}"

PV = "1.1.1+git${SRCPV}"
S = "${WORKDIR}/git"

inherit cmake pkgconfig

# CMake configuration
EXTRA_OECMAKE = "\
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_EXAMPLES=ON \
    -DBUILD_TESTS=OFF \
    -DENABLE_LINUX_SUPPORT=ON \
    -DENABLE_MTB_SUPPORT=OFF \
"

# Package the library, development files, and examples separately
PACKAGES = "${PN} ${PN}-dev ${PN}-examples"

FILES:${PN} = "\
    ${libdir}/libxensiv_bgt60trxx.so.* \
"

FILES:${PN}-dev = "\
    ${includedir}/xensiv_bgt60trxx.h \
    ${includedir}/xensiv_bgt60trxx_regs.h \
    ${includedir}/xensiv_bgt60trxx_platform.h \
    ${includedir}/xensiv_bgt60trxx_linux.h \
    ${libdir}/libxensiv_bgt60trxx.so \
    ${libdir}/pkgconfig/xensiv_bgt60trxx.pc \
    ${libdir}/cmake/xensiv_bgt60trxx/*.cmake \
"

FILES:${PN}-examples = "\
    ${bindir}/examples/basic_example \
    ${bindir}/examples/fifo_example \
    ${bindir}/examples/config_example \
"

# Allow empty main package if only dev/examples are needed
ALLOW_EMPTY:${PN} = "1"

# Extend for native and SDK builds
BBCLASSEXTEND = "native nativesdk"