#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Vector3 pos, float spawnTime, float lifeTime_): Particle(0.01f,color,pos,lifeTime_)
{
	spawnRateTime = spawnTime;
	time_ = 0;
}

ParticleGenerator::~ParticleGenerator()
{
	while (particlesVec.begin() != particlesVec.end())
	{
		Particle* p = (*particlesVec.begin());
		particlesVec.erase(particlesVec.begin());
		delete p;
	}
}

void ParticleGenerator::createParticle(float time)
{
	if (time_ > spawnRateTime) { //para mantener el tiempo entre particula y particula
		float life = (rand() % 10);

		//Nueva particula, y se añade al vector
		Particle* newP = new Particle(0.1, Vector4(1.0, 1.0, 1.0, 1), position, life); //tamaño reducido para que se aprecie mejor
		newP->setDirVel(Vector3(0, -9.8, 0), Vector3(cos(rand() % 360) * 10, 10, sin(rand() % 360)*10)); //-9.8, gravedad "realista", 10 en la y para que salga hacia arriba
		newP->setMass(1.0f);
		newP->setDamping(1.0f);
		particlesVec.push_back(newP);

		time_ = 0;
	}
	time_ += time;
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
		else if (!particlesVec.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			it++;
	}
}
