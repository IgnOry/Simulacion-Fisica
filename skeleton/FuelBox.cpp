#include "FuelBox.h"

FuelBox::FuelBox(float size, Vector3 pos)
{
	part = new Particle(size, Vector4(1, 1, 1, 1), pos, 0, 0, 0, 1);
	fuel = size * 2000;
}

FuelBox::~FuelBox()
{
	delete part;
}

float FuelBox::getFuel()
{
	return fuel;
}

Particle* FuelBox::getPart()
{
	return part;
}
