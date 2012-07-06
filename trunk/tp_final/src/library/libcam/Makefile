CC=g++

SOURCES=main.cpp timer.cpp libcam.cpp

EXECUTABLE=stereocam

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBCAM
# LIBS := # -L . -lcam

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)

