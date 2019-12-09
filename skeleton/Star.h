#pragma once
#include "Particle.h"
#include "AttachForceGenerator.h"

class Star
{
	protected:
		Particle* part;
		AttachForceGenerator* attach;

	public:
		Star(float radius = 1.0f, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), float time = 1.0f);
		~Star();
		void update(float t);
		AttachForceGenerator* getAttach();
		Particle* getPart();
		bool explosion(float t);
};

