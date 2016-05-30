CC=g++
CFLAGS=-c -Wall
LDFLAGS=
SOURCES=main.cpp occurrence.cpp nodeOfInterest.cpp lineage.cpp parser.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=snape

all: $(SOURCES) $(EXECUTABLE)
	
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@


