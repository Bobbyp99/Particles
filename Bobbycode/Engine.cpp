#include "Engine.h"
Engine::Engine() : m_Window(VideoMode(VideoMode::getDesktopMode().width / 2, VideoMode::getDesktopMode().height / 2), "PARTICLES", Style::Default) {}

void Engine::run()
{
	Clock clock;
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

    while (m_Window.isOpen())
    {
	    Time dt = clock.restart();
	    float dtAsSeconds = dt.asSeconds();
	    input();
	    update(dtAsSeconds);
	    draw();
    }
}

void Engine::input()
{
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)  //if event is closed, then execute close command.
		{
          	    m_Window.close();
       	        }
     	        if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
		    m_Window.close();
		}
		if (event.mouseButton.button == sf::Mouse::Left) 
		{
			cout << "the left mouse button was pressed" << endl;
			cout << "mouse x: " << event.mouseButton.x << endl;
			cout << "mouse y: " << event.mouseButton.y << endl;

		    for (int i = 0; i < 5; ++i)
		    {			
		    	int numPoints = rand() % (50 - 25 + 1) + 25;
		    	Vector2i mouseClickPosition = Vector2i(event.mouseButton.x, event.mouseButton.y);
		    	Particle pDisplay(m_Window, numPoints, mouseClickPosition);
		    	m_particles.push_back(pDisplay);
		    }
	    }
       }
}
void Engine::update(float dtAsSeconds)
{
    for (auto i = m_particles.begin(); i != m_particles.end();)
    {
       if (i->getTTL() > 0.0) // TTL expired check
       {
	   i->update(dtAsSeconds);
           i++;
       }
       else
       {
           i = m_particles.erase(i);
       }
    }
}

void Engine::draw()
{
    m_Window.clear();
    m_Window.draw(backgroundSprite);
    for(const Particle& particle : m_particles)
    {
        m_Window.draw(particle);
    }
    m_Window.display();
}
