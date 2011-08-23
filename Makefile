CC=gcc -std=gnu99 -O3 -s
O=tgen.o two.o
all: ${O}
	${CC} ${O} -o Two -fwhole-program -lglfw -lGL
spr:
	pinch font G _ r +2