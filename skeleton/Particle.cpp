#include "Particle.h"

Particle::Particle(float r, Vector4 c, Vector3 pos)
{
	position = pos;
	transform = new PxTransform(position);
	radius = r;
	color = c;
	rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), transform, color);
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
