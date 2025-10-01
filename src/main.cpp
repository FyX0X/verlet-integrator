#include <SFML/Graphics.hpp>
#include "VerletIntegrator.h"
#include "Renderer.h"
#include <iostream>


void printAllPoints(const Verlet::VerletIntegrator& integrator)
{
    const std::vector<Verlet::Point>& points = integrator.getPoints();
    for (size_t i = 0; i < points.size(); i++)
    {
        const Verlet::Point& p = points[i];
        std::cout << "Point " << i << ": (" << p.position.x << ", " << p.position.y << "), pinned: " << (p.isPinned ? "true" : "false") << std::endl;
    }
}

void printAllLinks(const Verlet::VerletIntegrator& integrator)
{
    const std::vector<Verlet::Link>& links = integrator.getLinks();
    for (size_t i = 0; i < links.size(); i++)
    {
        const Verlet::Link& l = links[i];
        const Verlet::Point* p1 = l.getPoint1();
        const Verlet::Point* p2 = l.getPoint2();
        std::cout << "Link " << i << ": Point1(" << p1->position.x << ", " << p1->position.y << "), Point2(" << p2->position.x << ", " << p2->position.y << ")" << std::endl;
    }
}

void initializeGrid(sf::Vector2f origin, int rows, int cols, float spacing, Verlet::VerletIntegrator& integrator)
{
    std::vector<Verlet::Point*> points;
    points.reserve(rows * cols);
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            bool isPinned = (y == 0); // Pin the top row of points
            points.push_back(integrator.addPoint({ origin.x + x * spacing, origin.y + y * spacing }, isPinned));
        }
    }
    // Create links
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            if (x < cols - 1)
                integrator.addLink(points[y * cols + x], points[y * cols + (x + 1)]); // Horizontal link
            if (y < rows - 1)
                integrator.addLink(points[y * cols + x], points[(y + 1) * cols + x]); // Vertical link
        }
    }
}

void initializeRigidStructure(Verlet::VerletIntegrator& integrator)
{

    // Left square
    Verlet::Point* l1 = integrator.addPoint({ 150.f, 100.f }, true);
    Verlet::Point* l2 = integrator.addPoint({ 100.f, 150.f }, false);
    Verlet::Point* l3 = integrator.addPoint({ 150.f, 200.f }, false);
    Verlet::Point* l4 = integrator.addPoint({ 200.f, 150.f }, false);

    integrator.addLink(l1, l2);
    integrator.addLink(l2, l3);
    integrator.addLink(l3, l4);
    integrator.addLink(l4, l1);

    // Right square
    Verlet::Point* r1 = integrator.addPoint({ 300.f, 100.f }, false);
    Verlet::Point* r2 = integrator.addPoint({ 250.f, 150.f }, false);
    Verlet::Point* r3 = integrator.addPoint({ 300.f, 200.f }, false);
    Verlet::Point* r4 = integrator.addPoint({ 350.f, 150.f }, false);

    integrator.addLink(r1, r2);
    integrator.addLink(r2, r3);
    integrator.addLink(r3, r4);
    integrator.addLink(r4, r1);

    // Connect the two squares
    //integrator.addLink(l1, r1);
    //integrator.addLink(l3, r3);
    integrator.addLink(l2, r4);
    integrator.addLink(l4, r2);

    // Middle points
    Verlet::Point* ml = integrator.addPoint({ 150.f, 150.f }, false);
    Verlet::Point* mr = integrator.addPoint({ 300.f, 150.f }, false);

    integrator.addLink(ml, l2);
    integrator.addLink(ml, l4);

    integrator.addLink(mr, r2);
    integrator.addLink(mr, r4);
}

void initializeScene(Verlet::VerletIntegrator& integrator)
{

	initializeRigidStructure(integrator);
	initializeGrid({ 400.f, 50.f }, 10, 10, 40.f, integrator);

    printAllPoints(integrator);
	printAllLinks(integrator);
}


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Verlet Integrator");
    window.setFramerateLimit(144);

	Verlet::VerletIntegrator integrator(1.f / 60);        // 60 FPS, default gravity = 9.81 m/s^2
	initializeScene(integrator);

	Verlet::Renderer renderer(integrator);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        window.clear();


		// drag points with mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            for (Verlet::Point& p : integrator.getPoints())
            {
                if (p.isPinned)
                    continue;
				sf::Vector2f delta = p.position - mousePosF;
                if (delta.lengthSquared() < 200.0f) // if mouse is close to point
                {
                    p.position = mousePosF;
                    p.previousPosition = mousePosF; // uncomment to disable inertia when dragging
                }
            }
		}

		// Right click to remove links near mouse
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
            auto& links = integrator.getLinks();
            links.erase(std::remove_if(links.begin(), links.end(),
                [&mousePosF](const Verlet::Link& link)
                {
					sf::Vector2f midPoint = link.getMidpoint();
                    sf::Vector2f delta = midPoint - mousePosF;
                    return delta.lengthSquared() < 100.0f; // if mouse is close to midpoint of link
                }), links.end());
		}


		// Update physics
		integrator.update(10); // 5 constraint iterations for better stability

		// Render
		renderer.render(window);


        window.display();
    }
}



