
#pragma once
#include "..\depend\glew\glew.h"
#include "..\depend\freeglut\freeglut.h"
#include "..\shaders\Shader.h"
#include "..\depend\glm\vec4.hpp"
#include "_colours.h"
#include "BaseObject.h"

class Box : public BaseObject
{
private:
	int _xsize, _ysize, _border;
	glm::vec4 _outer_colour, _inner_colour;
	static GLuint vao;
	static GLuint position_vbo, colour_vbo;
	static bool _ready;
	static Shader _shader;

	void _Init(int xsize, int ysize, int xpos, int ypos, int border, glm::vec4 outer_colour, glm::vec4 inner_colour);
	bool _Assert();
	void _SetValues(int xsize, int ysize, int xpos, int ypos);
	void _SetColours(glm::vec4 outer_colour, glm::vec4 inner_colour);
	glm::vec4* _CreateVerticesArray();
	void _PushVerticesToBuffer();
	void _PushColoursToBuffer();
	void _Draw();

	static void _CreateGLObjects();

public:
	Box();
	Box(int xsize, int ysize, int xpos, int ypos);
	Box(int xsize, int ysize, int xpos, int ypos, int border);
	Box(int xsize, int ysize, int xpos, int ypos, glm::vec4 outer_colour, glm::vec4 inner_colour);
	Box(int xsize, int ysize, int xpos, int ypos, int border, glm::vec4 outer_colour, glm::vec4 inner_colour);
	Box(const Box &old_box);
	~Box();

	int GetXSize();
	int GetYSize();
	int GetXPosition();
	int GetYPosition();
	int GetBorderSize();

	void SetXSize(int x);
	void SetYSize(int y);
	void SetXPosition(int x);
	void SetYPosition(int y);
	void SetBorderSize(int size);
	void Translate(int x, int y);
	void SetColour(glm::vec4 outer_colour, glm::vec4 inner_colour);
	void SetOuterColour(glm::vec4 colour);
	void SetInnerColour(glm::vec4 colour);
	static void Prepare(const Shader shader);
	static void Cleanup();

	void Draw();
	void Draw(int x_translate, int y_translate);

};

