all: test-power

power.o: power.c
	gcc -c power.c

test-power.o: test-power.c
	gcc -c test-power.c

test-power: power.o test-power.o
	gcc power.o test-power.o -o test-power

clean:
	rm -f *.o test-power
