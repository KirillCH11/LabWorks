CXX := g++
CXXFLAGS := -fopenmp -O3 -march=native -std=c++11
LDFLAGS := -fopenmp
TARGET := bmp_processor

SRCS := main.cpp Image.cpp
HDRS := Image.h
OBJS := $(SRCS:.cpp=.o)

GENERATED_BMP := rotated_*.bmp filtered_*.bmp small_*.bmp middle_*.bmp large_*.bmp

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp $(HDRS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

.PHONY: all clean test

all: $(TARGET)

clean:
	rm -f $(TARGET) $(OBJS) $(GENERATED_BMP)

test: $(TARGET)
	./$(TARGET) Pan.bmp small_rotated.bmp small_filtered.bmp
	./$(TARGET) Pan_2.bmp middle_rotated.bmp middle_filtered.bmp
