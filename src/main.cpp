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
#include "leaf.h"
#include "sky.h"
#include "spherical_coordinates.h"

int lastTime = 0;

GLuint grassTexture;
GLuint branchTexture;
GLuint leafTexture;
TGAImg image;

Clock *worldClock = new Clock();
Tree *tree = new Tree(worldClock);
Sky *sky = new Sky(worldClock);

SphericalCoordinates *cameraCoordinates = new SphericalCoordinates(10.0f, 0.0f, 0.45f * M_PI, 5.0f, 25.0f, 0.05 * M_PI, 0.45f * M_PI);
bool cameraFlags[6] = { false, false, false, false, false };
bool speedupFlag = false;

#define CAMERA_UP_KEY GLUT_KEY_UP
#define CAMERA_DOWN_KEY GLUT_KEY_DOWN
#define CAMERA_LEFT_KEY GLUT_KEY_LEFT
#define CAMERA_RIGHT_KEY GLUT_KEY_RIGHT
#define CAMERA_ZOOMIN_KEY 'z'
#define CAMERA_ZOOMOUT_KEY 'x'
#define CLOCK_SPEEDUP_KEY ' '

#define CLOCK_SPEEDUP_FACTOR 10.0f

void loadTexture(char *filename, GLuint *handle) {
  printf("Loading texture %s\n", filename);
  if (image.Load(filename) == IMG_OK) {
    glGenTextures(1, handle);
    glBindTexture(GL_TEXTURE_2D, *handle);
    if (image.GetBPP() == 24) {
      glTexImage2D(GL_TEXTURE_2D, 0, 3, image.GetWidth(), image.GetHeight(), 0, GL_RGB,GL_UNSIGNED_BYTE, image.GetImg());
    }
    else if (image.GetBPP() == 32) {
      glTexImage2D(GL_TEXTURE_2D, 0, 4, image.GetWidth(), image.GetHeight(), 0, GL_RGBA,GL_UNSIGNED_BYTE, image.GetImg());
    }
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else
    printf("Error loading texture %s\n", filename);
}

void drawLeaf(Leaf *leaf, glm::mat4 V, glm::mat4 M) {
  glm::mat4 lM = glm::translate(M, glm::vec3(0.0f, 0.0f, leaf->rootDistance()));
  glLoadMatrixf(glm::value_ptr(V*lM));
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,leafTexture);
  GLUquadric *qobj = gluNewQuadric();
  gluQuadricTexture(qobj,GL_TRUE);
  gluSphere(qobj,leaf->length(), 10, 10);
  gluDeleteQuadric(qobj);
  glDisable(GL_TEXTURE_2D);
}

void drawBranch(Branch *branch, const glm::mat4 V) {
  glm::mat4 M=glm::mat4(1.0f);
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
  glEnable(GL_TEXTURE_2D);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  float radius = branch->radius()/2;
  float length = branch->length();

  glBindTexture(GL_TEXTURE_2D,branchTexture);
  GLUquadric *qobj = gluNewQuadric();
  gluQuadricTexture(qobj,GL_TRUE);
  gluCylinder(qobj,radius, 0, length,50,50);
  gluDeleteQuadric(qobj);
  glDisable(GL_TEXTURE_2D);

  for(int l = 0; l < branch->leaves.size(); l++) {
    drawLeaf(branch->leaves[l], V, M);
  }
}

void drawGround(const glm::mat4 V) {
  glBindTexture(GL_TEXTURE_2D,grassTexture);
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_TEXTURE_COORD_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnable(GL_TEXTURE_2D);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

  int groundDimension = 50;
  float groundRadius = 1.0f;
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
  float textureScale = 0.5f;
  float textureCoords[] = {
    0, 0,
    textureScale, 0,
    textureScale, textureScale,
    0, textureScale
  };
  float groundNormals[] = {
    0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0
  };

  for(int x = 0; x < groundDimension; x++) {
    for(int y = 0; y < groundDimension; y++) {
      glm::mat4 M=glm::translate(glm::mat4(1.0f), glm::vec3(groundRadius * 2 * x - groundDimension / groundRadius, 0.0f, groundRadius * 2 * y - groundDimension / groundRadius));
      glLoadMatrixf(glm::value_ptr(V*M));
      glVertexPointer(3,GL_FLOAT,0,groundVertices);
      glTexCoordPointer(2, GL_FLOAT, 0, textureCoords);
      glNormalPointer(GL_FLOAT, 0, groundNormals);
      glDrawElements(GL_TRIANGLES,sizeof(groundIndexes)/sizeof(int),GL_UNSIGNED_INT,groundIndexes);
    }
  }

  glDisable(GL_TEXTURE_2D);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_TEXTURE_COORD_ARRAY);
  glDisableClientState(GL_VERTEX_ARRAY);
}

void drawTree(Tree *tree, const glm::mat4 V) {
  for(int i = 0; i < tree->branchCount(); i++) {
    drawBranch(tree->getBranch(i), V);
  }
}

