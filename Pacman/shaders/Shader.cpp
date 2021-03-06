/*
//	code adapted from: http://in2gpu.com/2014/10/29/shaders-basics/
//	and from: http://learnopengl.com/#!In-Practice/Text-Rendering
*/

#include "Shader.h"
#include <iostream>
#include <fstream>
#include <vector>

#define GLSL(version, A) "#version " #version "\n" #A

// Constructor and destructor
Shader::Shader()
{
	_program = 0;
	_ready = false;
}

Shader::Shader(int type)
{
	_program = 0;
	_ready = false;
	CreateProgram(type);
}

Shader::Shader(char* vertexShaderFilename, char* fragmentShaderFilename)
{
	_program = 0;
	_ready = false;
	CreateProgram(vertexShaderFilename, fragmentShaderFilename);
}

Shader::~Shader() {}

// Extracts shader code into a string
std::string Shader::_ReadShader(char *filename) {
	std::string shader_code;
	std::ifstream file(filename, std::ios::in);

	if ( !file.good() ) {
		std::cout << "Can't read file " << filename << std::endl;
		std::terminate();
	}

	file.seekg(0, std::ios::end);
	shader_code.resize( (unsigned int)file.tellg() );
	file.seekg(0, std::ios::beg);
	file.read(&shader_code[0], shader_code.size());
	return shader_code;
}

// Compiles shader from shader code
GLuint Shader::_CreateShader(GLenum shaderType, std::string source, char* shaderName) {
	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	if (compile_result == GL_FALSE) {
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		std::cout << "Error compiling shader: " << shaderName << std::endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

// Public methods
GLuint Shader::GetProgram()
{
	if (!_ready)
	{
		printf("Shader [WARNING]: GetProgram called on a non-ready shader program, returning -1.\n");
		return -1;
	}
	return _program;
}

// Creates default shader programs
void Shader::CreateProgram(int type)
{
	std::string vertex_shader_code, fragment_shader_code;
	// default shader
	if (type == 1)
	{
		vertex_shader_code = GLSL(330,
			in vec4 vPosition;
			in vec4 vColour;
			out vec4 colour;
	
			uniform int xsize;
			uniform int ysize;
	
			void main(void) {
				mat4 scale = mat4(
					2.0 / xsize, 0.0, 0.0, 0.0,
					0.0, 2.0 / ysize, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
					);
	
				vec4 position = vPosition + vec4(-400, -400, 0, 0);
				gl_Position = scale * position;

				colour = vColour;
			});
		fragment_shader_code = GLSL(330,
			in vec4 colour;
			out vec4 fColour;
	
			void main(void) {
			fColour = colour;
			});
	}
	// text shader
	else if (type == 2)
	{
		vertex_shader_code = GLSL(330,
			in vec4 vertex;
			out vec2 TexCoor;
	
			uniform int xsize;
			uniform int ysize;
	
			void main(void) {
	
				mat4 scale = mat4(
					2.0 / xsize, 0.0, 0.0, 0.0,
					0.0, 2.0 / ysize, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
					);
	
				vec4 position = vec4(vertex.xy, 0, 1) + vec4(-400, -400, 0, 0);
				gl_Position = scale * position;
	
				TexCoor = vertex.zw;
			});
		fragment_shader_code = GLSL(330,
			in vec2 TexCoor;
			out vec4 fColour;

			uniform sampler2D text;
			uniform vec4 textColour;

			void main(void) {
				vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoor).r);
				fColour = textColour * sampled;
			});
	}
	else if (type == 3)
	{
		vertex_shader_code = GLSL(330,
			in vec4 vertex;
			out vec2 TexCoor;
	
			uniform int xsize;
			uniform int ysize;
	
			void main(void) {
				mat4 scale = mat4(
					2.0 / xsize, 0.0, 0.0, 0.0,
					0.0, 2.0 / ysize, 0.0, 0.0,
					0.0, 0.0, 1.0, 0.0,
					0.0, 0.0, 0.0, 1.0
					);

				vec4 position = vec4(vertex.xy, 0, 1) + vec4(-400, -400, 0, 0);
				gl_Position = scale * position;

				TexCoor = vertex.zw;
			});
		fragment_shader_code = GLSL(330,
			in vec2 TexCoor;
			out vec4 fColour;

			uniform sampler2D text;
			uniform vec4 spriteColour;

			void main(void) {
				fColour = spriteColour * texture(text, TexCoor);
			});
	}
	else return;

	_CreateProgram(vertex_shader_code, fragment_shader_code);
		
}

// Creates shader program from separate files
void Shader::CreateProgram(char* vertexShaderFilename, char* fragmentShaderFilename) {
	std::string vertex_shader_code = _ReadShader(vertexShaderFilename);
	std::string fragment_shader_code = _ReadShader(fragmentShaderFilename);

	_CreateProgram(vertex_shader_code, fragment_shader_code);
}

// Sets as active shader program
void Shader::UseShader()
{
	if (!_ready)
	{
		printf("Shader [ERROR]: UseShader called on a non-ready shader program.\n");
		return;
	}
	glUseProgram(_program);
}

// Gets the xsize uniform location
GLuint Shader::GetX()
{
	return _loc_x;
}

// Gets the ysize uniform location
GLuint Shader::GetY()
{
	return _loc_y;
}

void Shader::_CreateProgram(std::string v_code, std::string f_code)
{
	GLuint vertex_shader = _CreateShader(GL_VERTEX_SHADER, v_code, "vertex shader");
	GLuint fragment_shader = _CreateShader(GL_FRAGMENT_SHADER, f_code, "fragment shader");

	int link_result = 0;

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);

	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length);
		std::vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]);
		std::cout << "Error encountered when linking shaders with program" << std::endl << &program_log[0] << std::endl;
		_ready = false;
		return;
	}
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	_loc_x = glGetUniformLocation(program, "xsize");
	_loc_y = glGetUniformLocation(program, "ysize");

	_ready = true;
	_program = program;
}
