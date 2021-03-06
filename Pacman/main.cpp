
#include <iostream>

#include "depend\glew\glew.h"
#include "depend\freeglut\freeglut.h"
#include "shaders\Shader.h"

#include "depend\glm\vec3.hpp"
#include "depend\glm\vec4.hpp"
#include "depend\glm\mat4x4.hpp"

#include "managers\GameManager.h"

#include "objects\Test.h"
#include "objects\_util.h"

using namespace std;

// GLOBAL VARIABLES
bool enable_test = false;
Test *tester_object;

int window_x = 800;
int window_y = 800;

GameManager *manager;

Shader default_shader, text_shader, sprite_shader;	// stores shader programs

void Initialize()
{
	// Prepare shaders
	default_shader = Shader(1);
	text_shader = Shader(2);
	sprite_shader = Shader(3);

	GameManager::Prepare(default_shader, text_shader, sprite_shader);

	manager = new GameManager();

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Cleanup()
{
	if (enable_test)
		delete tester_object;
	delete manager;
}

void Display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Send window length and width to shader programs
	default_shader.UseShader();
	glUniform1i(default_shader.GetX(), window_x);
	glUniform1i(default_shader.GetY(), window_y);
	text_shader.UseShader();
	glUniform1i(text_shader.GetX(), window_x);
	glUniform1i(text_shader.GetY(), window_y);
	sprite_shader.UseShader();
	glUniform1i(sprite_shader.GetX(), window_x);
	glUniform1i(sprite_shader.GetY(), window_y);

	if (enable_test)
	{
		tester_object->DisplayTest();
		//tester_object->BasicTestDisplay();
	}

	manager->Draw();

	glutSwapBuffers();
}

void Keyboard(unsigned char key, int x, int y)
{
	y = window_y - y;

	if (enable_test)
		tester_object->KeyboardTest(key, x, y);

	manager->Keyboard(key);

	switch (key)
	{
	case 'q':
		Cleanup();
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y)
{
	y = window_y - y;

	if (enable_test)
		tester_object->SpecialTest(key, x, y);

	manager->Special(key);

	glutPostRedisplay();
}

void PassiveMotion(int x, int y)
{
	y = window_y - y;

	if (enable_test)
		tester_object->PassiveMotionTest(x, y);

	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	y = window_y - y;

	if (enable_test)
		tester_object->MouseTest(button, state, x, y);

	glutPostRedisplay();
}

void Reshape(GLsizei w, GLsizei h)
{
	window_x = w;
	window_y = h;
	glViewport(0, 0, w, h);
}

int main(int argc, char **argv) {

	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(window_x, window_y);
	glutCreateWindow("Pacman");
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		printf("GLEW failed to init");
		return 1;
	}

	// Initialization
	Initialize();

	// Testing
	if (enable_test)
	{
		Test::SetProgram(default_shader);
		tester_object = new Test();
		tester_object->SetupTest();
		//tester_object->BasicTest();
	}

	// Callback registration
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(Special);
	glutPassiveMotionFunc(PassiveMotion);
	glutMouseFunc(Mouse);
	glutReshapeFunc(Reshape);

	glutMainLoop();

	return 0;
}