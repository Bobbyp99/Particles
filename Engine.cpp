#include "Engine.h"

//
Engine::Engine()
{
	m_Window.create(VideoMode::getDesktopMode(),"Particles");
}

//
void Engine::run()
{
	// Clock object made to track time per frame
	sf::Clock timePassed;

	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	while (m_Window.isOpen())
	{
		float dtAsSeconds = (timePassed.restart()).asSeconds();
		input();
		update(dtAsSeconds);
		draw();
	}
}


//
void Engine::input()
{
	//Handle input
	sf::Event event;

	// Poll Windows queue events
	while (m_Window.pollEvent(event))
	{
		
		// Handle event to close the window
		if (event.type == sf::Event::Closed)
			m_Window.close();

		// Check if Esc is pressed to close the window
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_Window.close();
		}


		if (event.type == sf::Event::MouseButtonPressed)
		{
			// Left click (create particles)
			if (event.mouseButton.button == sf::Mouse::Left)
			{
				// Construct particles
				for (int i = 0; i < PARTICLES_GENERATED; i++)
				{
					m_particles.push_back(Particle(m_Window, (rand() % VERT_RANGE) + VERT_RANGE_LOWER, sf::Vector2i{ event.mouseButton.x, event.mouseButton.y }));
				}
			}
		}

	}
}

//
void Engine::update(float dtAsSeconds)
{
	// Check the life of the particle (TTL)
	for (vector<Particle>::iterator iter = m_particles.begin(); iter != m_particles.end();)
	{
		// If the particle is still has life time, update it and increment the iterator
		if (iter->getTTL() > 0.0)
		{
			iter->update(dtAsSeconds);
			iter++;
		}
		// If the particle's TTL <= 0 then erase that particle from the vector and do not iterate
		// - Not iterating prevents incrementing past the end of the vector
		else
		{
			iter = m_particles.erase(iter);
		}
	}
}

//
void Engine::draw()
{
	m_Window.clear();

	// Draw each particle
	for (int i = 0; i < m_particles.size(); i++)
	{
		m_Window.draw(m_particles[i]);
	}

	m_Window.display();
}

