CC=g++

DIRS=../library/libelas/src/

SOURCES=stereo_video_disparity.cpp GlobalConfig.cpp $(DIRS)elas.cpp $(DIRS)descriptor.cpp $(DIRS)filter.cpp $(DIRS)matrix.cpp $(DIRS)triangle.cpp

EXECUTABLE=stereo_video_disparity

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

all: stereo_video_disparity

stereo_video_disparity:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LIBS)

clean:
	rm -rf stereo_video_disparity