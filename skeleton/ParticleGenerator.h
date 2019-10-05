#pragma once
#include "Particle.h"
#include <vector>

class ParticleGenerator
{
public:
	ParticleGenerator(Vector3 pos, float spawnTime);
	~ParticleGenerator();
	void update(float time);
	void createParticle(float time);
	Vector3 random();

	float spawnRateTime;
	float time_;

	Vector3 position;
	Vector4 color = Vector4(0.5, 0.5, 0.5, 1);
	PxTransform* transform;
	RenderItem* rItem;

	std::vector<Particle*> particlesVec;
};
