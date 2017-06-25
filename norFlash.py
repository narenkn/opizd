import spi

spi.openSPI(speed=2000000)
val = spi.transfer((0x9F, 0x0, 0x0, 0x0))
print "%x %x %x %x" % (val[0], val[1], val[2], val[3]);

val = spi.transfer((0x90, 0x0, 0x0, 0x0, 0x0, 0x0))
print "%x %x" % (val[4], val[5]);
