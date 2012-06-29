# CXXFLAGS := -g -O3 -fPIC -Wall `pkg-config --cflags opencv` -Ilibelas/src -msse2
# LDFLAGS := `pkg-config --libs opencv` -lhighgui -L./libelas/build/ -lelas

# all: libelas/build/libelas.a main

# libelas/build/libelas.a:
# 	cd libelas; mkdir build; cmake ..; make

# clean:
# 	rm -rf *.o main

# main: main.o
# 	g++ -o $@ main.o $(LDFLAGS)


CC=g++

SOURCES=read_values.cpp

EXECUTABLE=read_values

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ./libelas/build/ -lelas

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LIBS)

