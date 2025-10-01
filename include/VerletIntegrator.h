#pragma once

#include "Point.h"
#include "Link.h"
#include <vector>
#include <SFML/System/Vector2.hpp>

namespace Verlet {


	class VerletIntegrator {
	public:
		VerletIntegrator(float timeStep, const sf::Vector2f& gravity = { 0.0f, 9.81f }, float damping = .999f, int maxPointCapacity = 1000);


		// Scene management
		Point* addPoint(const sf::Vector2f& pos, bool isPinned);
		Link* addLink(Point* p1, Point* p2);
		const std::vector<Point>& getPoints() const { return m_points; }
		std::vector<Point>& getPoints() { return m_points; }
		const std::vector<Link>& getLinks() const { return m_links; }
		std::vector<Link>& getLinks() { return m_links; }

		
		// Physics parameters
		void setTimeStep(float timeStep) { m_timeStep = timeStep; }
		float getTimeStep() const { return m_timeStep; }
		void setGravity(const sf::Vector2f& gravity) { m_gravity = gravity; }
		const sf::Vector2f& getGravity() const { return m_gravity; }


		// Simulation
		void update(int constraintIterations = 5);


	private:
		void applyForces(Point& point);

		sf::Vector2f getAcceleration(const sf::Vector2f pos) const { return m_gravity; }		// could be extended for other forces

		float m_timeStep;
		float m_damping;
		sf::Vector2f m_gravity;
		std::vector<Point> m_points;
		std::vector<Link> m_links;


	};

}