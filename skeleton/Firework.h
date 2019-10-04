#pragma once
#include "Particle.h"
class Firework :	public Particle
{
	//
	//Copiado de las transparencias
	//

	/*struct Payload
	{
		// Type of firework to create
		unsigned type;
		unsigned count;
		void set(unsigned _type, unsigned _count)
		{
			type = _type;
			count = _count;
		}
	};
	struct FireworkRule
	{
		// Type of firework configured
		unsigned type;
		float minAge;
		float maxAge;
		// Minimum relative velocity
		Vector3 minVelocity;
		float damping;
		std::vector<Payload> payloads;

		void create(Firework* firework, const Firework* parent = NULL)
			const
		{
			firework->type = type;
			firework->age = random(minAge, maxAge); //random raro
			Vector3 vel;
			if (parent)
			{ // inherit velocity from parent
				firework->setPosition(parent->getPosition());
				vel += parent->getVelocity();
			}
			else
			{
				Vector3 start;
				int x = random(3) - 1;
				start.x = 5.0f * x;
				firework->setPosition(start);
			}
			vel += random(minVelocity, maxVelocity); //random raro
			firework->setVelocity(vel);
			// Force the mass to 1
			firework->setMass(1);
			firework->setDamping(damping);
			firework->setAcceleration(GRAVITY); //Gravity = Vector3 (0, -10,0);
			firework->clearAccumulator();
		}
	};

	// Type of payload it has (what it will generate)
	unsigned type;
	// Time left for the firework to detonate
	// When age reaches zero, the particle disappears and delivers the payload
	float age;

	FireworkRule rules[];*/

public:
	virtual void setPosition(Vector3 position_);
	virtual Vector3 getPosition();
	virtual Vector3 getVelocity();
	void Practica1(Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), float lifeTime); //??
	std::vector<Particle*> particles;

	//
	//Copiado de las transparencias
	//

	/*void initFireworkRules()
	{
		rules[0].setParameters( //hacer métodos
			1, // type
			0.5f, 1.4f, // age range
			Vector3(-5, 25, -5), Vector3(5, 28, 5), // velocity range
			0.1, // damping
			);
		rules[0].payloads.push_back(
			Payload(3, 5)); //algo
		rules[0].payloads.push_back(
			Payload(5, 5));
	}

	bool update(float t)
	{
		// Update the particle
		integrate(t);
		age -= t;
		return (age < 0);
	}	void FireworksCreate(unsigned type, const Firework* parent)
	{
		FireworkRule* rule = GetRuleFromType(type); //hacer metodo
		Firework* newFirework = AllocNewFirework(); //hacer metodo
		rule->create(newFirework, parent);
	}
	bool isActive() const
	{
		return type != FW_UNKNOWN_TYPE; //como se usa esto xd
	}
	void setInactive()
	{
		type = FW_UNKNOWN_TYPE;
	}

	void FireworksUpdate(float t)
	{
		for (auto it = fireworks.begin(); it != fireworks.end(); ++it) //añadir atributo (?)
		{
			Firework* firework = (*it);
			if (firework->isActive())
			{
				if (firework->update(t))
				{
					FireworkRule* rule = GetRuleFromType(firework->type);
					firework->setInactive();
					for (auto itPlayload = rule->payloads.begin(); itPlayload != rule->payloads.end(); ++itPlayload)
					{
						FireworkRule::Payload* payload = (*itPlayload); //lo mismo
						create(payload->type, payload->count, firework);
					}
				}
			}
		}
	}

	void clearAccumulator();*/
};

