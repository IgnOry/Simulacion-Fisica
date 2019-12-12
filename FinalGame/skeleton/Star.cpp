#include "Star.h"

Star::Star(float radius, Vector4 c, Vector3 pos, float time)
{
	part = new Particle(radius, c, pos, time);
}

Star::~Star()
{
	delete part;
}

void Star::update(float t)
{
	part->update(t);
}

Particle* Star::getPart()
{
	return part;
}

bool Star::explosion(float t)
{	
	if (part->deathTime(t))
		return true;
	else
		return false;
}
