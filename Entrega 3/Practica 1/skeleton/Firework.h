#pragma once
#include "Particle.h"
#include "ParticleGravity.h"

enum Types { DISABLED_TYPE, FIREWORK_TYPE, WC_TYPE, ASPERSOR_TYPE };

class Firework :	public Particle
{
public:
	struct Payload
	{
		// Type of firework to create
		Payload(unsigned type_, unsigned count_, Vector3 gravity_);
		
		void update(float t);
		void explode();
		unsigned type;
		unsigned count;
		std::vector<Particle*> particles;
		ParticleGravity* gravity;
	};

	struct FireworkRule
	{
		// Type of firework configured
		unsigned type;
		float minAge;
		float maxAge;

		// Minimum relative velocity
		Vector3 minVelocity;
		Vector3 maxVelocity;

		float damping;
		std::vector<Payload*> payloads;

		void setAll(unsigned type_, float minAge_, float maxAge_, Vector3 minVelocity_, Vector3 maxVelocity_);
		void setType(unsigned type_);
		void setMinVelocity(Vector3 maxVelocity_);
		void setMaxVelocity(Vector3 minVelocity_);
		void setMinAge(float minAge_);
		void setMaxAge(float maxAge_);

		void create(Firework* firework, const Firework* parent = NULL)	const;
	};

	// Time left for the firework to detonate
	// When age reaches zero, the particle disappears and delivers the payload

	int rule;
	unsigned fireworkType;
	FireworkRule** rules = new FireworkRule* [2];
	Firework(float age_, Vector3 pos_, Vector3 vel_, int rule_, int type_, int count_, Vector3 gravity_);
	~Firework();
	void initFireworkRules(int type_, int count_, Vector3 gravity_);
	void deleteFirework();
	bool update(float t);
	bool isActive() const;
	void setInactive();
};

