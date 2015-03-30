Build Qt5 RPi
=============


$ mkdir <workdir> && cd <workdir>
$ git clone https://gitorious.org/bakeqtpi/bakeqtpi.git
$ git clone https://gitorious.org/cross-compile-tools/cross-compile-tools.git
$ git clone https://github.com/raspberrypi/tools.git

$ mkdir images
$ <download raspian image to images/ folder>

$ mkdir /mnt/rasp-pi-rootfs
# example, may vary
$ sudo mount -o loop,offset=62914560 2014-12-24-wheezy-raspbian.img /mnt/rasp-pi-rootfs
    
$ git clone git://gitorious.org/qt/qt5.git

# TODO: details

== Build Results

Loopback mounted image is ready to be unmounted and written to SD memory.

It is good to make backup of ready mady binaries by coying them to 
`gberry-console/qt5rpi`

    $ cp -r /mnt/rasp-pi-rootfs/usr/local/qt5pi <workspace>/gberry-console/qt5rpi/deploy/usr/local/
    $ cp -r /usr/local/qt5pi  <workspace>/gberry-console/qt5rpi/desktop/usr/local/
    