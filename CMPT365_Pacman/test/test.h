
#pragma once

#include "..\depend\glew\glew.h"
#include "..\depend\freeglut\freeglut.h"
#include "..\depend\glm\vec4.hpp"

#include "..\objects\menu\Box.h"
#include "..\objects\_colours.h"

class Test
{
private:
	static GLuint _vertex_position, _vertex_colour;
	static GLuint _vao;
	static GLuint _pos_vbo, _col_vbo;
	
	Box *box_objects;
	void _CreateBoxTest();
	void _DisplayBoxTest();

public:
	Test();
	~Test();

	void SetupTest();
	void DisplayTest();
	static void SetVertexAttributes(GLuint vertex_position, GLuint vertex_colour);
	static void StaticTest();
	static void StaticTestDisplay();
};