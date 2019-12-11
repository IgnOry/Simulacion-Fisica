#pragma once
#include "ParticleSpring.h"
class Ship
{
	protected:
		ParticleSpring* spring;
		Particle* part;
		Particle* part2;
		Particle* part3;
		Particle* part4;
	public:
		Ship(Vector3 pos);
		~Ship();
		void update(float t);
		ParticleSpring* getSpring();
		Particle* get1();
		Particle* get2();
		Particle* get3();
		Particle* get4();
};

