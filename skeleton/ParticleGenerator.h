#pragma once
#include "Particle.h"
#include "ParticleGravity.h"
#include "ParticleForceRegistry.h"
#include "WindGenerator.h"
#include "ExplosionGenerator.h"
#include <vector>

class ParticleGenerator: public Particle
{
public:
	ParticleGenerator(Vector3 pos, float spawnTime, float lifeTime_, Vector3 gravity_, Vector3 windDir = Vector3(0, 0, 0)); //Si no se especifica, la direccion del viento es 0
	~ParticleGenerator();
	void update(float time);
	void createParticle(float time);

	float spawnRateTime;
	float time_;

	Vector4 color = Vector4(0.5, 0.5, 0.5, 1);

	std::vector<Particle*> particlesVec;
	ParticleGravity* gravity;
	ParticleForceRegistry* forcesRegistry;
	WindGenerator* windGenerator;
	ExplosionGenerator* explosionGenerator;
};
