#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, float spawnTime)
{
	position = pos;
	transform = new PxTransform(position);
	spawnRateTime = spawnTime;
	rItem = new RenderItem(CreateShape(PxSphereGeometry(0.001f)), transform, color);
	time_ = 0;
}

ParticleGenerator::~ParticleGenerator()
{
	while (!particlesVec.empty()) //Similar al usado en el vector de disparo, solo que se recorre entero en vez de borrar solo el primero
	{
		auto it = particlesVec.begin();
		Particle* p = (*it);
		particlesVec.erase(it);
		delete p;
	}
}

void ParticleGenerator::createParticle(float time)
{
	if (time_ > spawnRateTime) { //para mantener el tiempo entre particula y particula
		float life = (rand() % 10);

		//Nueva particula, y se añade al vector
		Particle* newP = new Particle(0.1, Vector4(1.0, 1.0, 1.0, 1), position, life); //tamaño reducido para que se aprecie mejor
		newP->setDirVel(Vector3(0, -9.8, 0), random()); //-9.8, gravedad "realista"
		particlesVec.push_back(newP);

		time_ = 0;
	}
	time_ += time;
}

Vector3 ParticleGenerator::random() //Para dar un vector alatorio como velocity/direccion de movimiento
{
	int angle = rand() % 360; //*360 salen como si fuera una ducha

	int x = cos(angle) * 10;
	int y = 10; //Asi salen hacia arriba
	int z = sin(angle) * 10;

	Vector3 vec = Vector3(x, y, z);

	return vec;
}

void ParticleGenerator::update(float time) //Crea particula nueva, actualiza el resto y borra si toca
{
	createParticle(time);
	
	auto it = particlesVec.begin();
	while (!particlesVec.empty() && it != particlesVec.end()) 
	{
		Particle* p = (*it);
		p->update(time);

		if (p->deathTime(time)) { //Si se borra, el iterador vuelve al principio
			particlesVec.erase(it);
			delete p;
			it = particlesVec.begin();
		}
		else if (!particlesVec.empty())
			it++;
	}
}
