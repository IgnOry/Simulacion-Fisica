#pragma once
#include "Particle.h"
#include "AttachForceGenerator.h"

class Planet
{
	protected:
		Particle* part;
		AttachForceGenerator* attach;
		float currentAngle;
		float speed;
		float distance;
		float trueTime;
		int rotation;
	public:
		Planet(float radius = 1.0f, Vector4 c = Vector4(0.5, 0.5, 0.5, 1), Vector3 pos = Vector3(0.0f, 0.0f, 0.0f), float speed = 0.1f, int rotation_ = 0);
		~Planet();
		void update(float t);
		AttachForceGenerator* getAttach();
		Particle* getPart();
		Vector3 nextPos(float t);
};
