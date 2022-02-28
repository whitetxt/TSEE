filename = build/TSEE

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -lz -lm `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf libfyaml`

files = ${wildcard src/tsee*.c} src/main.c ${wildcard src/ext/*.c}
objFiles = ${files:.c=.o}

editor_files = ${wildcard src/tsee*.c} src/editor.c ${wildcard src/ext/*.c}
editor_fn = build/TSEE_Editor

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
	cd build && valgrind --leak-check=full --show-possibly-lost=no --show-reachable=no -s ../${filename}

editor:
	${CC} -o ${editor_fn} ${editor_files} ${CFLAGS}

estart: editor
	chmod +x ${editor_fn}
	cd build && ../${editor_fn}