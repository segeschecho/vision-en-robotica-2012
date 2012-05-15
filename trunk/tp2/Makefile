
CC=g++

SOURCES=surf.cpp
OBJECTS=$(SOURCES:.cpp=.o)

EXECUTABLE=surf

CFLAGS := -Wall -DNDEBUG -O3 -march=native
LDFLAGS := `pkg-config opencv --libs`

# OpenCV
CFLAGS += `pkg-config opencv --cflags`

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) -g -c $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE) 
