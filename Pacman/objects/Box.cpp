
#include <iostream>
#include <stdexcept>

#include "Box.h"
#include "Test.h"
#include "_util.h"

// Globals
GLuint Box::vao = 0;
GLuint Box::position_vbo = 0;
GLuint Box::colour_vbo = 0;
Shader Box::_shader = Shader();
bool Box::_ready = false;

// Constructors
Box::Box()
{
	_Init(40, 20, 0, 0, 5, WHITE, BLACK);
}

Box::Box(int xsize, int ysize, int xpos, int ypos)
{
	_Init(xsize, ysize, xpos, ypos, 5, WHITE, BLACK);
	if (!_Assert())
		throw std::invalid_argument("Box [WARNING]: constructor recieved an invalid input.");
}

Box::Box(int xsize, int ysize, int xpos, int ypos, int border)
{
	_Init(xsize, ysize, xpos, ypos, border, WHITE, BLACK);
	if (!_Assert())
		throw std::invalid_argument("Box [WARNING]: constructor recieved an invalid input.");
}

Box::Box(int xsize, int ysize, int xpos, int ypos, glm::vec4 outer_colour, glm::vec4 inner_colour)
{
	_Init(xsize, ysize, xpos, ypos, 5, outer_colour, inner_colour);
	if (!_Assert())
		throw std::invalid_argument("Box [WARNING]: constructor recieved an invalid input.");
}

Box::Box(int xsize, int ysize, int xpos, int ypos, int border, glm::vec4 outer_colour, glm::vec4 inner_colour)
{
	_Init(xsize, ysize, xpos, ypos, border, outer_colour, inner_colour);
	if (!_Assert())
		throw std::invalid_argument("Box [WARNING]: constructor recieved an invalid input.");
}

Box::Box(const Box &old_box)
{
	_Init(old_box._xsize, old_box._ysize, old_box._xpos, old_box._ypos, old_box._border,
		old_box._outer_colour, old_box._inner_colour);
	if (!_Assert())
		throw std::invalid_argument("Box [WARNING]: constructor recieved an invalid input.");
}

// Destructor
Box::~Box() {}

// Getter methods
int Box::GetXSize()
{
	return _xsize;
}

int Box::GetYSize()
{
	return _ysize;
}

int Box::GetXPosition()
{
	return _xpos;
}

int Box::GetYPosition()
{
	return _ypos;
}

int Box::GetBorderSize()
{
	return _border;
}

// Setter methods
void Box::SetXSize(int x)
{
	_xsize = x;
	_Assert();
}

void Box::SetYSize(int y)
{
	_ysize = y;
	_Assert();
}

void Box::SetXPosition(int x)
{
	_xpos = x;
	_Assert();
}

void Box::SetYPosition(int y)
{
	_ypos = y;
	_Assert();
}

void Box::SetBorderSize(int size)
{
	_border = size;
	_Assert();
}

void Box::Translate(int x, int y)
{
	_SetValues(_xsize, _ysize, _xpos + x, _xpos + y);
	_Assert();
}

void Box::SetColour(glm::vec4 outer_colour, glm::vec4 inner_colour)
{
	_SetColours(outer_colour, inner_colour);
}

void Box::SetOuterColour(glm::vec4 colour)
{
	_outer_colour = colour;
}

void Box::SetInnerColour(glm::vec4 colour)
{
	_inner_colour = colour;
}

// Create GL objects before first draw
void Box::Prepare(Shader shader)
{
	_shader = shader;

	_CreateGLObjects();
}

// Destroy GL objects
void Box::Cleanup()
{
	if (glIsBuffer(position_vbo) == GL_TRUE)
		glDeleteBuffers(1, &position_vbo);
	if (glIsBuffer(colour_vbo) == GL_TRUE)
		glDeleteBuffers(1, &colour_vbo);
	if (glIsVertexArray(vao) == GL_TRUE)
		glDeleteVertexArrays(1, &vao);
	_ready = false;
}

// Rendering method
void Box::Draw() { Draw(0, 0); }
void Box::Draw(int x_translate, int y_translate)
{
	int old_xpos, old_ypos;

	if ( !_Assert() )
		return;

	if (x_translate != 0 || y_translate != 0)
	{
		old_xpos = _xpos;
		old_ypos = _ypos;

		_SetValues(_xsize, _ysize, _xpos + x_translate, _ypos + y_translate);
	}

	_Draw();
	
	if (x_translate != 0 || y_translate != 0)
	{
		_SetValues(_xsize, _ysize, old_xpos, old_ypos);
	}
}

// Private helper methods
void Box::_Init(int xsize, int ysize, int xpos, int ypos, int border, glm::vec4 outer_colour, glm::vec4 inner_colour)
{
	_SetValues(xsize, ysize, xpos, ypos);
	_SetColours(outer_colour, inner_colour);
	SetBorderSize(border);
}

bool Box::_Assert()
{
	if (_xsize < 11)
	{
		printf("Box [WARNING]: _Assert found invalid _xsize (%d).\n", _xsize);
		return false;
	}
	if (_ysize < 11)
	{
		printf("Box [WARNING]: _Assert found invalid _ysize (%d).\n", _ysize);
		return false;
	}
	if (_border >= _ysize / 2 || _border >= _xsize / 2)
	{
		printf("Box [WARNING]: _Assert found invalid _border (%d).\n", _border);
		return false;
	}

	return true;
}

void Box::_SetValues(int xsize, int ysize, int xpos, int ypos)
{
	_xsize = xsize;
	_ysize = ysize;
	_xpos = xpos;
	_ypos = ypos;
}

