#pragma once
#include "ParticleForceGenerator.h"
#include "Player.h"
#include <iostream>

class AttachForceGenerator: public ParticleForceGenerator
{
	protected:
		Vector3 pos;
		float radio;
		Particle* particle;
		Particle* planet;
		float force;

	public:
		AttachForceGenerator(float force_, Particle* planet_, float time_ = 0);
		~AttachForceGenerator();
		virtual void updateForce(Particle* particleEx, float t);
		virtual void PxUpdateForce(Player* particleEx, float t);
		Particle* getParticle();
};

