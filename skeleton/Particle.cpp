#include "Particle.h"

Particle::Particle(float radius, Vector4 c, Vector3 position)
{
	PxTransform* t = new PxTransform(position);
	rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), t, c);
}
