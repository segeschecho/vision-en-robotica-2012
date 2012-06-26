
CC=g++

SOURCES=read_values.cpp

EXECUTABLE=read_values

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)