void drawLight(const glm::mat4 V) {
  glLoadMatrixf(glm::value_ptr(V));

  float light0Position[] = { 0.0f, 20.0f, 0.0f, 1.0f};
  float light0Attenuation = 10*pow(sin(2 * M_PI * worldClock->value / 360.0f - M_PI / 4), 2);
  glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
  glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, light0Attenuation);

  float light1Ambient[] = { 0.2, 0.2, 0.2, 1.0 };
  float light1Diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
  float light1Specular[] = { 1.0, 1.0, 1.0, 1.0 };
  float light1Position[] = { 5.0f, 0.5f, 0.0f, 1.0f};

  glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
  glLightfv(GL_LIGHT1, GL_SPECULAR, light1Specular);
  glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
}

void displayFrame(void) {
  glm::vec3 skyColor = sky->color();
  glClearColor(skyColor.r, skyColor.g, skyColor.b, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glm::mat4 P=glm::perspective(50.0f, 1.0f, 1.0f, 50.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadMatrixf(glm::value_ptr(P));

  glMatrixMode(GL_MODELVIEW);

  float altitude = tree->getTrunk()->length();
  glm::vec3 cameraTarget = glm::vec3(0.0f, altitude, 0.0f);
  glm::vec3 cameraObserver = cameraCoordinates->toCarthesian() + glm::vec3(0.0f, altitude, 0.0f);
  glm::vec3 cameraNose = glm::vec3(0.0f, 1.0f, 0.0f);
  glm::mat4 V=glm::lookAt(cameraObserver, cameraTarget, cameraNose);

  drawGround(V);
  drawTree(tree, V);
  drawLight(V);

  glutSwapBuffers();
}

void nextFrame(void) {
	int actTime = glutGet(GLUT_ELAPSED_TIME);
	float delta = (actTime - lastTime) / 1000.f;
	lastTime = actTime;
  float angleSpeed = 1.0f;
  float zoomSpeed = 2.0f;

  if(cameraFlags[0]) // left {
    cameraCoordinates->changeAzimuth(angleSpeed * -delta);
  if(cameraFlags[1]) // right
    cameraCoordinates->changeAzimuth(angleSpeed * delta);
  if(cameraFlags[2]) // up
    cameraCoordinates->changePolar(angleSpeed * delta);
  if(cameraFlags[3]) // down
    cameraCoordinates->changePolar(angleSpeed * -delta);
  if(cameraFlags[4]) // zoom in
    cameraCoordinates->changeDistance(zoomSpeed * -delta);
  if(cameraFlags[5]) // zoom out
    cameraCoordinates->changeDistance(zoomSpeed * delta);

  worldClock->tick();
  tree->grow();

	glutPostRedisplay();
}

void keyDown(int c, int x, int y) {
  switch (c) {
    case CAMERA_LEFT_KEY:
      cameraFlags[0] = true;
      break;
    case CAMERA_RIGHT_KEY:
      cameraFlags[1] = true;
      break;
    case CAMERA_UP_KEY:
      cameraFlags[2] = true;
      break;
    case CAMERA_DOWN_KEY:
      cameraFlags[3] = true;
      break;
  }
}

void keyUp(int c, int x, int y) {
  switch (c) {
    case CAMERA_LEFT_KEY:
      cameraFlags[0] = false;
      break;
    case CAMERA_RIGHT_KEY:
      cameraFlags[1] = false;
      break;
    case CAMERA_UP_KEY:
      cameraFlags[2] = false;
      break;
    case CAMERA_DOWN_KEY:
      cameraFlags[3] = false;
      break;
  }
}

void letterDown(unsigned char key, int x, int y) {
  switch(key) {
    case CAMERA_ZOOMIN_KEY:
      cameraFlags[4] = true;
      break;
    case CAMERA_ZOOMOUT_KEY:
      cameraFlags[5] = true;
      break;
    case CLOCK_SPEEDUP_KEY:
      if(!speedupFlag) {
        worldClock->speedUp(CLOCK_SPEEDUP_FACTOR);
        speedupFlag = true;
      }
      break;
  }
}

void letterUp(unsigned char key, int x, int y) {
  switch(key) {
    case CAMERA_ZOOMIN_KEY:
      cameraFlags[4] = false;
      break;
    case CAMERA_ZOOMOUT_KEY:
      cameraFlags[5] = false;
      break;
    case CLOCK_SPEEDUP_KEY:
      if(speedupFlag) {
        worldClock->slowDown(CLOCK_SPEEDUP_FACTOR);
        speedupFlag = false;
      }
      break;
  }
}

void initKeyboard() {
	glutSpecialFunc(keyDown);
	glutSpecialUpFunc(keyUp);
  glutKeyboardFunc(letterDown);
  glutKeyboardUpFunc(letterUp);
}

void initLight() {
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHT1);
}

void initTextures() {
  loadTexture("res/grass.tga", &grassTexture);
  loadTexture("res/branch.tga", &branchTexture);
  loadTexture("res/leaf.tga", &leafTexture);
}

void initGlut(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800,800);
	glutInitWindowPosition(0,0);
	glutCreateWindow("Growing Tree Simulation");
	glutDisplayFunc(displayFrame);
	glutIdleFunc(nextFrame);
	glewInit();
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
}

void initRandomness() {
  srand(time(0));
}

int main(int argc, char* argv[]) {
  initRandomness();

  initGlut(argc, argv);

  initKeyboard();

  initLight();

  initTextures();

  glutMainLoop();

  return 0;
}
