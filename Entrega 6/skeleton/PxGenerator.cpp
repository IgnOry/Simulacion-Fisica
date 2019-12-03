#include "PxGenerator.h"

PxGenerator::PxGenerator(Vector3 pos, PxScene* PhysxScene, PxPhysics* pxPhy): Particle(1.0, Vector4(1,1,1,1), pos, 50)
{
	forcesRegistry = new ParticleForceRegistry();
	expl = new ExplosionGenerator(Vector3(3, 20, 3), 10);
	scene = PhysxScene;
	PxSystem = pxPhy;
	spawnRateTime = 1;
	time_ = 0;
}

PxGenerator::~PxGenerator()
{
}

void PxGenerator::update(float time)
{
	if (particlesVec.size() < limit && time_ > spawnRateTime)
	createParticle(time);

	auto it = particlesVec.begin();
	while (!particlesVec.empty() && it != particlesVec.end())
	{
		Particle* p = (*it);

		forcesRegistry->PxUpdateForces(time);

		it++;
	}

	time_ += time;
}

void PxGenerator::createParticle(float time)
{
	Particle* p = new Particle(0.5, position, scene, PxSystem, true, 0);
	particlesVec.push_back(p);
	forcesRegistry->add(p, expl);
	time_ = 0;
}
