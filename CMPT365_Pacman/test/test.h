
#pragma once

#include "..\depend\glew\glew.h"
#include "..\depend\freeglut\freeglut.h"
#include "..\depend\glm\vec4.hpp"

#include "..\objects\menu\Box.h"

#include "..\shaders\Shader.h"
#include "..\objects\_colours.h"

class Test
{
private:
	static Shader _shader;
	GLuint _vao;
	GLuint _pos_vbo, _col_vbo;
	
	Box *box_objects;
	void _CreateBoxTest();
	void _DisplayBoxTest();

public:
	Test();
	~Test();

	void SetupTest();
	void DisplayTest();
	static void SetProgram(const Shader shader);
	void BasicTest();
	void BasicTestDisplay();
};