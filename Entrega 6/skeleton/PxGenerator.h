#pragma once
#include "Particle.h"
#include "ParticleForceRegistry.h"
#include "ExplosionGenerator.h"

class PxGenerator: public Particle
{
public:
	PxGenerator(Vector3 pos, PxScene* PhysxScene, PxPhysics* pxPhy); //Si no se especifica, la direccion del viento es 0
	~PxGenerator();
	void update(float time);
	void createParticle(float time);

	float spawnRateTime;
	float time_;

	Vector4 color = Vector4(0.5, 0.5, 0.5, 1);

	std::vector<Particle*> particlesVec;
	ParticleForceRegistry* forcesRegistry;
	ExplosionGenerator* expl;

	int limit = 50;

	PxScene* scene;
	PxPhysics* PxSystem;
};

