#pragma once

#include "RenderUtils.hpp"
#include <PxPhysicsAPI.h>

using namespace physx;

class Particle
{
public:
	Particle(float radius = 1.0f, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), float time = 1.0f, int count_ = 50, int repeat_ = 0);
	~Particle();
	void integrate(float t);
	void update(float t);
	bool deathTime(float t);

	// Clears accumulated force
	void clearForce();
	// Add force to apply in next integration only
	void addForce(const Vector3& f);

	//Setters
	void setPosition(Vector3 position_);
	void setMass(float mass_);
	void setDamping(float damping_);
	void setLifeTime(float time_);
	void setDirVel(Vector3 acc = Vector3(1.0, 0, 0), Vector3 vel = Vector3(1.0, 0, 0));
	void setRepeat(int repeat_);

	//Getters
	Vector3 getPosition() const;
	Vector3 getVelocity() const;
	int getCount();
	int getRepeat();
	bool hasInfiniteMass();
	float getMass();

protected:
	float damping = 1.0f;
	float radius;
	float inverseMass = 1.0/45.0;
	float lifeTime;

	int repeat;
	int count;

	Vector3 acceleration;
	Vector3 velocity;
	Vector3 position;
	Vector3 force;

	Vector4 color;

	PxTransform* transform;

	RenderItem* rItem;
};
