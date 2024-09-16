
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
	DEVKITARM=/opt/devkitPro/devkitARM
	DEVKITPRO=/opt/devkitPro
endif
ifeq ($(UNAME_S),Darwin)
	DEVKITARM=/opt/devkitPro/devkitARM
	DEVKITPRO=/opt/devkitPro
endif
ifeq ($(findstring CYGWIN,$(UNAME_S)),CYGWIN)
endif
ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
	DEVKITARM=/i/devkitPro/devkitARM
	DEVKITPRO=/i/devkitPro
endif
ifeq ($(findstring MSYS,$(UNAME_S)),MSYS)
	DEVKITARM=/i/devkitPro/devkitARM
	DEVKITPRO=/i/devkitPro
endif


AS = $(DEVKITARM)/bin/arm-none-eabi-as
CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CXX = $(DEVKITARM)/bin/arm-none-eabi-g++
OBJCOPY = $(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX = $(DEVKITPRO)/tools/bin/gbafix


BASENAME = gba_self

PROGS = $(BASENAME).gba
BINARY = $(BASENAME).elf
SRCS1 = $(BASENAME).c
SRCS2 = vlt_gba_crt0.s
OBJS1 = $(SRCS1:.c=.o)
OBJS2 = $(SRCS2:.s=.o)

CFLAGS = -Wall
CFLAGS += -marm -mthumb-interwork
#CFLAGS += -mthumb -mthumb-interwork
CFLAGS += -std=c99
#CFLAGS += -specs=gba_mb.specs
CFLAGS += -fno-strict-aliasing
CFLAGS += -mcpu=arm7tdmi
CFLAGS += -mtune=arm7tdmi
CFLAGS += -fomit-frame-pointer
CFLAGS += -ffast-math
CFLAGS += -fpermissive
CFLAGS += -O0 -g 
#CFLAGS += -O2
#CFLAGS += -MMD -MP -MF tiny3d.d 

#CFLAGS = -Wall -specs=gba_mb.specs

#LDFLAGS = -L$(DEVKITARM)/arm-none-eabi/lib -specs=gba.specs
LDFLAGS = -specs=vlt_gba.specs
LDFLAGS += -marm -mthumb-interwork
#LDFLAGS += -T vlt_gba_cart.ld
#LDFLAGS += -mthumb-interwork
#LDFLAGS += -mthumb


ALL: $(PROGS)

.c.o:
	rm -f $*.o
	$(CC) -c $(CFLAGS) -o $*.o $*.c

.cpp.o:
	rm -f $*.o
	$(CXX) -c $(CFLAGS) -o $*.o $*.cpp

.s.o:
	rm -f $*.o
	$(AS) -mthumb-interwork -o $*.o $*.s

$(PROGS): $(OBJS1) $(OBJS2)
	rm -f $@
	$(CC) -o $(BINARY) $(OBJS1) $(LDFLAGS) $(LIBS)
	$(OBJCOPY) -v -O binary $(BINARY) $@
	$(GBAFIX) $(PROGS)

clean:
	rm -f $(OBJS) *.o
	rm -f $(PROGS)
	rm -f $(BINARY)
	rm -f *.sav
