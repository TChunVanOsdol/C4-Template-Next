#include "Buff.h"

Buff::Buff() {
	duration = 0.5;
}

Buff::Buff(Character& target) {
	duration = 0.5;
	buff_target = &target;
}

void Buff::apply(Character& target) {
	target.playershape.setFillColor(sf::Color::White);
}

void Buff::end(Character& target) {
	target.playershape.setFillColor(sf::Color::Red);
}

void Buff::update(float dt) {
	duration -= dt;
	if (duration > 0) {
		//Apply a periodic effect
		apply(*buff_target);
	}
	else {
		//Apply an ending effect, commonly return to normal
		end(*buff_target);
	}
	
}