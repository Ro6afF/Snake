FLAGS = -O2 -std=gnu++1z
SDL = -I/usr/local/include -lSDL2

all: clean build run
clean:
	rm *.exe

build:
	clang++ main.cpp -o main.exe $(FLAGS) $(SDL)

run:
	./main.exe

