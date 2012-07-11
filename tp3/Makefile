CC=g++

DIRS=./libelas/src/
SOURCES=compute_disparity.cpp $(DIRS)elas.cpp $(DIRS)descriptor.cpp $(DIRS)filter.cpp $(DIRS)matrix.cpp $(DIRS)triangle.cpp

EXECUTABLE=compute_disparity

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

all: compute_disparity

compute_disparity:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)

clean:
	rm -rf compute_disparity