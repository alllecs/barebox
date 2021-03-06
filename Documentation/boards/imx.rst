Freescale i.MX
==============

Freescale i.MX is traditionally very well supported under barebox.
Depending on the SoC, there are different Boot Modes supported. Older
SoCs up to i.MX31 support only the external Boot Mode. Newer SoCs
can be configured for internal or external Boot Mode with the internal
boot mode being the more popular mode. The i.MX23 and i.MX28, also
known as i.MXs, are special. These SoCs have a completely different
boot mechanism.

Internal Boot Mode
------------------

The Internal Boot Mode is supported on:

* i.MX25
* i.MX35
* i.MX51
* i.MX53
* i.MX6

With the Internal Boot Mode, the images contain a header which describes
where the binary shall be loaded and started. These headers also contain
a so-called DCD table which consists of register/value pairs. These are
executed by the Boot ROM and are used to configure the SDRAM. In barebox,
the i.MX images are generated with the ``scripts/imx/imx-image`` tool.
Normally it's not necessary to call this tool manually, it is executed
automatically at the end of the build process.

The images generated by the build process can be directly written to an
SD card::

  # with Multi Image support:
  cat images/barebox-freescale-imx51-babbage.img > /dev/sdd
  # otherwise:
  cat barebox-flash-image > /dev/sdd

The above will overwrite the MBR (and consequently the partition table)
on the destination SD card. To preserve the MBR while writing the rest
of the image to the card, use::

  dd if=images/barebox-freescale-imx51-babbage.img of=/dev/sdd bs=512 skip=1 seek=1

The images can also always be started second stage::

  bootm /mnt/tftp/barebox-freescale-imx51-babbage.img

Internal Boot Mode Through Internal RAM(IRAM)
---------------------------------------

The Internal Boot Mode Through Internal RAM is supported on:

* i.MX51

As can be easily deduced from its name, the Internal Boot Mode Through
Internal RAM is just a variant of Internal Boot Mode so all of the
stated above still applies in this case. What it differs in is the following:

* Boot process is done in two stages(First stage binary can be
  produced with ``imx_v7-xload_defconfig``)
* DCD of the first stage image is set such that the image is fetched
  into an unoccupied area or IRAM
* First stage image once uncompressed and set up will look for a
  second stage bootloader on the same media it booted from and start
  it(see mach-imx/xload.c for more details)
* Second stage images are just regular i.MX boot images

Since on a typical i.MX SoC unused IRAM area is not enough to run
anything but a PBL this mode, due to its very limited usability,
serves only one purpose -- allow for a portion of a bootloader to be
executed without depending on DRAM to be functional. This peculiarity
of the mode can be used to implement various memory testing
scenarious.

USB Boot
^^^^^^^^

Most boards can be explicitly configured for USB Boot Mode or fall back
to USB Boot when no other medium can be found. The barebox repository
contains a USB upload tool. As it depends on the libusb development headers,
it is not built by default. Enable it explicitly in ``make menuconfig``
and install the libusb development package. On Debian, this can be done
with ``apt-get install libusb-dev``. After compilation, the tool can be used
with only the image name as argument::

  scripts/imx/imx-usb-loader images/barebox-freescale-imx51-babbage.img

External Boot Mode
------------------

The External Boot Mode is supported by the older i.MX SoCs:

* i.MX1
* i.MX21
* i.MX27
* i.MX31

(It may be supported on newer SoCs as well, but it is not widely used there.)

The External Boot Mode supports booting only from NOR and NAND flash. On NOR
flash, the binary is started directly on its physical address in memory. Booting
from NAND flash is more complicated. The NAND flash controller copies the first
2kb of the image to the NAND Controller's internal SRAM. This initial binary
portion then has to:

* Set up the SDRAM
* Copy the initial binary to SDRAM to make the internal SRAM in the NAND flash
  controller free for use for the controller
* Copy the whole barebox image to SDRAM
* Start the image

It is possible to write the image directly to NAND. However, since NAND flash
can have bad blocks which must be skipped during writing the image and also
by the initial loader, it is recommended to use the :ref:`command_barebox_update`
command for writing to NAND flash.

i.MX boards
-----------

Not all supported boards have a description here. Many newer boards also do
not have individual defconfig files, they are covered by ``imx_v7_defconfig``
or ``imx_defconfig`` instead.

.. toctree::
  :glob:
  :numbered:
  :maxdepth: 1

  imx/*
