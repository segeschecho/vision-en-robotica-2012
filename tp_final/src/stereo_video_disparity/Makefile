CC=g++

SOURCES=stereo_video_disparity.cpp GlobalConfig.cpp

EXECUTABLE=stereo_video_disparity

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ../library/libelas/build/ -lelas

all:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS)  $(LIBS)
