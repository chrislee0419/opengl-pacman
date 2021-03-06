
#pragma once

#include "../objects/Scoreboard.h"
#include "../objects/GameBoard.h"
#include"../shaders/Shader.h"

class GameManager
{
private:
	int m_gamestate;
	Scoreboard *m_score;
	GameBoard *m_board;

public:
	GameManager();
	~GameManager();

	void Keyboard(int key);
	void Special(int key);

	static void Prepare(Shader default_shader, Shader text_shader, Shader sprite_shader);

	void Draw();

};
