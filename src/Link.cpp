#include "Link.h"
#include <SFML/System/Vector2.hpp>

namespace Verlet
{
	Link::Link(Point* p1, Point* p2, float stiffness)
	{
		m_point1 = p1;
		m_point2 = p2;
		m_stiffness = stiffness;
		sf::Vector2f delta = m_point2->position - m_point1->position;
		m_restLength = delta.length();
	}


	void Link::satisfyConstraint()
	{
		sf::Vector2f delta = m_point2->position - m_point1->position;
		float currentLength = delta.length();
		if (currentLength == 0.0f)
			return; // Prevent division by zero
		float difference = (currentLength - m_restLength) / currentLength;
		sf::Vector2f correction = delta * 0.5f * m_stiffness * difference;
		if (!m_point1->isPinned)
			m_point1->position += correction;
		if (!m_point2->isPinned)
			m_point2->position -= correction;
	}
}