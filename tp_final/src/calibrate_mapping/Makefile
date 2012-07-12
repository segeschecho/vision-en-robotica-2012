CC=g++

DIRS=../library/libelas/src/

SOURCES=calibrate_mapping.cpp $(DIRS)elas.cpp $(DIRS)descriptor.cpp $(DIRS)filter.cpp $(DIRS)matrix.cpp $(DIRS)triangle.cpp

EXECUTABLE=calibrate_mapping

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

all: calibrate_mapping

calibrate_mapping:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)

clean:
	rm -rf calibrate_mapping
