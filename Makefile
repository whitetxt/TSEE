filename = build/TSEE.exe

CC = gcc
CFLAGS = -g -Wall -Wextra -pedantic -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lws2_32

files = ${wildcard src/*.c}
objFiles = ${files:.c=.o}

all:
	${CC} -o ${filename} ${files} ${CFLAGS}

clean:
	-rm -rf ${filename}
	-rm -rf ${objFiles}

new:
	-rm -rf ${filename}
	-rm -rf ${objFiles}
	${CC} -o ${filename} ${files} ${CFLAGS}

run:
	${CC} -o ${filename} ${files} ${CFLAGS}
	cd build && ../${filename}

start:
	cd build && ../${filename}

gdb:
	cd build && gdb ../${filename}