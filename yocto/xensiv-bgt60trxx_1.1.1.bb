SUMMARY = "XENSIV BGT60TRxx 60 GHz FMCW Radar Sensor Library"
DESCRIPTION = "Cross-platform library for interfacing with XENSIV BGT60TRxx radar sensors"
HOMEPAGE = "https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=..."

SRCREV = "v${PV}"
PV = "1.1.1"

SRC_URI = "git://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git;protocol=https;tag=v${PV}"
S = "${WORKDIR}/git"

DEPENDS = ""
RDEPENDS:${PN} = ""

inherit cmake

EXTRA_OECMAKE = " \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_EXAMPLES=ON \
    -DBUILD_TESTS=OFF \
    -DENABLE_LINUX_SUPPORT=ON \
    -DENABLE_MTB_SUPPORT=OFF \
"

PACKAGES = "${PN} ${PN}-dev ${PN}-examples"

FILES:${PN} = " \
    ${libdir}/libxensiv_bgt60trxx.so.* \
"

FILES:${PN}-dev = " \
    ${includedir}/xensiv_bgt60trxx.h \
    ${includedir}/xensiv_bgt60trxx_regs.h \
    ${includedir}/xensiv_bgt60trxx_platform.h \
    ${includedir}/xensiv_bgt60trxx_linux.h \
    ${libdir}/libxensiv_bgt60trxx.so \
    ${libdir}/pkgconfig/xensiv_bgt60trxx.pc \
    ${libdir}/cmake/xensiv_bgt60trxx/ \
"

FILES:${PN}-examples = " \
    ${bindir}/basic_example \
    ${bindir}/fifo_example \
    ${bindir}/config_example \
"

BBCLASSEXTEND = "native nativesdk"