filename = build/TSEE

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -lz -lm -rdynamic `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf libfyaml`

files = ${wildcard src/*.c src/tsee/*/*.c}
objFiles = ${files:.c=.o}

all:
	${CC} -o ${filename} ${files} ${CFLAGS}

clean:
	-rm -rf ${filename}
	-rm -rf ${objFiles}

new: clean all

run: 
	chmod +x ${filename}
	cd build && ../${filename}

start: all run

gdb:
	cd build && gdb ../${filename}

vg:
	cd build && valgrind --leak-check=full --show-possibly-lost=no --show-reachable=no -s ../${filename}