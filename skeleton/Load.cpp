#include "Load.h"

Load::Load(Vector3 pos)
{
	part = new Particle(1, Vector4(1, 1, 1, 1), pos, 0, 0, 0, 1);
}

Load::~Load()
{
	delete part;
}

Particle* Load::getPart()
{
	return part;
}

float Load::getHealth()
{
	return health;
}

void Load::changeHealth(float value_)
{
	health = health + value_;
}
