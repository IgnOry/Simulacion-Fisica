#include "ParticleForceGenerator.h"

float ParticleForceGenerator::distance(Vector3 p1, Vector3 p2)
{
	float x, y, z;

	x = (p1.x - p2.x) * (p1.x - p2.x);

	y = (p1.y - p2.y) * (p1.y - p2.y);

	z = (p1.z - p2.z) * (p1.z - p2.z);

	return sqrt(x+y+z);
}
