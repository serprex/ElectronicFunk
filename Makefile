CC=gcc -std=gnu99 -Ofast -g -pipe
O=tgen.o bgen.o spr.o o.o ef.o
all: ${O}
	${CC} ${O} -o ElectronicFunk -fwhole-program -lglfw -lGL -lm
clean:
	rm *.o
spr: spr/*
	pinch S R
	bitch map