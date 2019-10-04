#include "Firework.h"

void Firework::clearAccumulator()
{
}

void Firework::setPosition(Vector3 position_)
{
	position = position_;
}

Vector3 Firework::getPosition()
{
	return position;
}

Vector3 Firework::getVelocity()
{
	return velocity;
}

void Firework::Practica1(Vector3 gravity, float lifeTime)
{
	//angulo random
	Particle* p = new Particle(1);
	p->setPosition(getPosition());
	p->setAcceleration(gravity);

	//p->setVelocity();
}
