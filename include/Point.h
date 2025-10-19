#pragma once
#include <SFML/System/Vector2.hpp>


namespace Verlet
{

	struct Point
	{
		sf::Vector2f position;
		sf::Vector2f previousPosition;
		bool isPinned;


		Point(const sf::Vector2f& pos, bool pinned = false, float mass = 1.f) :
			position(pos),
			previousPosition(pos),
			isPinned(pinned)
		{
		}
	};

}