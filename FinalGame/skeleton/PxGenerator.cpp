#include "PxGenerator.h"

PxGenerator::PxGenerator(Vector3 pos, PxScene* PhysxScene, PxPhysics* pxPhy)
{
	forcesRegistry = new ParticleForceRegistry();
	scene = PhysxScene;
	PxSystem = pxPhy;
	spawnRateTime = 0.05;
	time_ = 0;
	part = new Particle(1.0, Vector4(1, 0, 0, 0), pos, 5);
}

PxGenerator::~PxGenerator()
{
	auto auxP = particlesVec.begin();

	while (!particlesVec.empty() && auxP != particlesVec.end()) {
		Particle* p = (*auxP);

		particlesVec.erase(particlesVec.begin());
		
		scene->removeActor(*p->getDin()); //Ya que da error al borrar, al menos que no colapsen las fisicas

		p->getDin()->setGlobalPose(PxTransform(Vector3(0, -10000000, 0))); //Esto est� aqu� al lado

		auxP = particlesVec.begin();
		
		if (!particlesVec.empty())
			auxP++;
	}

	delete part;
}

void PxGenerator::update(float time)
{
	if (particlesVec.size() < limit && time_ > spawnRateTime)
		createParticle();

	time_ += time;
}

void PxGenerator::createParticle()
{
	Particle* p = new Particle(1, part->getPosition(), Vector4(1.0, 1.0, 1.0, 1.0), scene, PxSystem, true, 0);
	particlesVec.push_back(p);
	time_ = 0;
}

Particle* PxGenerator::getPart()
{
	return part;
}