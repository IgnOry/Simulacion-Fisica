#pragma once
#include "Particle.h"
#include <vector>

class ParticleGenerator: public Particle
{
public:
	ParticleGenerator(Vector3 pos, float spawnTime, float lifeTime_);
	~ParticleGenerator();
	void update(float time);
	void createParticle(float time);

	float spawnRateTime;
	float time_;

	Vector4 color = Vector4(0.5, 0.5, 0.5, 1);

	std::vector<Particle*> particlesVec;
};
