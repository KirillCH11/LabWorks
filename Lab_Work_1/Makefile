all: bmp_processor

bmp_processor: main.cpp Image.cpp Image.h
	g++ -fopenmp -O3 -march=native -std=c++11 -o bmp_processor main.cpp Image.cpp

clean:
	rm -f bmp_processor
