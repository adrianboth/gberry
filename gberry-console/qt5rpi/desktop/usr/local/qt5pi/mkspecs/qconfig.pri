#configuration
CONFIG +=  cross_compile shared qpa no_mocdepend release qt_no_framework
host_build {
    QT_ARCH = x86_64
    QT_TARGET_ARCH = arm
} else {
    QT_ARCH = arm
    QMAKE_DEFAULT_LIBDIRS = /mnt/rasp-pi-rootfs/usr/lib /mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc/arm-linux-gnueabihf/4.8.3 /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc/arm-linux-gnueabihf /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/lib /mnt/rasp-pi-rootfs/lib/arm-linux-gnueabihf /mnt/rasp-pi-rootfs/lib
    QMAKE_DEFAULT_INCDIRS = /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/include/c++/4.8.3 /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/include/c++/4.8.3/arm-linux-gnueabihf /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/include/c++/4.8.3/backward /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc/arm-linux-gnueabihf/4.8.3/include /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/lib/gcc/arm-linux-gnueabihf/4.8.3/include-fixed /home/tevuore/workspace/qt-rpi/raspberrypi-tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/arm-linux-gnueabihf/include /mnt/rasp-pi-rootfs/usr/include /mnt/rasp-pi-rootfs/usr/include/arm-linux-gnueabihf
}
QT_CONFIG +=  minimal-config small-config medium-config large-config full-config evdev linuxfb c++11 accessibility egl eglfs opengl opengles2 shared qpa reduce_exports clock-gettime clock-monotonic posix_fallocate mremap getaddrinfo ipv6ifname getifaddrs inotify eventfd png freetype harfbuzz system-zlib nis iconv rpath concurrent audio-backend release

#versioning
QT_VERSION = 5.4.1
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 4
QT_PATCH_VERSION = 1

#namespaces
QT_LIBINFIX = 
QT_NAMESPACE = 

# pkgconfig
PKG_CONFIG_SYSROOT_DIR = /mnt/rasp-pi-rootfs
PKG_CONFIG_LIBDIR = /mnt/rasp-pi-rootfs/usr/lib/pkgconfig:/mnt/rasp-pi-rootfs/usr/share/pkgconfig:/mnt/rasp-pi-rootfs/usr/lib/arm-linux-gnueabihf/pkgconfig

# sysroot
!host_build {
    QMAKE_CFLAGS    += --sysroot=$$[QT_SYSROOT]
    QMAKE_CXXFLAGS  += --sysroot=$$[QT_SYSROOT]
    QMAKE_LFLAGS    += --sysroot=$$[QT_SYSROOT]
}

QT_GCC_MAJOR_VERSION = 4
QT_GCC_MINOR_VERSION = 8
QT_GCC_PATCH_VERSION = 3
