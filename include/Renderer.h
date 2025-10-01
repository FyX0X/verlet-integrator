#pragma once

#include <SFML/Graphics.hpp>
#include "VerletIntegrator.h"


namespace Verlet
{

	class Renderer
	{

	public:
		Renderer(VerletIntegrator& integrator);
		
		
		void render(sf::RenderWindow& window);

	private:
		VerletIntegrator& m_integrator;
		static constexpr float POINT_RADIUS = 3.0f;
	};

}