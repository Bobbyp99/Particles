#pragma once
#include <SFML/Graphics.hpp>
#include "Particle.h"
using namespace sf;
using namespace std;

// Number of particles generated per click
const int PARTICLES_GENERATED = 4;
// Range of points / verts each particle can have
const int VERT_RANGE_UPPER = 50, 
		  VERT_RANGE_LOWER = 20, 
		  VERT_RANGE = VERT_RANGE_UPPER - VERT_RANGE_LOWER;

class Engine
{
private:
	// A regular RenderWindow
	RenderWindow m_Window;
	//vector for Particles
	vector<Particle> m_particles;
	// Private functions for internal use only
	void input();
	void update(float dtAsSeconds);
	void draw();
public:
	// The Engine constructor
	Engine();
	// Run will call all the private functions
	void run();
};
