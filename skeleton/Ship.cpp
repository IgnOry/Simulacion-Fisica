#include "Ship.h"

Ship::Ship(Vector3 pos)
{
	spring = new ParticleSpring(10, 5, pos);
	Vector3 pos2 = pos + Vector3(5, 5, 5);
	part = new Particle(1, Vector4(1, 1, 1, 1), pos2, 0, 0, 0, 0);

	Vector3 pos3 = pos + Vector3(-5, -5, -5);

	part2 = new Particle(1, Vector4(1, 1, 1, 1), pos3, 0, 0, 0, 0);

	Vector3 pos4 = pos + Vector3(5, -5, 5);

	part3 = new Particle(1, Vector4(1, 1, 1, 1), pos4, 0, 0, 0, 0);

	Vector3 pos5 = pos + Vector3(-5, 5, -5);

	part4 = new Particle(1, Vector4(1, 1, 1, 1), pos5, 0, 0, 0, 0);
	
	part->setVelocity(Vector3(0, 1, 1));
	part2->setVelocity(Vector3(1, 1, 0));
	part3->setVelocity(Vector3(-1, 1, 0));
	part4->setVelocity(Vector3(0, 1, -1));
}

Ship::~Ship()
{
	delete spring;
	delete part;
	delete part2;
	delete part3;
	delete part4;
}

void Ship::update(float t)
{
	part->update(t);
	part2->update(t);
	part3->update(t);
	part4->update(t);
	spring->updateForce(part, t);
	spring->updateForce(part2, t);
	spring->updateForce(part3, t);
	spring->updateForce(part4, t);
	spring->getOther()->update(t);
}

ParticleSpring* Ship::getSpring()
{
	return spring;
}

Particle* Ship::get1()
{
	return part;
}

Particle* Ship::get2()
{
	return part2;
}

Particle* Ship::get3()
{
	return part3;
}

Particle* Ship::get4()
{
	return part4;
}
