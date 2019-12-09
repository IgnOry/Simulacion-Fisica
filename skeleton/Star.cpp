#include "Star.h"

Star::Star(float radius, Vector4 c, Vector3 pos, float time)
{
	part = new Particle(radius, c, pos, time);
	attach = new AttachForceGenerator(radius * 3, part, time);
}

Star::~Star()
{
	delete part;
	delete attach;
}

void Star::update(float t)
{
	part->update(t);
}

AttachForceGenerator* Star::getAttach()
{
	return attach;
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
