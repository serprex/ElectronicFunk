CC=gcc -std=gnu99 -O3 -s
O=tgen.o ef.c
all: ${O}
	${CC} ${O} -o ElectronicFunk -fwhole-program -lglfw -lGL
spr: spr/*
	pinch font G _ r +2