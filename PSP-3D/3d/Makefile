
TARGET=3D
OBJS=main.o ../common/callback.o ../common/graphics.o

INCDIR=
CFLAGS=-lpsplibc -lstdc++ -G0 -Wall -O3 
CXXFLAGS=$(CFLAGS) -std=c++17 -fno-exceptions -fno-rtti -lpthread
ASFLAGS=$(CFLAGS)

LIBDIR=
LDFLAGS=
LIBS=-lm -lpsplibc -lstdc++

BUILD_PRX=1 

EXTRA_TARGETS=EBOOT.PBP
PSP_EBOOT_TITLE=3D

PSPSDK=$(shell psp-config --pspsdk-path)
include $(PSPSDK)/lib/build.mak
