CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=$(wildcard source/*.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=snape

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


