#pragma once
#include "Point.h"


namespace Verlet
{

	class Link
	{
	public:
		// Create a link between two points with a given stiffness (1.0f = rigid, <1.0f = elastic)
		Link(Point* p1, Point* p2, float stiffness = 1.0f, float initialTension = 1.f);

		Point* getPoint1() const { return m_point1; }
		Point* getPoint2() const { return m_point2; }

		void satisfyConstraint();
		sf::Vector2f getMidpoint() const { return 0.5f * (m_point1->position + m_point2->position); }

	private:
		Point* m_point1;
		Point* m_point2;
		float m_stiffness;
		float m_restLength;
	};

}