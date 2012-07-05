CC=g++

SOURCES=compute_disparity.cpp

EXECUTABLE=compute_disparity

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ./libelas/build/ -lelas

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)  $(LIBS)

