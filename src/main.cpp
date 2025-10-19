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


void createRope(Verlet::Point* start, Verlet::Point* end, int segments, Verlet::VerletIntegrator& integrator, float stiffness = 1.0f, float initialTension = 1.f)
{
	// create middle points
	std::vector<Verlet::Point*> points;
	points.push_back(start);
    for (int i = 1; i < segments; ++i)
    {
        float t = static_cast<float>(i) / segments;
        sf::Vector2f pos = (1 - t) * start->position + t * end->position;
        points.push_back(integrator.addPoint(pos, false));
	}
	points.push_back(end);

	// create links
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
        integrator.addLink(points[i], points[i + 1], stiffness, initialTension);
	}
}

void initializeTensegrity(const sf::Vector2f& pos, float size, Verlet::VerletIntegrator& integrator)
{

    // Top structure
    Verlet::Point* topLeft = integrator.addPoint({ pos.x, pos.y }, false);
    Verlet::Point* topRight = integrator.addPoint({ pos.x + size, pos.y}, false);
    Verlet::Point* topMid = integrator.addPoint({ pos.x + size / 2, pos.y}, false);
	Verlet::Point* topRod = integrator.addPoint({ pos.x + size, pos.y + size * 0.66f }, false);
	Verlet::Point* topHook = integrator.addPoint({ pos.x + size / 2, pos.y + size * 0.66f }, false); 

	Verlet::Point* bottomLeft = integrator.addPoint({ pos.x, pos.y + size }, true);
	Verlet::Point* bottomRight = integrator.addPoint({ pos.x + size, pos.y + size }, true);
	Verlet::Point* bottomMid = integrator.addPoint({ pos.x + size / 2, pos.y + size }, false);
	Verlet::Point* bottomRod = integrator.addPoint({ pos.x, pos.y + size * 0.33f }, false);
	Verlet::Point* bottomHook = integrator.addPoint({ pos.x + size / 2, pos.y + size * .33f }, false);
    
    // Links for top structure
    integrator.addLink(topLeft, topMid);
    integrator.addLink(topMid, topRight);
	integrator.addLink(topRight, topLeft);
	integrator.addLink(topRight, topRod);
	integrator.addLink(topRod, topMid);
	integrator.addLink(topRight, topHook);
	integrator.addLink(topRod, topHook);
    

	// Links for bottom structure
	integrator.addLink(bottomLeft, bottomMid);
	integrator.addLink(bottomMid, bottomRight);
	integrator.addLink(bottomRight, bottomLeft);
	integrator.addLink(bottomLeft, bottomRod);
	integrator.addLink(bottomRod, bottomMid);
	integrator.addLink(bottomLeft, bottomHook);
	integrator.addLink(bottomRod, bottomHook);

	// Vertical links
	createRope(topLeft, bottomLeft, 5, integrator, 1.f, 1.2f);
	createRope(topRight, bottomRight, 5, integrator, 1.f, 1.2f);
	createRope(topHook, bottomHook, 5, integrator, 1.f);

}

void initializeRigid2(Verlet::VerletIntegrator& integrator)
{
    
}

void initializeScene(Verlet::VerletIntegrator& integrator)
{

	//initializeRigidStructure(integrator);
	//initializeGrid({ 400.f, 50.f }, 10, 10, 40.f, integrator);
	
    //initializeGrid({ 10, 10 }, 50, 50, 10.f, integrator);

	initializeTensegrity({ 400.f, 50.f }, 100.f, integrator); 


	// Rope
	Verlet::Point* ropeStart = integrator.addPoint({ 600.f, 50.f }, true);
	Verlet::Point* ropeEnd = integrator.addPoint({ 600.f, 300.f }, false);
	createRope(ropeStart, ropeEnd, 20, integrator, 0.5f, 0.9f);

 //   printAllPoints(integrator);
	//printAllLinks(integrator);
}


int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({800u, 600u}), "Verlet Integrator");
    window.setFramerateLimit(144);

	Verlet::VerletIntegrator integrator(1.f / 60, {0.f, 9.81f}, 0.999f, 100000);        // 60 FPS, default gravity = 9.81 m/s^2
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
		static bool wasLeftPressed = false;
		static Verlet::Point* selectedPoint = nullptr;
        if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
        {
            sf::Vector2i mousePos = sf::Mouse::getPosition(window);
            sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));

			if (!wasLeftPressed) { // find ONE close point only once when the mouse button is first pressed
				wasLeftPressed = true;

                for (Verlet::Point& p : integrator.getPoints())
                {
                    if (p.isPinned)
                        continue;
                    sf::Vector2f delta = p.position - mousePosF;
                    if (delta.lengthSquared() < 200.0f) // if mouse is close to point
                    {
						selectedPoint = &p;
                        p.position = mousePosF;
                        //p.previousPosition = mousePosF; // uncomment to disable inertia when dragging
                        break; // only drag one point at a time
                    }
                }


            }
            else {
                if (selectedPoint) {
                    selectedPoint->position = mousePosF;
                    //selectedPoint->previousPosition = mousePosF; // uncomment to disable inertia when dragging
                }
            }

        }
        else {
			wasLeftPressed = false;
			selectedPoint = nullptr;
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

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
        {
            integrator.clearScene();
            initializeScene(integrator);
		}


		// Update physics
		integrator.update(10); // 5 constraint iterations for better stability

		// Render
		renderer.render(window);


        window.display();
    }
}



