#include "Particle.h"

Particle::Particle(float r, Vector4 c, Vector3 pos, float time, int count_, int repeat_)
{
	position = pos;
	transform = new PxTransform(position);
	radius = r;
	color = c;
	rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), transform, color);
	lifeTime = time;
	count = count_;
	repeat = repeat_;
	force = Vector3(0, 0, 0);
}

Particle::~Particle()
{
	delete transform;
	rItem->release();
}

void Particle::setDirVel(Vector3 acc, Vector3 vel)
{
	acceleration = acc;
	velocity = vel;
}

void Particle::setRepeat(int repeat_)
{
	repeat = repeat_;
}

void Particle::integrate(float t)
{
	// Trivial case, infinite mass --> do nothing
	if (hasInfiniteMass()) return;

	// Update position
	position += velocity * t;
	*transform = PxTransform(position);
	Vector3 totalAcc = acceleration;
	totalAcc += force * inverseMass;

	//Update linear velocity
	velocity += totalAcc * t; //esto lo rompe visualmente
	//velocity += acceleration * t;

	//Impose drag (damping)
	velocity *= powf(damping, t);

	clearForce();
}

void Particle::setLifeTime(float time_)
{
	lifeTime = time_;
}

void Particle::update(float t) //devuelve true si debe morir
{
	integrate(t);
}

bool Particle::deathTime(float t)
{
	lifeTime -= t;
	
	if (lifeTime < 0)
		return true;
	else
		return false;
}

void Particle::clearForce()
{
	force = Vector3(0, 0, 0);
}

void Particle::addForce(const Vector3& f)
{
	force += f;
}

void Particle::setMass(float mass_)
{
	inverseMass = 1.0/mass_;
}

void Particle::setDamping(float damping_)
{
	damping = damping_;
}

Vector3 Particle::getPosition() const
{
	return position;
}

Vector3 Particle::getVelocity() const
{
	return velocity;
}

int Particle::getCount()
{
	return count;
}

int Particle::getRepeat()
{
	return repeat;
}

bool Particle::hasInfiniteMass()
{
	return inverseMass <= 0.0f;
}

float Particle::getMass()
{
	return 1.0 / inverseMass;
}

void Particle::setPosition(Vector3 position_)
{
	position = position_;
}
