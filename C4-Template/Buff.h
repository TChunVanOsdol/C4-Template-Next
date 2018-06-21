#pragma once
#include "Character.h"
//#include <SFML/Graphics.hpp>

class Buff 
{
public:
	float duration;
	
	Character* buff_target;
	
	Buff();
	Buff(Character& target);

	void apply(Character& target);
	void end(Character& target);

	void update(float dt);
};