#include "Particle.h"

//Generate a randomized shape with:
// - numPoints vetrices
// - centered on mouseClickPosition to the Cartesian plane
//	- Cartesian plane centered at (0,0), instead of (width / 2, height / 2)
Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition)
	: m_A(2, numPoints)
{
	m_ttl = TTL;
	m_numPoints = numPoints;
	m_radiansPerSec = static_cast<float>(M_PI * (static_cast<float>(rand()) / RAND_MAX));

	//Tell the particle that its on the CartesianPlane:
	// - Center of the plane will be (0, 0)
	m_cartesianPlane.setCenter(0, 0);
	// - initialize its width and height to the size of the RenderWindow (target) and invert the y-axis so the Cartesian Plane behvaes properly
	m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);

	//storing the particles center cord on the cartesian plane to be where the mouse clicked
	m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane);

	//initial horizontal and vertical velocity of the particle
	m_vx = (static_cast<float>(rand() % 401) + 100) * (rand() % 2 != 0 ? -1 : 1);
	m_vy = static_cast<float>(rand() % 401) + 100;

	//Assign colors                  
	m_color1 = sf::Color::White;
	m_color2 = sf::Color(rand() % 256,rand() % 256,rand() % 256);

	
	// --- begining of generation --- //
	//Generate a particle with numPoint vertices

	// - this is done by sweeping a circular arc with randomized radii
	float theta = (M_PI / 2) * (static_cast<float>(rand()) / RAND_MAX);
	
	// amount roated ver vertex
	// - (numPoints - 1) because the last vertex should overlap with the first so no open edges are left
	float dTheta = (2 * M_PI) / (numPoints - 1);

	// assigning the location of each vertex/point
	int rangeLower = 20, rangeUpper = 80;
	int range = rangeUpper - rangeLower;
	for (int vert = 0; vert < numPoints; vert++)
	{
		float r, dx, dy;
		
		r = ((static_cast<float>(rand()) / RAND_MAX) * range) + rangeLower;

		dx = r * cos(theta);
		dy = r * sin(theta);

		m_A(0, vert) = m_centerCoordinate.x + dx;
		m_A(1, vert) = m_centerCoordinate.y + dy;

		// increment theta by dTheta to move to the next location for the next iteration of the loop
		theta += dTheta;
	}

	// --- end of generation --- //
}

// overrides the virtual function from sf::Drawable, allowing polymorph for this function
void Particle::draw(RenderTarget& target, RenderStates states) const
{
	// creates a vector of triangles that will form the shape
	// - (m_numPoints + 1) is to account for the center
	sf::VertexArray lines(sf::TriangleFan, m_numPoints + 1);
	
	// store the location of the center of the partical on the monitor
	sf::Vector2f center{ target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane) };

	// store position 0 in lines with the center location
	lines[0].position = center;
	// set the color for the center of the Triangle Fan
	lines[0].color = m_color1;
	
	// loop to store the location of each point in lines and assign them a color
	for (int vert = 1; vert < m_numPoints + 1; vert++)
	{
		lines[vert].position = sf::Vector2f{ target.mapCoordsToPixel({static_cast<float>(m_A(0,vert - 1)), static_cast<float>(m_A(1, vert - 1))}, m_cartesianPlane) };
		lines[vert].color = m_color2;
	}
	target.draw(lines);
}


void Particle::update(float dt)
{
	m_ttl -= dt;
	rotate(dt * m_radiansPerSec);
	scale(SCALE - dt);
	
	// movement of the particle affected by gravity
	float dx, dy;
	// horizontal movement
	dx = m_vx * dt;
	// vertical movement
	m_vy -= G * dt;
	dy = m_vy * dt;
	// move the particle
	translate(dx, dy);
}

