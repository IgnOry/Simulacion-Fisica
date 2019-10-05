#pragma once

#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

class Particle
{
public:
	Particle(float radius = 1.0f, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), float time = 1.0f);
	~Particle();
	void integrate(float t);
	void update(float t);
	bool deathTime(float t);
	//Setters
	void setPosition(Vector3 position_);
	virtual void setMass(float mass_);
	virtual void setDamping(float damping_);
	void setLifeTime(float time_);
	void setDirVel(Vector3 acc = Vector3(1.0, 0, 0), Vector3 vel = Vector3(1.0, 0, 0));

	//Getters
	virtual Vector3 getPosition();
	virtual Vector3 getVelocity();

protected:
	float damping = 1.0f;
	float radius;
	float inverseMass = 45.0f;
	float lifeTime;

	Vector3 acceleration;
	Vector3 velocity;
	Vector3 position;

	Vector4 color;

	PxTransform* transform;

	RenderItem* rItem;
};

