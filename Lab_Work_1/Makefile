CXX := g++
CXXFLAGS := -fopenmp -O3 -march=native -std=c++11
LDFLAGS := -fopenmp
TARGET := bmp_processor

SRCS := main.cpp Image.cpp
HDRS := Image.h
OBJS := $(SRCS:.cpp=.o)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean test

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) *.bmp

test: $(TARGET)
	./$(TARGET) Panda_small.bmp small_rotated.bmp small_filtered.bmp
	./$(TARGET) Panda_middle.bmp middle_rotated.bmp middle_filtered.bmp
	./$(TARGET) Panda_large.bmp large_rotated.bmp large_filtered.bmp
