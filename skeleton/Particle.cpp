#include "Particle.h"

Particle::Particle(float r, Vector4 c, Vector3 pos)
{
	position = pos;
	transform = new PxTransform(position);
	radius = r;
	color = c;
	rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), transform, color);
}

Particle::~Particle()
{
	delete transform;
	rItem->release();
}

void Particle::setDirVel(Vector3 acc, Vector3 vel)
{
	acceleration = acc;
	velocity = vel;
}

void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	if (inverseMass <= 0.0f) return;

	// Update position
	position += velocity * t;
	*transform = PxTransform(position);

	//Update linear velocity
	velocity += acceleration * t;

	//Impose drag (damping)
	velocity *= powf(damping, t);
}

void Particle::setLifeTime(float time_)
{
	lifeTime = time_;
}

void Particle::setVelocity(Vector3 velocity_)
{
	velocity = velocity_;
}

void Particle::setMass(float mass_)
{
	inverseMass = -mass_;
}

void Particle::setDamping(float damping_)
{
	damping = damping_;
}

void Particle::setAcceleration(Vector3 acceleration_)
{
	acceleration = acceleration_;
}

Vector3 Particle::getPosition()
{
	return position;
}

Vector3 Particle::getVelocity()
{
	return velocity;
}

void Particle::setPosition(Vector3 position_)
{
	position = position_;
}
