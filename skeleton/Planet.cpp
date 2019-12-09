#include "Planet.h"

Planet::Planet(float radius, Vector4 c, Vector3 pos, float angle, float speed)
{
	part = new Particle(radius, c, pos);
	attach = new AttachForceGenerator(radius * 3, part);

	distance = (part->getPosition() - Vector3(0, 0, 0)).magnitude();

	std::cout << distance << std::endl;
}

Planet::~Planet()
{
	delete part;
	delete attach;
}

void Planet::update(float t)
{
	Vector3 position = nextPos(t);


	part->setPosition(position);
	attach->getParticle()->setPosition(position);

	Vector3 pos = part->getPosition();

	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

AttachForceGenerator* Planet::getAttach()
{
	return attach;
}

Particle* Planet::getPart()
{
	return part;
}

Vector3 Planet::nextPos(float t)
{
	currentAngle = speed * t;

	Vector3 offsetPos = Vector3(sin(currentAngle), cos(currentAngle), 0) * distance;

	Vector3 end = part->getPosition() + offsetPos;

	return end;
}
