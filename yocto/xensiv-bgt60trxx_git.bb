SUMMARY = "XENSIV BGT60TRxx 60 GHz FMCW Radar Sensor Library"
DESCRIPTION = "Cross-platform library for interfacing with XENSIV BGT60TRxx radar sensors"
HOMEPAGE = "https://github.com/DynamicDevices/sensor-xensiv-bgt60trxx"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=..."

SRCREV = "${AUTOREV}"
PV = "1.0.0+git${SRCPV}"

SRC_URI = "git://github.com/DynamicDevices/sensor-xensiv-bgt60trxx.git;protocol=https;branch=main"

S = "${WORKDIR}/git"

inherit cmake pkgconfig

DEPENDS = ""
RDEPENDS:${PN} = ""

# Build configuration
EXTRA_OECMAKE = " \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_EXAMPLES=ON \
    -DBUILD_TESTS=OFF \
    -DENABLE_LINUX_SUPPORT=ON \
    -DENABLE_MTB_SUPPORT=OFF \
"

# Package configuration
PACKAGES = "${PN} ${PN}-dev ${PN}-examples"

FILES:${PN} = " \
    ${libdir}/lib*.so.* \
"

FILES:${PN}-dev = " \
    ${includedir}/* \
    ${libdir}/lib*.so \
    ${libdir}/lib*.a \
    ${libdir}/pkgconfig/* \
    ${libdir}/cmake/* \
"

FILES:${PN}-examples = " \
    ${bindir}/* \
"

BBCLASSEXTEND = "native nativesdk"