#include <windows.h>
#include "GL\glew.h"
#include "glut.h"
#include "3DObject.h"
//#include <vector>
#include <stdlib.h>
#include "glutil.h"
#include "ModelObj3d.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

GLuint p, f, v;
// Globals
// Real programs don't use globals :-D
// Data would normally be read from files
/*GLfloat vertices[] = {	-0.8f,-0.8f,0.0f,
						0.8f,-0.8f,0.0f,
						-0.8f,0.8f,0.0f };
GLfloat colours[] = {	1.0f, 0.0f, 0.0f,
						0.0f, 1.0f, 0.0f,
						0.0f, 0.0f, 1.0f };*/

GLfloat *vertices;
GLfloat *colours;

float ang = 0;

// two vertex array objects, one for each object drawn
unsigned int vertexArrayObjID[2];
// three vertex buffer objects in this example
unsigned int vertexBufferObjID[3];

glm::mat4x4 projection;
glm::mat4x4 model;
glm::mat4x4 view;

float viewAngle = 45;

float cx,cy,cz;

// loadFile - loads text file into char* fname
// allocates memory - so need to delete after use
// size of file returned in fSize
char* loadFile(char *fname, GLint &fSize)
{
	ifstream::pos_type size;
	char * memblock;
	string text;

	// file read based on example in cplusplus.com tutorial
	ifstream file (fname, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		size = file.tellg();
		fSize = (GLuint) size;
		memblock = new char [size];
		file.seekg (0, ios::beg);
		file.read (memblock, size);
		file.close();
		cout << "file " << fname << " loaded" << endl;
		text.assign(memblock);
	}
	else
	{
		cout << "Unable to open file " << fname << endl;
		exit(1);
	}
	return memblock;
}


// printShaderInfoLog
// From OpenGL Shading Language 3rd Edition, p215-216
// Display (hopefully) useful error messages if shader fails to compile
void printShaderInfoLog(GLint shader)
{
	int infoLogLen = 0;
	int charsWritten = 0;
	GLchar *infoLog;

	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLen);

	// should additionally check for OpenGL errors here

	if (infoLogLen > 0)
	{
		infoLog = new GLchar[infoLogLen];
		// error check for fail to allocate memory omitted
		glGetShaderInfoLog(shader,infoLogLen, &charsWritten, infoLog);
		cout << "InfoLog:" << endl << infoLog << endl;
		delete [] infoLog;
	}

	// should additionally check for OpenGL errors here
}


void init(void)
{

    vertices = new GLfloat[180000];
    colours = new GLfloat[180000];

    float frax = 0.016;

    int index = 0;
    float basex = -0.8;
    float basey = -0.8;
    for(int i = 0; i < 100; i++){
        basex = -0.8;
        for(int j = 0; j < 100; j++){
            vertices[index] = basex;
            vertices[index+1] = basey;
            vertices[index+2] = 0;

            vertices[index+3] = basex+frax;
            vertices[index+4] = basey;
            vertices[index+5] = 0;

            vertices[index+6] = basex;
            vertices[index+7] = basey+frax;
            vertices[index+8] = 0;


            vertices[index+9] = basex+frax;
            vertices[index+10] = basey;
            vertices[index+11] = 0;

            vertices[index+12] = basex+frax;
            vertices[index+13] = basey+frax;
            vertices[index+14] = 0;

            vertices[index+15] = basex;
            vertices[index+16] = basey+frax;
            vertices[index+17] = 0;

            colours[index] = 1;
            colours[index+1] = 0;
            colours[index+2] = 0;

            colours[index+3] = 0;
            colours[index+4] = 1;
            colours[index+5] = 0;

            colours[index+6] = 0;
            colours[index+7] = 0;
            colours[index+8] = 1;


            colours[index+9] = 1;
            colours[index+10] = 0;
            colours[index+11] = 0;

            colours[index+12] = 0;
            colours[index+13] = 1;
            colours[index+14] = 0;

            colours[index+15] = 0;
            colours[index+16] = 0;
            colours[index+17] = 1;

            index+=18;
            basex+=frax;
        }
        basey+=frax;
    }

	// Would load objects from file here - but using globals in this example

	// Allocate Vertex Array Objects
	glGenVertexArrays(1, &vertexArrayObjID[0]);
	// Setup first Vertex Array Object
	glBindVertexArray(vertexArrayObjID[0]);
	glGenBuffers(2, vertexBufferObjID);

	// VBO for vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[0]);
	glBufferData(GL_ARRAY_BUFFER, 180000*sizeof(GLfloat), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	// VBO for colour data
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObjID[1]);
	glBufferData(GL_ARRAY_BUFFER, 180000*sizeof(GLfloat), colours, GL_STATIC_DRAW);
	glVertexAttribPointer((GLuint)1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Setup second Vertex Array Object
	glBindVertexArray(vertexArrayObjID[1]);
	glGenBuffers(1, &vertexBufferObjID[2]);

	glBindVertexArray(0);

	projection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.f);
	model = glm::mat4();
	view = glm::rotate(glm::mat4(),glm::radians(45.0f),glm::vec3(1.0,0,0));

	cx = cy = 0;
	cz = -4;

}



