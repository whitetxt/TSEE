filename = build/TSEE

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -lz -lm -rdynamic `pkg-config --cflags --libs sdl2 SDL2_image SDL2_ttf`

files = ${wildcard src/*.c src/tsee/*/*.c}
objFiles = ${files:.c=.o}

all: check_folder
	${CC} -o ${filename} ${files} ${CFLAGS}

clean: check_folder
	-rm -rf ${filename}
	-rm -rf ${objFiles}
	
new: check_folder clean all

run: check_folder
	chmod +x ${filename}
	cd build && ../${filename}

start: check_folder all run

gdb: check_folder
	cd build && gdb ../${filename}

vg: check_folder
	cd build && valgrind --leak-check=full --show-possibly-lost=no --show-reachable=no -s ../${filename}

check_folder:
	mkdir -p build