#
#*Doinakis Michail
#*e-mail: doinakis@ece.auth.gr
#
# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects
all:	main

main:	main.o queue.o timer.o prod-cons.o
			gcc -pthread queue.o timer.o prod-cons.o main.o -o main -lm
main.o: main.c
			gcc -c main.c
queue.o: queue.c
			gcc -c queue.c
timer.o: timer.c
			gcc -c timer.c
prod-cons.o: prod-cons.c
			gcc -c prod-cons.c
clean:
		$(RM)	*.o main
