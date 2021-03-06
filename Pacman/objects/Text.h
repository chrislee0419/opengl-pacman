
#pragma once

#include <map>

#include "..\depend\glew\glew.h"
#include "..\depend\freeglut\freeglut.h"
#include "..\depend\freetype\ft2build.h"
#include FT_FREETYPE_H

#include "..\shaders\Shader.h"
#include "..\depend\glm\vec2.hpp"
#include "..\depend\glm\vec4.hpp"
#include "BaseObject.h"

#define ROBOTO 0
#define UBUNTU 1
#define EMULOGIC 2

struct Character
{
	GLuint TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	GLuint Advance;
};

class Text : public BaseObject
{
private:
	static Shader _shader;
	static std::map<char, Character> _ubuntu;
	static std::map<char, Character> _roboto;
	static std::map<char, Character> _emulogic;
	static GLuint _vao;
	static GLuint _vbo;
	static bool _ready;
	char _text[200];
	glm::vec4 _colour;
	float _size;
	int _font;

	void _Init(int font, float size, std::string text, int xpos, int ypos, glm::vec4 colour);
	bool _Assert();
	static void _CreateGLObjects();
	static void _PrepareFT();
	static void _LoadChar(FT_Face face, GLubyte c, int font);

public:
	Text();
	Text(std::string text);
	Text(std::string text, glm::vec4 colour);
	Text(int font, float size, std::string text, glm::vec4 colour);
	Text(std::string text, int xpos, int ypos);
	Text(int font, float size, std::string text, int xpos, int ypos);
	Text(std::string text, int xpos, int ypos, glm::vec4 colour);
	Text(int font, float size, std::string text, int xpos, int ypos, glm::vec4 colour);
	Text(const Text &copytext);
	~Text();

	float GetHeight();
	float GetWidth();
	float GetSize();

	void SetFont(int font);
	void SetSize(float size);
	void SetText(std::string text);
	void SetColour(glm::vec4 colour);
	void SetXPosition(int xpos);
	void SetYPosition(int ypos);
	void SetPosition(int xpos, int ypos);
	void Translate(int x, int y);
	static void Prepare(const Shader shader);
	static void Cleanup();

	void Draw();
	void Draw(int x_translate, int y_translate);

};
