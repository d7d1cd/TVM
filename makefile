.PHONY: all clean

all: tvm.x

tvm.x: main.o tvm.o core.o
	g++ main.o tvm.o core.o -o tvm.x

main.o: main.cpp
	g++ -c main.cpp

tvm.o: tvm.cpp
	g++ -c tvm.cpp

core.o: core.cpp
	g++ -c core.cpp

clean:
	rm -rf *.o tvm.x



