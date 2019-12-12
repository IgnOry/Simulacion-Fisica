#include "Player.h"

Player::Player(PxScene* scene_, PxPhysics* pxphy_)
{
	falcon = new Particle(5, Vector3(0, 0, 0), Vector4(1, 1, 1, 1), scene_, pxphy_, true, 0);//(10, Vector4(0.3, 0.3, 0.3, 1), Vector3(0, 0, 0), 0.0, 0, 0, 0);
	pos = Vector3(0, 0, 0);
	falcon->getDin()->setLinearVelocity(Vector3(0, 0, 0));
	falcon->setVelocity(Vector3(0, 0, 0));
}

void Player::update(float t)
{
	if (!trapped)
		falcon->getDin()->setLinearVelocity(Vector3(100 * throtle, 50 * topdown, 50 * leftright));
	
	falcon->setPosition(falcon->getDin()->getGlobalPose().p);

	Vector3 dist = falcon->getDin()->getGlobalPose().p - pos;
	float d = dist.magnitude();
	changeFuel(-d/100);

	pos = falcon->getDin()->getGlobalPose().p;
}

void Player::handleEvent(char c)
{
	if (!trapped)
	{
		switch (toupper(c))
		{
		case 'W':
		{
			if (throtle < 1.0)
			{
				throtle = throtle + 0.05;
				std::cout << "Dale gas " << throtle << " " << std::endl;
			}
			//falcon->getDin()->setLinearVelocity(Vector3(5, 0, 0));
			break;
		}
		case 'A':
		{
			if (leftright > -1.0)
			{
				leftright = leftright - 0.05;
				std::cout << "Dale gas " << throtle << " " << std::endl;
			}
			//falcon->getDin()->setLinearVelocity(Vector3(5, 0, 0));
			break;
		}
		case 'S':
		{
			if (throtle > -1.0)
			{
				throtle = throtle - 0.05;
				std::cout << "No le des gas " << throtle << " " << std::endl;
			}
			break;
		}
		case 'D':
		{
			if (leftright < 1.0)
			{
				leftright = leftright + 0.05;
				std::cout << "No le des gas " << throtle << " " << std::endl;
			}
			break;
		}
		case 'I':
		{
			if (topdown < 1.0)
			{
				topdown = topdown + 0.05;
				std::cout << "No le des gas " << throtle << " " << std::endl;
			}
			break;
		}
		case 'K':
		{
			if (topdown > -1.0)
			{
				topdown = topdown - 0.05;
				std::cout << "Dale gas " << throtle << " " << std::endl;
			}
			//falcon->getDin()->setLinearVelocity(Vector3(5, 0, 0));
			break;
		}
		case 'X':
		{
			topdown = 0;
			leftright = 0;
			throtle = 0;
			break;
		}
		default:
			break;
		}
	}
}

Player::~Player()
{
	delete falcon;
}

Particle* Player::getParticle()
{
	return falcon;
}

float Player::getHealth()
{
	return health;
}

void Player::changeHealth(float value_)
{
	health = health + value_;
}

float Player::getFuel()
{
	return fuel;
}

void Player::changeFuel(float value_)
{
	fuel = fuel + value_;
}

Vector3 Player::getAcc()
{
	Vector3 ret = Vector3(leftright, topdown, throtle); 

	return ret;
}

void Player::setTrapped()
{
	trapped = true;
}
