# Heartbeat

Heartbeat is a Linux kernel module that when loaded will regularly "phone home" and send its internal and external IP address to a trusted location. This means if your laptop is taken, and then connected to a network, you will know where it is.

Are there other solutions to this problem? Definitely. Are there better solutions? Probably. Is this one a fun excuse to play around with kernel modules? Absolutely.

## Usage

To build and run the module, simply type:

   make
   sudo insmod ./hb.ko

To uninstall the module, run:

   sudo rmmod hb

## License

This code is released under the GPLv2 (included), as some kernel code we run requires that this code be released under the same license.
