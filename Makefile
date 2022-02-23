filename = build/TSEE

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -lz -lm `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf libfyaml`

files = ${wildcard src/*.c}
objFiles = ${files:.c=.o}

all:
	${CC} -o ${filename} ${files} ${CFLAGS}

clean:
	-rm -rf ${filename}
	-rm -rf ${objFiles}

new: clean all

start: all
	chmod +x ${filename}
	cd build && ../${filename}

gdb:
	cd build && gdb ../${filename}

vg:
	cd build && valgrind --leak-check=full --track-origins=yes -s ../${filename} 