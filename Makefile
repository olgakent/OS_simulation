#############################################
##
## Makefile
## LINUX compilation
##
##############################################



CC = g++
LFLAGS = -std=c++11
CFLAGS = -c -Wall

SOURCES = Driver.cpp Computer.cpp
OBJECTS = $(SOURCES:.cpp=.o)
EXECUTABLE_NAME = osSimulation

all: $(EXECUTABLE_NAME)

$(EXECUTABLE_NAME): $(OBJECTS)
	$(CC) -o $(EXECUTABLE_NAME) $(OBJECTS)

.cpp.o:
	$(CC) $(CFLAGS) $(LFLAGS) $< -o $@

clean:
	$(RM) *.o *~ $(EXECUTABLE_NAME)
