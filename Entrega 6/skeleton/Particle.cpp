#include "Particle.h"

Particle::Particle(float r, Vector4 c, Vector3 pos, float time, int count_, int repeat_, int type)
{
	position = pos;
	transform = new PxTransform(position);
	radius = r;
	color = c;
	if (type == 0)
		rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), transform, color);
	else if (type == 1)
		rItem = new RenderItem(CreateShape(PxBoxGeometry(radius, radius, radius)), transform, color);
	lifeTime = time;
	count = count_;
	repeat = repeat_;
	force = Vector3(0, 0, 0);
	setDirVel(Vector3(0, 0, 0), Vector3(0, 0, 0));
	setMass(1.0);
	setDamping(1.0);
}

Particle::Particle(float radius, Vector3 pos, PxScene* scene, PxPhysics* pxphy_, bool dyn, int type)
{
	PxShape* shape;
	if (type == 0)
		shape = CreateShape(PxSphereGeometry(radius));
	else if (type == 1)
		shape = CreateShape(PxBoxGeometry(radius, 1, radius));

	PxTransform* tr = new PxTransform(pos);

	if (dyn)
	{
		din = pxphy_->createRigidDynamic(*tr);
		scene->addActor(*din);
		din->attachShape(*shape);

		PxRigidBodyExt::updateMassAndInertia(*din, 1);


		setMass(1.0);
		setDamping(1.0);
		din->setLinearDamping(1.0);

		Vector3 vel = Vector3(cos(rand() % 360) * 10, 30, sin(rand() % 360) * 10);

		setVelocity(vel);
		din->setLinearVelocity(vel);

		rItem = new RenderItem(shape, din, Vector4(1, 0, 0, 1));
	}

	else
	{ 
		sta = pxphy_->createRigidStatic(*tr);
		scene->addActor(*sta);
		sta->attachShape(*shape);


		setMass(1.0);

		rItem = new RenderItem(shape, sta, Vector4(1,1,1,1));
	}
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

void Particle::setRItem(int shape)
{
	//case 0, esfera
	if (shape == 0)
	{
		rItem->release();
		rItem = new RenderItem(CreateShape(PxSphereGeometry(radius)), transform, color);
	}
	//case 1
	else if (shape == 1)
	{
		rItem->release();
		rItem = new RenderItem(CreateShape(PxBoxGeometry(1,1,1)), transform, Vector4(1.0, 0.9, 0, 1.0));
	}
}

void Particle::setVelocity(Vector3 vel_)
{
	velocity = vel_;
}

void Particle::setAcceleration(Vector3 acc_)
{
	acceleration = acc_;
}

void Particle::PxAddForce(Vector3 f_)
{
	force += f_;
	din->addForce(f_);
}

void Particle::PxClearForce()
{
	force = Vector3(0, 0, 0);
	din->clearForce();
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
	velocity += totalAcc * t;
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

float Particle::getInverseMass()
{
	return inverseMass;
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
	return getMass() <= 0.0f;
}

float Particle::getMass()
{
	return 1.0 / inverseMass;
}

PxRigidDynamic* Particle::getDin()
{
	return din;
}

void Particle::setPosition(Vector3 position_)
{
	position = position_;
}
