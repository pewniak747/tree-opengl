LIBS=-framework OpenGL -framework GLUT -lGLEW
CC=g++ -I/opt/local/include -L/opt/local/lib

SOURCES=tree.cpp tga.cpp detailed_cube.cpp
HEADERS=tga.h cube.h detailed_cube.h
OBJECTS=$(SOURCES:.cpp=.o)

all: tree

tree: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o tree
