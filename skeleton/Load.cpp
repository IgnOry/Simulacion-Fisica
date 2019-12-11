#include "Load.h"

Load::Load(Vector3 pos, PxScene* scene_, PxPhysics* pxphy_)
{
	part = new Particle(1, pos, Vector4(0, 1, 1, 1), scene_, pxphy_, true, 1);//(1, Vector4(1, 1, 1, 1), pos, 0, 0, 0, 1);
	part->setPosition(part->getDin()->getGlobalPose().p);
}

Load::~Load()
{
	delete part;
}

Particle* Load::getPart()
{
	return part;
}

float Load::getHealth()
{
	return health;
}

void Load::changeHealth(float value_)
{
	health = health + value_;
}