void Box::_SetColours(glm::vec4 outer_colour, glm::vec4 inner_colour)
{
	_outer_colour = outer_colour;
	_inner_colour = inner_colour;
}

glm::vec4* Box::_CreateVerticesArray()
{
	using glm::vec4;
	// object origin at bottom left corner
	// 6 vertices for box background/border
	// 6 vertices for box foreground

	vec4 *vertices = new vec4[12];
	// background/border
	vertices[0] = vec4(_xpos, _ypos, 0.0, 1.0);
	vertices[1] = vec4(_xpos + _xsize, _ypos, 0.0, 1.0);
	vertices[2] = vec4(_xpos, _ypos + _ysize, 0.0, 1.0);
	vertices[3] = vec4(_xpos, _ypos + _ysize, 0.0, 1.0);
	vertices[4] = vec4(_xpos + _xsize, _ypos, 0.0, 1.0);
	vertices[5] = vec4(_xpos + _xsize, _ypos + _ysize, 0.0, 1.0);
	// foreground
	vertices[6] = vec4(_xpos + _border, _ypos + _border, 0.0, 1.0);
	vertices[7] = vec4(_xpos + _xsize - _border, _ypos + _border, 0.0, 1.0);
	vertices[8] = vec4(_xpos + _border, _ypos + _ysize - _border, 0.0, 1.0);
	vertices[9] = vec4(_xpos + _border, _ypos + _ysize - _border, 0.0, 1.0);
	vertices[10] = vec4(_xpos + _xsize - _border, _ypos + _border, 0.0, 1.0);
	vertices[11] = vec4(_xpos + _xsize - _border, _ypos + _ysize - _border, 0.0, 1.0);

	return vertices;
}

void Box::_CreateGLObjects()
{
	_shader.UseShader();

	GLuint v_pos = glGetAttribLocation(_shader.GetProgram(), "vPosition");
	GLuint v_col = glGetAttribLocation(_shader.GetProgram(), "vColour");

	// Prepare VAO and VBOs
	if ( glIsVertexArray(vao) == GL_FALSE )
	{
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
	}

	// Store vertex positions in buffer
	if ( glIsBuffer(position_vbo) == GL_FALSE )
	{
		glGenBuffers(1, &position_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(v_pos, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(v_pos);
	}

	// Store vertex colours in buffer
	if ( glIsBuffer(colour_vbo) == GL_FALSE )
	{
		glGenBuffers(1, &colour_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
		glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(glm::vec4), NULL, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(v_col, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(v_col);
	}

	// Unbinding
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	_ready = true;
	printf("Prepared OpenGL objects for Box objects: VAO = %d, pos = %d, col = %d\n",
			vao, position_vbo, colour_vbo);
}

void Box::_PushVerticesToBuffer()
{
	glm::vec4 *vertices = _CreateVerticesArray();

	glBindBuffer(GL_ARRAY_BUFFER, position_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(glm::vec4), vertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] vertices;
}

void Box::_PushColoursToBuffer()
{
	glm::vec4 colours[12] = {};
	for (int i = 0; i < 6; i++) colours[i] = _outer_colour;
	for (int i = 6; i < 12; i++) colours[i] = _inner_colour;

	glBindBuffer(GL_ARRAY_BUFFER, colour_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(glm::vec4), colours);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Box::_Draw()
{
	if (!_ready)
		_CreateGLObjects();
	_shader.UseShader();
	_PushVerticesToBuffer();
	_PushColoursToBuffer();
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 12);
	glBindVertexArray(0);
}

// Testing methods
void Test::_CreateBoxTest()
{
	/*box_objects = (Box*) malloc(sizeof(Box) * 10);
	box_objects[0] = Box();
	box_objects[1] = Box(100, 200, 0, 600);
	box_objects[2] = Box(100, 100, 100, 700, CYAN, RED);
	box_objects[3] = Box(box_objects[2]);
	box_objects[3].SetXPosition(200);
	box_objects[4] = Box(box_objects[2]);
	box_objects[4].SetYPosition(600);
	box_objects[5] = Box(20, 20, 700, 0, LIGHTBLUE, WHITE);
	box_objects[5].SetXSize(100);
	box_objects[6] = Box(box_objects[5]);
	box_objects[6].SetYSize(100);
	box_objects[7] = Box(150, 150, 400, 400, PINK, YELLOW);
	box_objects[7].Translate(250, 250);
	box_objects[8] = Box(150, 150, 300, 300);
	box_objects[8].SetInnerColour(ORANGE);
	box_objects[8].SetOuterColour(GOLD);
	box_objects[9] = Box();*/

	box_objects = (Box*)malloc(sizeof(Box)* 64);
	box_objects[0] = Box(100, 100, 0, 0, DARKBLUE, LIGHTBLUE);
	for (int i = 1; i < 64; i++)
	{
		box_objects[i] = Box(box_objects[0]);
		box_objects[i].Translate(100 * (i % 8), 100 * (i / 8));
		if (i % 2 == 1 && (i/8) % 2 == 0)
			box_objects[i].SetColour(RED, PINK);
		else if (i % 2 == 0 && (i/8) % 2 == 1)
			box_objects[i].SetColour(DARKGREEN, LIGHTGREEN);
		else if (i % 2 == 1 && (i / 8) % 2 == 1)
			box_objects[i].SetColour(GOLD, YELLOW);
	}

	_box_test = true;
}

void Test::_DisplayBoxTest()
{
	if (!_box_test)
		return;

	/*for (int i = 0; i < 9; i++)
		box_objects[i].Draw(0, 0);
	box_objects[9].Draw(20, 20);*/

	for (int i = 0; i < 64; i++)
		box_objects[i].Draw(0, 0);
}