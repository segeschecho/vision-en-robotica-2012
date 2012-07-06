CC=g++

SOURCES=obstacle_avoidance.cpp GlobalConfig.cpp

EXECUTABLE=obstacle_avoidance

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ../library/libelas/build/ -lelas

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)  $(LIBS)