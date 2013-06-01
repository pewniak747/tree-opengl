#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tga.h"
#include "cube.h"
#include "tree.h"
#include "branch.h"

float speed_x=0;
float speed_y=100;
int lastTime=0;
float angle_x;
float angle_y;

Tree *tree = new Tree();

void displayFrame(void) {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
	glm::mat4 V=glm::lookAt(
		glm::vec3(0.0f,0.0f,-10.0f),
		glm::vec3(0.0f,0.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f));
	
	glm::mat4 P=glm::perspective(100.0f, 1.0f, 1.0f, 50.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	
  Branch *branch;

  for(int i = 0; i < tree->branchCount(); i++) {
    branch = tree->getBranch(i);
    glm::mat4 M=glm::mat4(1.0f);
    M=glm::rotate(M,angle_y,glm::vec3(0.0f,1.0f,0.0f));
    std::vector<int> parents = branch->parents();
    for(int l = parents.size()-1; l >= 0; l--) {
      M=glm::rotate(M, tree->getBranch(parents[l])->direction, glm::vec3(0.0f, 1.0f, 0.0f));
      M=glm::rotate(M, tree->getBranch(parents[l])->angle, glm::vec3(0.0f, 0.0f, 1.0f));
      M=glm::translate(M, glm::vec3(0.0f, tree->getBranch(parents[l])->length(), 0.0f));
    }
    M=glm::rotate(M, branch->direction, glm::vec3(0.0f, 1.0f, 0.0f));
    M=glm::rotate(M, branch->angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glLoadMatrixf(glm::value_ptr(V*M));
    float lineVertices[] = {
      0, 0, 0,
      0, branch->length(), 0
    };

    glEnableClientState(GL_VERTEX_ARRAY);
    //glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,lineVertices);
    //glColorPointer(3,GL_FLOAT,0,cubeColors);	

    glDrawArrays(GL_LINES,0,2);

    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);	
  }
		
	glutSwapBuffers();
}

void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;
	angle_x+=speed_x*interval/1000.0;
	angle_y+=speed_y*interval/1000.0;
	if (angle_x>360) angle_x-=360;
	if (angle_x>360) angle_x+=360;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;
  tree->clock += 0.001;

  if(rand() % 200 == 0) {
    tree->addBranch();
  }
	
	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=60;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-60;
      break;
    case GLUT_KEY_UP: 
      speed_x=60;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-60;
      break;  
  }
}

void keyUp(int c, int x, int y) {
  switch (c) {
    case GLUT_KEY_LEFT: 
      speed_y=0;
      break;
    case GLUT_KEY_RIGHT:
      speed_y=-0;
      break;
    case GLUT_KEY_UP: 
      speed_x=0;
      break;
    case GLUT_KEY_DOWN:
      speed_x=-0;
      break;  
  }
}


int main(int argc, char* argv[]) {
  srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Program OpenGL");        
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

  glutMainLoop();
  return 0;
}
