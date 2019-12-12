#pragma once
#include "Particle.h"

class Load
{
	protected:
		float health = 100;
		Particle* part;
	public:
		Load(Vector3 pos, PxScene* scene_, PxPhysics* pxphy_);
		~Load();
		Particle* getPart();
		float getHealth();
		void changeHealth(float value_);
};
