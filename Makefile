EE_BIN  = rsod.elf
EE_OBJS = main.o
EE_LIBS = -lkernel -lc

include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