void initShaders(void)
{


	char *vs,*fs;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);

	// load shaders & get length of each
	GLint vlen;
	GLint flen;

	cout << "Vai Carregar";
	vs = loadFile("minimal.vert",vlen);
	fs = loadFile("minimal.frag",flen);
	cout << "Carregou";

	const char * vv = vs;
	const char * ff = fs;

	glShaderSource(v, 1, &vv,&vlen);
	glShaderSource(f, 1, &ff,&flen);

	GLint compiled;

	glCompileShader(v);
	glGetShaderiv(v, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		cout << "Vertex shader not compiled." << endl;
		printShaderInfoLog(v);
	}

	glCompileShader(f);
	glGetShaderiv(f, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		cout << "Fragment shader not compiled." << endl;
		printShaderInfoLog(f);
	}

	p = glCreateProgram();

	glBindAttribLocation(p,0, "in_Position");
	glBindAttribLocation(p,1, "in_Color");

	glAttachShader(p,v);
	glAttachShader(p,f);

	glLinkProgram(p);
	glUseProgram(p);

	delete [] vs; // dont forget to free allocated memory
	delete [] fs; // we allocated this in the loadFile function...
}

void display(void)
{
	// clear the screen
	ang+=0.08;

	//viewAngle+=0.1;

	view = glm::translate(glm::mat4(),glm::vec3(cx,cy,cz));
	view = glm::rotate(view,glm::radians(viewAngle),glm::vec3(1.0,0,0));


    glUniformMatrix4fv( glGetUniformLocation( p, "projection" ), 1, GL_FALSE, glm::value_ptr( projection ) );
    glUniformMatrix4fv( glGetUniformLocation( p, "model" ), 1, GL_FALSE, glm::value_ptr( model ) );
    glUniformMatrix4fv( glGetUniformLocation( p, "view" ), 1, GL_FALSE, glm::value_ptr( view ) );

	//cout << "ang " << ang << endl;
	glUniform1f( glGetUniformLocation(p,"ang"),ang);

	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vertexArrayObjID[0]);	// First VAO
	glDrawArrays(GL_TRIANGLES, 0, 60000);	// draw first object


	glBindVertexArray(0);

	glutSwapBuffers();
}

void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
}

void update(int calbac){
    glutPostRedisplay();
    glutTimerFunc(5,update,1);
}

void keyboard(unsigned char key, int x, int y)
{
    //std::cout << "key " << (int)key << std::endl;

    switch(key) {
        case 'o':
            viewAngle++;
        break;
        case 'l':
            viewAngle--;
        break;
        case 'w':
            cy+=0.1;
        break;
        case 's':
             cy-=0.1;
        break;
        case 'a':
            cx-=0.1;
        break;
        case 'd':
            cx+=0.1;
        break;
        case 'q':
            cz+=0.1;
        break;
        case 'e':
            cz-=0.1;
        break;
        default:
        break;
    }
}

int main (int argc, char* argv[])
{

	cout << "Iniciando";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(600,600);
	glutCreateWindow("Triangle Test");
	glewInit();
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		cout << "glewInit failed, aborting." << endl;
		exit (1);
	}
	cout << "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
	cout << "OpenGL version " << glGetString(GL_VERSION) << " supported" << endl;

	init();
	initShaders();
    glutKeyboardFunc(keyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutTimerFunc(5,update,1);

	glutMainLoop();
	return 0;

}
