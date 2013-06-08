#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <vector>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "tga.h"
#include "tree.h"
#include "branch.h"

float speed_y=20;
float cameraHeight = 4.0f;
int lastTime=0;
float angle_x;
float angle_y;

GLuint grassTexture;
TGAImg image;

Tree *tree = new Tree();

void loadTexture(char *filename, GLuint *handle) {
  printf("Loading texture %s\n", filename);
  if (image.Load(filename)==IMG_OK) {
    glGenTextures(1,handle);
    glBindTexture(GL_TEXTURE_2D,*handle);
    if (image.GetBPP()==24)
      glTexImage2D(GL_TEXTURE_2D,0,3,image.GetWidth(),image.GetHeight(),0, GL_RGB,GL_UNSIGNED_BYTE,image.GetImg());
    else if (image.GetBPP()==32)
      glTexImage2D(GL_TEXTURE_2D,0,4,image.GetWidth(),image.GetHeight(),0, GL_RGBA,GL_UNSIGNED_BYTE,image.GetImg());
    else
      printf("Error loading texture %s\n", filename);
  }
  else {
    printf("Error loading texture %s\n", filename);
  }
}

void drawBranch(Branch *branch, const glm::mat4 V) {
  glm::mat4 M=glm::mat4(1.0f);
  M=glm::rotate(M,angle_y,glm::vec3(0.0f,1.0f,0.0f));
  std::vector<int> parents = branch->parents();
  for(int l = parents.size()-1; l >= 0; l--) {
    M=glm::rotate(M, tree->getBranch(parents[l])->direction, glm::vec3(0.0f, 1.0f, 0.0f));
    M=glm::rotate(M, tree->getBranch(parents[l])->angle, glm::vec3(0.0f, 0.0f, 1.0f));
    if(l > 0)
      M=glm::translate(M, glm::vec3(0.0f, tree->getBranch(parents[l-1])->rootDistance(), 0.0f));
  }
  if(branch->parent >= 0)
    M=glm::translate(M, glm::vec3(0.0f, branch->rootDistance(), 0.0f));
  M=glm::rotate(M, branch->direction, glm::vec3(0.0f, 1.0f, 0.0f));
  M=glm::rotate(M, branch->angle, glm::vec3(0.0f, 0.0f, 1.0f));
  M=glm::rotate(M, -90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
  glLoadMatrixf(glm::value_ptr(V*M));

  float radius = branch->radius()/2;
  float length = branch->length();

  // draw cuboids
  /*
  float branchVertices[] = {
    -radius, 0, -radius,
    -radius, 0,  radius,
    radius, 0, radius,
    radius, 0, -radius,
    -radius, length, -radius,
    -radius, length, radius,
    radius, length, radius,
    radius, length, -radius
  };

  int branchIndexes[] = {
    // top
    0, 1, 2,
    2, 3, 0,
    // bottom
    4, 5, 6,
    6, 7, 4,
    // left
    0, 1, 4,
    4, 5, 1,
    // right
    2, 3, 6,
    6, 7, 3,
    // front
    1, 2, 5,
    5, 6, 2,
    // back
    2, 3, 6,
    6, 7, 3
  };

  glEnableClientState(GL_VERTEX_ARRAY);
  //glEnableClientState(GL_COLOR_ARRAY);
  glVertexPointer(3,GL_FLOAT,0,branchVertices);
  //glColorPointer(3,GL_FLOAT,0,cubeColors);	

  glDrawElements(GL_TRIANGLES,sizeof(branchIndexes)/sizeof(int),GL_UNSIGNED_INT,branchIndexes);
  */

  // draw lines
  float branchVertices[] = {
    0, 0, 0,
    0, length, 0
  };
  int branchIndexes[] = {
    0, 1
  };
  //glEnableClientState(GL_VERTEX_ARRAY);
  //glVertexPointer(3,GL_FLOAT,0,branchVertices);
  //glDrawElements(GL_LINES,sizeof(branchIndexes)/sizeof(int),GL_UNSIGNED_INT,branchIndexes);
  glutSolidCone(radius, length, 10, 10);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
}

void drawGround(const glm::mat4 V) {
  float groundRadius = 10.0f;
  glm::mat4 M=glm::mat4(1.0f);
  M=glm::rotate(M,angle_y,glm::vec3(0.0f,1.0f,0.0f));
  M=glm::translate(M, glm::vec3(0.0f, -groundRadius, 0.0f));
  glLoadMatrixf(glm::value_ptr(V*M));
  glBindTexture(GL_TEXTURE_2D,grassTexture);
  glEnableClientState(GL_VERTEX_ARRAY);
  //glEnableClientState(GL_COLOR_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  float groundVertices[] = {
    -groundRadius, 0, -groundRadius,
    -groundRadius, 0, groundRadius,
    groundRadius, 0, groundRadius,
    groundRadius, 0, -groundRadius
  };
  int groundIndexes[] = {
    0, 1, 2,
    2, 3, 0
  };
  float groundColors[] = {
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0
  };
  float textureScale = 0.5f;
  float textureCoords[] = {
    0, 0,
    textureScale, 0,
    textureScale, textureScale,
    0, textureScale
  };
  glVertexPointer(3,GL_FLOAT,0,groundVertices);
  //glColorPointer(3, GL_FLOAT, 0, groundColors);
  glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
  //glDrawElements(GL_TRIANGLES,sizeof(groundIndexes)/sizeof(int),GL_UNSIGNED_INT,groundIndexes);
  glutSolidSphere(groundRadius, 50, 50);

  glDisable(GL_TEXTURE_2D);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
  //glDisableClientState(GL_COLOR_ARRAY);
}

void displayFrame(void) {
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 V=glm::lookAt(
		glm::vec3(0.0f,cameraHeight,-10.0f),
		glm::vec3(0.0f,4.0f,0.0f),
		glm::vec3(0.0f,1.0f,0.0f));
	
	glm::mat4 P=glm::perspective(50.0f, 1.0f, 1.0f, 50.0f);
	
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf(glm::value_ptr(P));
	glMatrixMode(GL_MODELVIEW);
	
  //for(int i = 0; i < std::min(1, tree->branchCount()); i++) {
  for(int i = 0; i < tree->branchCount(); i++) {
    drawBranch(tree->getBranch(i), V);
  }
		
  drawGround(V);

	glutSwapBuffers();
}

void nextFrame(void) {
	int actTime=glutGet(GLUT_ELAPSED_TIME);
	int interval=actTime-lastTime;
	lastTime=actTime;
	angle_y+=speed_y*interval/1000.0;
	if (angle_x>360) angle_x-=360;
	if (angle_x>360) angle_x+=360;
	if (angle_y>360) angle_y-=360;
	if (angle_y>360) angle_y+=360;
  tree->clock->tick();

  if(int(tree->clock->value * 1000) % 200 == 0) {
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
      if(cameraHeight < 10) cameraHeight += 0.1;
      break;
    case GLUT_KEY_DOWN:
      if(cameraHeight > 0) cameraHeight -= 0.1;
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
      break;
    case GLUT_KEY_DOWN:
      break;  
  }
}


int main(int argc, char* argv[]) {
  srand(time(0));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Growing Tree Simulation");        
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);

	glewInit();
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
	
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
  loadTexture("res/grass.tga", &grassTexture);

  glutMainLoop();
  return 0;
}
