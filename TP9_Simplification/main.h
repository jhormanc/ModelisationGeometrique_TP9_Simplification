#pragma once


#include "GL/freeglut.h"    
#include <stdio.h>      
#include <stdlib.h>     
#include <math.h>
#include <string>
#include "MeshWriter.h"

#define WIDTH  800
#define HEIGHT 600

#define RED   0
#define GREEN 0
#define BLUE  0
#define ALPHA 0
#define PI 3.14159

#define KEY_ESC 27

GLfloat ambient[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat diffuse[] = { 0.9, 0.9, 0.2, 1.0 };
GLfloat position[] = { 0.0, 3.0, 3.0, 0.0 };

GLfloat specular[] = { 1.0, 1.0, 0.5, 1.0 };
GLfloat shininess[] = { 100.0 };

Mesh mesh;
int num_mesh = 0;
int mode_rendu;
int last_x, last_y;
double dist;
float x_rot, y_rot, z_rot;
bool look = false;
bool mode_texture, mode_fill;
GLuint *buffers;

void init_scene();
void render_scene();
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid window_key(unsigned char key, int x, int y);
GLvoid window_mouse_click(int button, int state, int x, int y);
GLvoid window_mouse(int x, int y);
GLvoid window_idle();
void rotateBy(int x, int y, int z);
std::string changeMesh();
void traceMaillage();