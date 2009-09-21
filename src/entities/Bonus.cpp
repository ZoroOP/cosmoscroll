#include <cassert>
#include <SFML/System/Randomizer.hpp>

#include "Bonus.hpp"
#include "../utils/MediaManager.hpp"

#define BONUS_SPEED 100
#define SIZE        16


Bonus::Bonus(Type type, const sf::Vector2f& offset) :
	Entity(offset, 1, 0)
{
	SetImage(GET_IMG("bonus"));
	SetSubRect(GetSubRect(type));
	type_ = type;
}


Bonus* Bonus::Clone() const
{
	return new Bonus(*this);
}


Bonus* Bonus::MakeRandom(const sf::Vector2f& offset)
{
	Type type = (Type) sf::Randomizer::Random(0, BONUS_COUNT - 1);
	return new Bonus((Type) type, offset);
}


void Bonus::TakeDamage(int)
{
	// un bonus ne peut être détruit
}


void Bonus::Update(float frametime)
{
	sf::Sprite::Move(-BONUS_SPEED * frametime, 0);
	KillIfOut();
}


const wchar_t* Bonus::WhatItIs() const
{
	switch (type_)
	{
		case HEALTH:
			return L"Health";
		case SHIELD:
			return L"Bouclier";
		case COOLER:
			return L"Glaçon";
		case MISSILE:
			return L"Missile";
		case TRIPLE_SHOT:
			return L"Triple tir";
		case SPEED:
			return L"Vitesse";
		case STONED:
			return L"Stoned :D";
		case MAGIC_BANANA:
			return L"Magic Banana!";
		default:
			break;
	}
	return L"<unknow bonus>";
}


sf::IntRect Bonus::GetSubRect(Type type)
{
	return sf::IntRect(type * SIZE, 0, (type + 1) * SIZE, SIZE);
}

