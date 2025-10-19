#include "VerletIntegrator.h"
#include <stdexcept>
#include <string>
#include <iostream>

namespace Verlet {



	VerletIntegrator::VerletIntegrator(float timeStep, const sf::Vector2f& gravity, float damping, int maxPointCapacity):
		m_timeStep(timeStep),
		m_gravity(gravity),
		m_damping(damping)
	{
		m_points.reserve(maxPointCapacity);
		m_links.reserve(maxPointCapacity * 2);		// rough estimate, does not break anything if exceeded
	}


#pragma region SceneManagement

	Point* VerletIntegrator::addPoint(const sf::Vector2f& pos, bool isPinned)
	{
		if (m_points.size() >= m_points.capacity())
		{
			std::string msg = "Maximum point capacity (" + std::to_string(m_points.capacity()) + ") reached. \nPlease initialize VerletIntegrator with greater capacity.";
			std::cout << msg << std::endl;
			throw std::runtime_error(msg);
		}
		m_points.emplace_back(pos, isPinned);
		return &m_points.back();
	}

	Link* VerletIntegrator::addLink(Point* p1, Point* p2, float stiffness, float initialTension)
	{
		m_links.emplace_back(p1, p2, stiffness, initialTension);
		return &m_links.back();
	}

	void VerletIntegrator::clearScene()
	{
		m_points.clear();
		m_links.clear();
	}



#pragma endregion

	void VerletIntegrator::update(int constraintIterations)
	{

		for (Point& p : m_points)
		{
			applyForces(p);
		}


		for (size_t i = 0; i < constraintIterations; i++)
		{
			for (Link& l : m_links) 
			{
				l.satisfyConstraint();
			}
		}

	}


	void VerletIntegrator::applyForces(Point& point)
	{
		if (point.isPinned)
			return;

		sf::Vector2f oldPos = point.position;
		sf::Vector2f acceleration = getAcceleration(point.position);
		point.position += (point.position - point.previousPosition) * m_damping + acceleration * m_timeStep * m_timeStep;
		point.previousPosition = oldPos;
	}






}