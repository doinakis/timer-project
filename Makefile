#
#*Doinakis Michail
#*e-mail: doinakis@ece.auth.gr
#
# Usage:
# make        # compile all binary
# make clean  # remove ALL binaries and objects
all:	main

main:	main.o queue.o timer.o prod-cons.o globaldef.o
			arm-linux-gnueabihf-gcc -o3 -pthread queue.o timer.o prod-cons.o globaldef.o  main.o -o main -lm
main.o: main.c
			arm-linux-gnueabihf-gcc -c  main.c
queue.o: queue.c
			arm-linux-gnueabihf-gcc -c  queue.c
timer.o: timer.c
			arm-linux-gnueabihf-gcc -c  timer.c
prod-cons.o: prod-cons.c
			arm-linux-gnueabihf-gcc -c  prod-cons.c
globaldef.o: globaldef.c
			arm-linux-gnueabihf-gcc -c globaldef.c
clean:
		$(RM)	*.o main *.csv *.txt *.data
