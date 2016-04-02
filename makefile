objects = main.o ILibParsers.o

edit : $(objects)
	cc -o edit $(objects)

main.o : ILibParsers.h istar.h

ILibParsers.o : ILibParsers.h

clean :
	rm edit $(objects)
	