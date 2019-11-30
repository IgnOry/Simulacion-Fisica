#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>

#include "core.hpp"
#include "RenderUtils.hpp"
#include "callbacks.hpp"
#include "Particle.h"
#include "ParticleGenerator.h"
#include "Firework.h"
#include "ParticleDrag.h"
#include "ParticleForceGenerator.h"
#include "ParticleForceRegistry.h"
#include "ParticleGravity.h"
#include "ParticleCable.h"
#include <iostream>

using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;


PxMaterial* gMaterial = NULL;

PxPvd* gPvd = NULL;

PxDefaultCpuDispatcher* gDispatcher = NULL;
PxScene* gScene = NULL;
ContactReportCallback gContactReportCallback;

std::vector<ParticleGenerator*> generatorsVec;
std::vector<Particle*> particlesVec;
std::vector<Firework*> fireworksVec;

ParticleCable* cable;
ParticleContact* contact;
Particle* p1;
Particle* p2;
int fireworkModes = 1;
int count = 3;
float g = 1.0;

// Initialize physics engine
void initPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	gFoundation = PxCreateFoundation(PX_FOUNDATION_VERSION, gAllocator, gErrorCallback);

	gPvd = PxCreatePvd(*gFoundation);
	PxPvdTransport* transport = PxDefaultPvdSocketTransportCreate(PVD_HOST, 5425, 10);
	gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

	gPhysics = PxCreatePhysics(PX_PHYSICS_VERSION, *gFoundation, PxTolerancesScale(), true, gPvd);

	gMaterial = gPhysics->createMaterial(0.5f, 0.5f, 0.6f);

	// For Solid Rigids +++++++++++++++++++++++++++++++++++++
	PxSceneDesc sceneDesc(gPhysics->getTolerancesScale());
	sceneDesc.gravity = PxVec3(0.0f, -9.8f, 0.0f);
	gDispatcher = PxDefaultCpuDispatcherCreate(2);
	sceneDesc.cpuDispatcher = gDispatcher;
	sceneDesc.filterShader = contactReportFilterShader;
	sceneDesc.simulationEventCallback = &gContactReportCallback;
	gScene = gPhysics->createScene(sceneDesc);
	// ------------------------------------------------------

	cable = new ParticleCable();
	contact = new ParticleContact();

	p1 = new Particle(1.0, Vector4(1.0, 0.0, 0.0, 1.0), Vector3(5.0, 0.0, 0.0));
	p2 = new Particle(1.0, Vector4(0.0, 0.0, 1.0, 1.0), Vector3(5.0, 0.0, 5.0));

	cable->particle[0] = p1;
	cable->particle[1] = p2;

	//contact->particle[0] = p1;
	//contact->particle[1] = p2;

	cable->addContact(contact, 5);
	cable->maxLength = 5;
	cable->restitution = 1;

	p1->setVelocity(Vector3(1, 0, 0));
	//particula = new Particle(1.0f);
	//particula->setDirVel();

}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	auto auxP = particlesVec.begin();

	while (!particlesVec.empty() && auxP != particlesVec.end()) {
		Particle* p = (*auxP);

		p->update(t);

		if (p->deathTime(t)) {
			particlesVec.erase(particlesVec.begin());
			delete p;
			auxP = particlesVec.begin();
		}

		else if (!particlesVec.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			auxP++;
	}

	auto auxG = generatorsVec.begin();
	while (!generatorsVec.empty() && auxG != generatorsVec.end()) {
		ParticleGenerator* p = (*auxG);

		p->update(t);

		if (p->deathTime(t)) {
			generatorsVec.erase(generatorsVec.begin());
			delete p;
			auxG = generatorsVec.begin();
		}

		else if (!generatorsVec.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			auxG++;
	}

	auto auxF = fireworksVec.begin();
	while (!fireworksVec.empty() && auxF != fireworksVec.end()) {
		Firework* f = (*auxF);

		if (f != nullptr && f->update(t)) {
			fireworksVec.erase(fireworksVec.begin());
			delete f;
			auxF = fireworksVec.begin();
		}

		else if (!fireworksVec.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
			auxF++;
	}

	contact->resolve(t);

	p1->update(t);
	p2->update(t);

	gScene->simulate(t);
	gScene->fetchResults(true);
}

// Function to clean data
// Add custom code to the begining of the function
void cleanupPhysics(bool interactive)
{
	PX_UNUSED(interactive);

	// Rigid Body ++++++++++++++++++++++++++++++++++++++++++
	gScene->release();
	gDispatcher->release();
	// -----------------------------------------------------
	gPhysics->release();
	PxPvdTransport* transport = gPvd->getTransport();
	gPvd->release();
	transport->release();

	gFoundation->release();
}

// Function called when a key is pressed
void keyPress(unsigned char key, const PxTransform& camera)
{
	PX_UNUSED(camera);
	std::string text = "";

	switch (toupper(key))
	{
		//case 'B': break;
		//case ' ':	break;
	case 'F':
	{
		Firework* f = new Firework(0.05f, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0, fireworkModes, count, Vector3(0, -g, 0));
		fireworksVec.push_back(f);
	}
	break;
	case 'G':
	{
		ParticleGenerator* gen = new ParticleGenerator(Vector3(0.0f, 0.0f, 0.0f), 0.01f, 5.0f, Vector3(0.0, -g, 0.0), Vector3(1.0, 0.0, 0.0));
		generatorsVec.push_back(gen);
	}
	break;
	case 'P':
	{
		if (particlesVec.size() == 10)
		{
			Particle* aux = *particlesVec.begin();
			particlesVec.erase(particlesVec.begin());
			delete aux;
		}
		Particle* p = new Particle(1, Vector4(0.0, 1.0, 0.0, 1.0), GetCamera()->getTransform().p, 5.0f);
		p->setDirVel(Vector3(0.0, 0.0, 0.0), GetCamera()->getDir());
		particlesVec.push_back(p);
	}
	break;
	case 'M':
	{
		if (fireworkModes < 3)
			fireworkModes++;
		else
			fireworkModes = 1;

		switch (fireworkModes) {
		case 1:
			text = "Fuegos artificales";
			break;
		case 2:
			text = "Hacia arriba";
			break;
		case 3:
			text = "Generador de particulas (Practica 1)";
			break;
		}
		std::cout << "Modo: " << text << std::endl;
	}
	break;
	case 'C':
	{
		if (count < 15)
			count++;
		else
			count = 3;

		std::cout << "Particulas: " << count << std::endl;
	}
	case '+':
	{
		g += 0.5;
		std::cout << "Gravedad: " << g << std::endl;
	}
	break;
	case '-':
	{
		g -= 0.5;
		std::cout << "Gravedad: " << g << std::endl;
	}
	break;
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}


int main(int, const char* const*)
{
#ifndef OFFLINE_EXECUTION 
	extern void renderLoop();
	renderLoop();
#else
	static const PxU32 frameCount = 100;
	initPhysics(false);
	for (PxU32 i = 0; i < frameCount; i++)
		stepPhysics(false);
	cleanupPhysics(false);
#endif

	return 0;
}