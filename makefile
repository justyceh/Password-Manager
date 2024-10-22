password: main.o database.o
	g++ -o password main.o database.o -lssl -lcrypto
main.o:
	g++ -c main.cpp
database.o: database.h database.cpp
	g++ -c database.cpp 
make clean:
	rm *.o password