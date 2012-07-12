CC=g++

DIRS=../library/libelas/src/

SOURCES=obstacle_avoidance.cpp GlobalConfig.cpp $(DIRS)elas.cpp $(DIRS)descriptor.cpp $(DIRS)filter.cpp $(DIRS)matrix.cpp $(DIRS)triangle.cpp

EXECUTABLE=obstacle_avoidance

CFLAGS := -Wall -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ../library/libexabot-remote/build -lexabot-remote

all: obstacle_avoidance

obstacle_avoidance:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LIBS)

clean:
	rm -rf obstacle_avoidance