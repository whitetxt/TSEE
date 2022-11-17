out = build/TSEE

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -fstack-protector -fno-common -rdynamic -lz -lm ${pkg-config --cflags sdl2 SDL2_image SDL2_ttf}
LD = $(CC)

LDFLAGS = ${pkg-config --libs sdl2 SDL2_image SDL2_ttf}

files = ${wildcard src/*.c src/tsee/*/*.c}
obj = ${files:.c=.o}

ifeq ($(DEV),1)
CFLAGS += -DTSEE_DEV -g
else
CFLAGS += -O3
endif

all: check_folder $(out)

$(out): $(obj)
	$(LD) $(LDFLAGS) -o $@ $(obj) $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

clean: check_folder
	-rm -rf ${out}
	-rm -rf ${obj}
	-rm -rf ${src:.c=.d}
	
fresh: clean all

run:
	chmod +x ${out}
	cd build && ../${out}

start: all run

gdb:
	cd build && gdb ../${out}

vg:
	cd build && valgrind --leak-check=full --show-possibly-lost=no --show-reachable=no -s ../${out}

check_folder:
	mkdir -p build

docs:
	doxygen src/tsee.dxg

-include $(files:.c=.d)