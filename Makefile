LIBS=-framework OpenGL -framework GLUT -lGLEW
CC=g++ -I/opt/local/include -L/opt/local/lib -g

SOURCES=main.cpp tree.cpp branch.cpp tga.cpp clock.cpp leaf.cpp spherical_coordinates.cpp
HEADERS=tree.h branch.h tga.h clock.h leaf.h spherical_coordinates.h
OBJECTS=$(SOURCES:.cpp=.o)

all: tree

tree: $(OBJECTS) 
	$(CC) -o $@ $(addprefix bin/, $(OBJECTS)) $(LIBS)

$(OBJECTS): %.o: src/%.cpp $(addprefix src/, $(HEADERS))
	mkdir -p bin/
	$(CC) -c $< -o bin/$@
	
clean:
	rm -rf bin/ tree
