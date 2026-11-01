# ============================================================
#  PS2 RSOD Demo — Makefile
#  Изисква: ps2sdk, ps2dev toolchain
# ============================================================

EE_BIN   = rsod.elf
EE_OBJS  = main.o

# PS2DEV SDK пътища (задай ги ако не са в PATH)
# PS2DEV ?= /usr/local/ps2dev
# PS2SDK ?= $(PS2DEV)/ps2sdk

EE_LIBS  = -ldraw -lgraph -lpacket -ldma -lkernel -lsifrpc -lc

# Включи стандартните ps2sdk правила
include $(PS2SDK)/samples/Makefile.pref
include $(PS2SDK)/samples/Makefile.eeglobal
