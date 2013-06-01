LIBS=-framework OpenGL -framework GLUT -lGLEW
CC=g++ -I/opt/local/include -L/opt/local/lib -g

SOURCES=tree.cpp tga.cpp detailed_cube.cpp tree2.cpp branch.cpp
HEADERS=tga.h cube.h detailed_cube.h
OBJECTS=$(SOURCES:.cpp=.o)

all: tree

tree: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o tree
