CC=gcc

CFLAG=-c -Wall

console:Console.o
	$(CC) Console.o -o console

controler:Controler.o
	$(CC) Controler.o -o controler

Console.o: Console/Console.c Console/Console.h 
	$(CC) $(CFLAG) Console/Console.c		# target   dependency command 

Controler.o: Controler/Controler.c Controler/Controler.h 
	$(CC) $(CFLAG) Controler/Controler.c

.PHONY : BuildConsole BuildControler RunConsole RunControler clean

BuildConsole:
	make console

BuildControler:
	make controler

RunConsole:
	./console

RunControler:
	./controler

clean:
	rm -rf *o output	# remove object file and exe file