///rotate Particle by theta radians counter-clockwise
///construct a RotationMatrix R, left mulitply it to m_A
void Particle::rotate(double theta)
{
	// Since the rotation matrix is derived to rotate coordinates about the origin,..
	// ..the particle will temporarily shift to the origin before rotating
	sf::Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

	// Rotate the particle
	RotationMatrix R(theta);
	m_A = R * m_A;

	// Shift back to its original center
	translate(temp.x, temp.y);
}

///Scale the size of the Particle by factor c
///construct a ScalingMatrix S, left multiply it to m_A
void Particle::scale(double c)
{
	// Since the scale matrix scales relative to distance from the origin,..
	// ..the patricle will temporarily shift to the origin before scaling
	sf::Vector2f temp = m_centerCoordinate;
	translate(-m_centerCoordinate.x, -m_centerCoordinate.y);

	// Sclare the particle
	ScalingMatrix S(c);
	m_A = S * m_A;

	// Shift back to its original center
	translate(temp.x, temp.y);
}

///shift the Particle by (xShift, yShift) coordinates
///construct a TranslationMatrix T, add it to m_A
void Particle::translate(double xShift, double yShift)
{
	TranslationMatrix T(xShift, yShift, m_numPoints);
	m_A = T + m_A;

	// update the particle's center coordinate:
	m_centerCoordinate.x += xShift;
	m_centerCoordinate.y += yShift;
}

// ******************************* //
//   --- Particle unit tests ---   //
// ******************************* //

bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}
void Particle::unitTests()
{
	int score = 0;
	cout << "Testing RotationMatrix constructor...";
	double theta = M_PI / 4.0;
	RotationMatrix r(M_PI / 4);
	if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
		&& almostEqual(r(0, 1), -sin(theta))
		&& almostEqual(r(1, 0), sin(theta))
		&& almostEqual(r(1, 1), cos(theta)))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing ScalingMatrix constructor...";
	ScalingMatrix s(1.5);
	if (s.getRows() == 2 && s.getCols() == 2
		&& almostEqual(s(0, 0), 1.5)
		&& almostEqual(s(0, 1), 0)
		&& almostEqual(s(1, 0), 0)
		&& almostEqual(s(1, 1), 1.5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing TranslationMatrix constructor...";
	TranslationMatrix t(5, -5, 3);
	if (t.getRows() == 2 && t.getCols() == 3
		&& almostEqual(t(0, 0), 5)
		&& almostEqual(t(1, 0), -5)
		&& almostEqual(t(0, 1), 5)
		&& almostEqual(t(1, 1), -5)
		&& almostEqual(t(0, 2), 5)
		&& almostEqual(t(1, 2), -5))
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Testing Particles..." << endl;
	cout << "Testing Particle mapping to Cartesian origin..." << endl;
	if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
	{
		cout << "Failed. Expected (0,0). Received: (" << m_centerCoordinate.x <<
			"," << m_centerCoordinate.y << ")" << endl;
	}
	else
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	cout << "Applying one rotation of 90 degrees about the origin..." << endl;
	Matrix initialCoords = m_A;
	rotate(M_PI / 2.0);
	bool rotationPassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1,
			j), initialCoords(0, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
				<< ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			rotationPassed = false;
		}
	}
	if (rotationPassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a scale of 0.5..." << endl;
	initialCoords = m_A;
	scale(0.5);
	bool scalePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
				<< ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			scalePassed = false;
		}
	}
	if (scalePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Applying a translation of (10, 5)..." << endl;
	initialCoords = m_A;
	translate(10, 5);
	bool translatePassed = true;
	for (int j = 0; j < initialCoords.getCols(); j++)
	{
		if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !
			almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
		{
			cout << "Failed mapping: ";
			cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j)
				<< ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
			translatePassed = false;
		}
	}
	if (translatePassed)
	{
		cout << "Passed. +1" << endl;
		score++;
	}
	else
	{
		cout << "Failed." << endl;
	}
	cout << "Score: " << score << " / 7" << endl;
}
