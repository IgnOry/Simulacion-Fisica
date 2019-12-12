#include "Firework.h"
#include <iostream>

Firework::Firework(float age_, Vector3 pos_, Vector3 vel_, int rule_, int type_, int count_, Vector3 gravity_): Particle(0.01f, color, pos_, age_)
{
	setDirVel(Vector3(0, 0, 0), vel_);
	rule = rule_;
	rules[rule] = new FireworkRule();
	initFireworkRules(type_, count_, gravity_);
	fireworkType = type_;
}

Firework::~Firework()
{
	auto it = rules[rule]->payloads.begin();
	while (!rules[rule]->payloads.empty() && it != rules[rule]->payloads.end()) {
		Payload* pL = (*it);

		if (pL->particles.empty()) {
			rules[rule]->payloads.erase(rules[rule]->payloads.begin());
			it = rules[rule]->payloads.begin();
		}
		else
		{
			while (!pL->particles.empty())
			{
				Particle* p = (*pL->particles.begin());

				pL->particles.erase(pL->particles.begin());
				delete p;
			}
		}

		if (!rules[rule]->payloads.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			it++;
	}
		delete rules[rule];
}

void Firework::initFireworkRules(int type_, int count_, Vector3 gravity_)
{
	//rules[0] = new FireworkRule();
	rules[rule]->setAll(type_, 1.0f, 5.0f, Vector3(-10, 30, -10), Vector3(10, 50, 10));
	rules[rule]->payloads.push_back(new Payload(type_, count_, gravity_));
}

bool Firework::update(float t)
{
	if (isActive())
	{
		integrate(t);


		lifeTime -= t;

		if (deathTime(t))
		{
			setInactive();
			for (Payload* payload : rules[rule]->payloads)
			{
				payload->explode();
			}
		}
	}
	else
	{
		auto it = rules[rule]->payloads.begin();
		while (!rules[rule]->payloads.empty() && it != rules[rule]->payloads.end()) {
			Payload* pL = (*it);
			pL->update(t);

			if (pL->particles.empty()) {
				rules[rule]->payloads.erase(rules[rule]->payloads.begin());
				it = rules[rule]->payloads.begin();
			}

			else if (!rules[rule]->payloads.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				it++;
		}
	}

	return (rules[rule]->payloads.empty());
}


bool Firework::isActive() const
{
	return fireworkType != DISABLED_TYPE;
}

void Firework::setInactive()
{
	fireworkType = DISABLED_TYPE;
}

void Firework::FireworkRule::setAll(unsigned type_, float minAge_, float maxAge_, Vector3 minVelocity_, Vector3 maxVelocity_)
{
	setType(type_);
	setMinVelocity(maxVelocity_);
	setMaxVelocity(minVelocity_);
	setMinAge(minAge_);
	setMaxAge(maxAge_);
}

void Firework::FireworkRule::setType(unsigned type_)
{
	type = type_;
}

void Firework::FireworkRule::setMinVelocity(Vector3 minVelocity_)
{
	minVelocity = minVelocity_;
}

void Firework::FireworkRule::setMaxVelocity(Vector3 maxVelocity_)
{
	maxVelocity = maxVelocity_;
}

void Firework::FireworkRule::setMinAge(float minAge_)
{
	minAge = minAge_;
}

void Firework::FireworkRule::setMaxAge(float maxAge_)
{
	maxAge = maxAge_;
}

void Firework::FireworkRule::create(Firework* firework, const Firework* parent) const
{
	firework->fireworkType = type;
	firework->setLifeTime(((rand() % (int)(maxAge - minAge)) + (int)(minAge)));
	Vector3 vel;
	if (parent)
	{ // inherit velocity from parent
		firework->setPosition(parent->getPosition());
		vel += parent->getVelocity();
	}
	else
	{
		Vector3 start;
		int x = (rand() % 3) - 1;
		start.x = 5.0f * x;
		firework->setPosition(start);
	}
	vel = Vector3(((rand() % (int)(minVelocity.x - maxVelocity.x)) + minVelocity.x, (rand() % (int)(minVelocity.y - maxVelocity.y)) + minVelocity.y, (rand() % (int)(minVelocity.z - maxVelocity.z)) + minVelocity.z));
	firework->setDirVel(Vector3(0, -9.8, 0), (vel));

	// Force the mass to 1
	firework->setMass(1);
	firework->setDamping(damping);
}

Firework::Payload::Payload(unsigned type_, unsigned count_, Vector3 gravity_)
{
	type = type_;
	count = count_;
	gravity = new ParticleGravity(gravity_);
}

void Firework::Payload::update(float t)
{
	auto it = particles.begin();
	while (!particles.empty() && it != particles.end()) {
		Particle* p = (*it);

		switch (type)
		{
		case (FIREWORK_TYPE):
			for (int i = 0; i < count; i++)
			{
				p->addForce(Vector3(0, -9.8, 0));
			}
			break;
		case (WC_TYPE):
			for (int i = 0; i < count; i++)
			{
				p->addForce(Vector3(0, 3, 0));
			}
			break;
		case (ASPERSOR_TYPE):
			for (int i = 0; i < count; i++)
			{
				p->addForce(Vector3(0, 4.5, 0));
			}
		}

		gravity->updateForce(p, t);

		p->update(t);

		if (p->deathTime(t)) {
			if (type == FIREWORK_TYPE)
			{
				if (p->getRepeat() > 0)
				{
					for (int i = 0; i < p->getCount(); i++)
					{
						Particle* newP = new Particle(1.0f, Vector4(0.0, 1.0, 0.0, 1.0), Vector3(p->getPosition().x, p->getPosition().y, p->getPosition().z), 1.0f, 5, (p->getRepeat() - 1));
						newP->setDirVel(Vector3(0, -9.8, 0), Vector3(cos(rand() % 360) * 10, 3, sin(rand() % 360) * 10));
						newP->setMass(1.0f);
						newP->setDamping(1.0f);
						particles.push_back(newP);
					}
				}
			}
				particles.erase(particles.begin());
				delete p;
				it = particles.begin();
		}

		else if (!particles.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			it++;
	}
}

void Firework::Payload::explode()
{
	switch (type)
	{
	case (FIREWORK_TYPE):
		for (int i = 0; i < count; i++)
		{
			Particle* p = new Particle(1.0f, Vector4(0.0, 1.0, 0.0, 1.0), Vector3(i * 0.1, i * 0.1, i * 0.1), 1.0f, 5, 3);
			p->setDirVel(Vector3(0, -9.8, 0), Vector3(cos(rand() % 360) * 10, 10, sin(rand() % 360) * 10));
			p->setMass(1.0f);
			p->setDamping(1.0f);
			particles.push_back(p);
		}
		break;
	case (WC_TYPE):
		for (int i = 0; i < count; i++)
		{
			Particle* p = new Particle(0.5f, Vector4(0.0, 1.0, 0.0, 1.0), Vector3(i * 0.5, i * 0.5, i * 0.5), 5.0f);
			p->setDirVel(Vector3(0, 9.8, 0), Vector3(0, 10, 0));
			p->setMass(1.0f);
			p->setDamping(1.0f);
			particles.push_back(p);
		}
		break;
	case (ASPERSOR_TYPE):
		for (int i = 0; i < count; i++)
		{
			Particle* p = new Particle(1.0f, Vector4(0.0, 1.0, 0.0, 1.0), Vector3(i * 0.1, i * 0.1, i * 0.1), 5.0f);
			p->setDirVel(Vector3(0, -9.8, 0), Vector3(cos(rand() % 360) * 10, 10, sin(rand() % 360) * 10));
			p->setMass(1.0f);
			p->setDamping(1.0f);
			particles.push_back(p);
		}
		break;
	}
}