
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Linux)
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


CC = $(DEVKITARM)/bin/arm-none-eabi-gcc
CXX = $(DEVKITARM)/bin/arm-none-eabi-g++
OBJCOPY = $(DEVKITARM)/bin/arm-none-eabi-objcopy
GBAFIX = $(DEVKITPRO)/tools/bin/gbafix


BASENAME = gba_simple

PROGS = $(BASENAME).gba
BINARY = $(BASENAME).elf
SRCS = $(BASENAME).c
OBJS = $(SRCS:.c=.o)

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

LDFLAGS = -L$(DEVKITARM)/arm-none-eabi/lib -specs=gba.specs
LDFLAGS += -marm -mthumb-interwork
#LDFLAGS += -mthumb-interwork
#LDFLAGS += -mthumb


ALL: $(PROGS)

.c.o:
	rm -f $*.o
	$(CC) -c $(CFLAGS) -o $*.o $*.c

.cpp.o:
	rm -f $*.o
	$(CXX) -c $(CFLAGS) -o $*.o $*.cpp

$(PROGS): $(OBJS)
	rm -f $@
	$(CC) -o $(BINARY) $(OBJS)  $(LDFLAGS) $(LIBS)
	$(OBJCOPY) -v -O binary $(BINARY) $@
	$(GBAFIX) $(PROGS)

clean:
	rm -f $(OBJS)
	rm -f $(PROGS)
	rm -f $(BINARY)
