LIBS=-framework OpenGL -framework GLUT -lGLEW
CC=g++ -I/opt/local/include -L/opt/local/lib -g

SOURCES=main.cpp tree.cpp branch.cpp tga.cpp
HEADERS=tree.h branch.h tga.h
OBJECTS=$(SOURCES:.cpp=.o)

all: tree

tree: $(OBJECTS) 
	$(CC) -o $@ $(OBJECTS) $(LIBS)

$(OBJECTS): %.o: %.cpp $(HEADERS)
	$(CC) -c $< -o $@
	
clean:
	-rm -rf *.o tree
