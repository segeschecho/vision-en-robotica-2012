CC=g++

DIR_LIBRARY=../library/

DIR_LIBELAS=$(DIR_LIBRARY)libelas/src/
DIR_LIBCAM=$(DIR_LIBRARY)libcam/

SOURCES=obstacle_avoidance.cpp GlobalConfig.cpp VideoFileFrameGenerator.cpp CameraFrameGenerator.cpp $(DIR_LIBCAM)libcam.cpp $(DIR_LIBELAS)elas.cpp $(DIR_LIBELAS)descriptor.cpp $(DIR_LIBELAS)filter.cpp $(DIR_LIBELAS)matrix.cpp $(DIR_LIBELAS)triangle.cpp

EXECUTABLE=obstacle_avoidance

CFLAGS := -DNDEBUG -O3 -march=native

# OpenCV
CFLAGS += `pkg-config --cflags --libs opencv`

#LIBELAS
LIBS := -L ../library/libexabot-remote/build -lexabot-remote

all: obstacle_avoidance

obstacle_avoidance:
	$(CC) $(SOURCES) -o $(EXECUTABLE) $(CFLAGS) $(LIBS)

clean:
	rm -rf obstacle_avoidance
