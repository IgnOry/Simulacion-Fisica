#include "Planet.h"

Planet::Planet(float radius, Vector4 c, Vector3 pos, float speed_, int rotation_)
{
	part = new Particle(radius, c, pos);
	attach = new AttachForceGenerator(radius * 3, part);

	distance = (part->getPosition() - Vector3(0, 0, 0)).magnitude();

	trueTime = 0;
	speed = speed_;

	rotation = rotation_;

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

//	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
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
	trueTime += t;

	currentAngle = speed * trueTime;

	std::cout << currentAngle << std::endl;

	Vector3 offsetPos;

	switch (rotation)
	{
	case 0:
	{
		offsetPos = Vector3(sin(currentAngle), cos(currentAngle), 0) * distance;
		break;
	}
	case 1:
	{
		offsetPos = Vector3(sin(currentAngle), 0, cos(currentAngle)) * distance;
		break;
	}
	case 2:
	{
		offsetPos = Vector3(cos(currentAngle), 0, sin(currentAngle)) * distance;
		break;
	}
	case 3:
	{
		offsetPos = Vector3(cos(currentAngle), sin(currentAngle), 0) * distance;
		break;
	}
	case 4:
	{
		offsetPos = Vector3(0, cos(currentAngle), sin(currentAngle)) * distance;
		break;
	}
	case 5:
	{
		offsetPos = Vector3(0, sin(currentAngle), cos(currentAngle)) * distance;
		break;
	}
	default:
		break;
	}
	//std::cout << offsetPos.x << " " << offsetPos.y << " " << offsetPos.z << std::endl;

	Vector3 end = part->getPosition() + offsetPos;

	return end;
}
