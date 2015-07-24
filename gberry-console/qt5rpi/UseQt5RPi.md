Use Qt5 RPi
===========


== Building Applications

The Qt binaries are in two places
 * ARM binaries /mnt/rasp-pi-rootfs/usr/local/qt5pi
 * x64 tool binaries /usr/local/qt5pi
 
qt5rpi folder contains:
 * deploy/ for above ARM binaries
 * desktop/ for above x64 binaries
 
To setup a compilation environment you need raspian image:

    $ mkdir /mnt/rasp-pi-rootfs
    $ sudo mount -o loop,offset=62914560 2014-12-24-wheezy-raspbian.img /mnt/rasp-pi-rootfs
    $ cp -r deploy/* /mnt/rasp-pi-rootfs
    $ cp -r desktop/* /
    
You need toolchain (compiler) installed. The toolchain was located when qt5pi was built:
 * Folder: /home/tevuore/workspace/qt-rpi/raspberrypi-tools/
 * Compiler: arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++


                                         
 