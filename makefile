.PHONY: all clean

all: tvm.x

tvm.x: main.o tvm.o 
	g++ main.o tvm.o -o tvm.x

main.o: main.cpp
	g++ -c main.cpp

tvm.o: tvm.cpp
	g++ -c tvm.cpp

clean:
	rm -rf *.o tvm.x



