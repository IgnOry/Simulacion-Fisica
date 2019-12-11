#include "Player.h"

Player::Player(Camera* c)
{
	falcon = new Particle(10, Vector4(0.3, 0.3, 0.3, 1), Vector3(0, 0, 0), 0.0, 0, 0, 0);
	cam_ = c;
}

void Player::update(float t)
{
	falcon->update(t);

	auto it = municion.begin();

	while (!municion.empty() && it != municion.end()) {
		Particle* p = (*it);

		if (p != nullptr)
		{
			p->update(t);

			if (p->deathTime(t)) {
				municion.erase(municion.begin());
				delete p;
				it = municion.begin();
			}

			else if (!municion.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				it++;
		}
	}
}

void Player::handleEvent(char c)
{
	switch (toupper(c))
	{
	case 'W':
	{
		falcon->setVelocity(Vector3(5, 0, 0));
		break;
	}
	case 'A':
	{
		falcon->setVelocity(Vector3(0, 0, -5));
		std::cout << "a" << std::endl;
		break;
	}
	case 'S':
	{
		falcon->setVelocity(Vector3(-5, 0, 0));
		break;
	}
	case 'D':
	{
		falcon->setVelocity(Vector3(0, 0, 5));
		break;
	}
	case 'X':
	{
		falcon->setVelocity(Vector3(0, 0, 0));
		break;
	}
	default:
		break;
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

bool Player::shoot()
{
	if (municion.size() < maxBullets_)
	{
		Particle* p1 = new Particle(1, Vector4(0.0, 1.0, 0.0, 1.0), falcon->getPosition(), 3.0, 0, 0, 0);
		p1->setDirVel(Vector3(0, 0, 0), cam_->getDir() * 100);

		municion.push_back(p1);

		std::cout << "eee" << std::endl;

		return true;
	}
	else
		return false;
}

std::vector<Particle*> Player::getMunicion()
{
	return municion;
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
