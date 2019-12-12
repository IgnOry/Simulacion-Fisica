#pragma once
#include "Particle.h"

class FuelBox
{
	protected: 
		Particle* part;
		float fuel;
	public:
		FuelBox(float size, Vector3 pos);
		~FuelBox();
		float getFuel();
		Particle* getPart();
};

