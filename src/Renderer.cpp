#include "Renderer.h"

#include <SFML/Graphics/PrimitiveType.hpp>

namespace Verlet
{

	Renderer::Renderer(VerletIntegrator& integrator) :
		m_integrator(integrator)
	{
	}
	
	void Renderer::render(sf::RenderWindow& window)
	{
		// Render links
		sf::VertexArray linkVertices(sf::PrimitiveType::Lines);
		for (const Link& link : m_integrator.getLinks())
		{
			const Point* p1 = link.getPoint1();
			const Point* p2 = link.getPoint2();
			linkVertices.append(sf::Vertex{ p1->position, sf::Color::White });
			linkVertices.append(sf::Vertex{ p2->position, sf::Color::White });
		}
		window.draw(linkVertices);
		// Render points
		sf::CircleShape pointShape(POINT_RADIUS);
		pointShape.setOrigin({ POINT_RADIUS, POINT_RADIUS });
		for (const Point& point : m_integrator.getPoints())
		{
			pointShape.setPosition(point.position);
			pointShape.setFillColor(point.isPinned ? sf::Color::Red : sf::Color::Green);
			window.draw(pointShape);
		}
	}

}