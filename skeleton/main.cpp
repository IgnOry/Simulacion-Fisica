#include <ctype.h>

#include <PxPhysicsAPI.h>

#include <vector>
#include <queue>

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
#include "ParticleContactResolver.h"
#include "ParticleRod.h"
#include "ParticleContactManager.h"
#include "PxGenerator.h"
#include <iostream>

///////////////////////////////////////////////
#include <SDL.h>
#include "Resources.h"
#include "SDLAudioManager.h"
#include "ServiceLocator.h"
#include "SRandBasedGenerator.h"
#include "sdl_includes.h"

#undef main

///////////////////////////////////////////////

#include "Player.h"
#include "Star.h"
#include "Asteroid.h"
#include "Planet.h"
#include "FuelBox.h"

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

ParticleContactManager* contactManager;
ParticleCable* barra;
ParticleContact* contact;

PxGenerator* gene;

Particle* p1;
Particle* p2;

int fireworkModes = 1;
int counter = 3;
float g = 1.0;
bool running;

/////////////////////
Player* falcon;
physx::PxRigidDynamic* movingCamera = nullptr;
Vector3 c;
Vector3 p;
Planet* pl;

///////////////////////////////////////////////////////
//Vectores de partículas, de forma directa o indirecta
std::vector<Particle*> particles; //Vector con todas
std::vector<Star*> stars;
std::vector<Asteroid*> asteroids;
std::vector<Planet*> planets;
std::vector<PxGenerator*> pxGens;
std::vector<FuelBox*> fuel;
std::vector<ParticleContact*> contacts;
/////////////////////////
ServiceLocator services_; // (textures, font, music, etc)
SDLAudioManager audio_;
SRandBasedGenerator random_;

//////////////////////////////////////////////////////////////////////////////////////

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

	falcon = new Player(GetCamera());
	/*p1 = new Particle(50, Vector3(0, -10, 0), Vector4(1, 1, 1, 1), gScene, gPhysics, false, 1);
	p2 = new Particle(1, Vector4(1, 1, 1, 1), Vector3(-15, 0, 0), 0, 0, 0, 1);

	barra = new ParticleCable();
	contact = new ParticleContact();*/
	contactManager = new ParticleContactManager();

	/*barra->particle[0] = falcon->getParticle();
	barra->particle[1] = p2;
	barra->maxLength = 15;
	barra->restitution = 0;*/

	//movingCamera = gPhysics->createRigidDynamic(GetCamera()->getTransform());
	//movingCamera->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//gScene->addActor(*movingCamera);

	pl = new Planet(5, Vector4(0.5, 0.5, 0.5, 0), Vector3(2, 0, 0));

	//FuelBox* f = new FuelBox(0.5, Vector3(5, 5, 5));
	//particles.push_back(pl->getPart());
	planets.push_back(pl);

	c = GetCamera()->getDir();
	p = GetCamera()->getEye();
}

void collisionSystem()
{
	//Nave con cosas
	/*
	
	//////////////////////////////////////////////////////////////////////////////////////

	//Nave con objetivo

	if (detectCollisions(falcon.getPart(), target.getPart())
	{
	}

	//////////////////////////////////////////////////////////////////////////////////////

	//Nave/Carga con planetas, Nave con asteroides y Nave con estrellas

	//recorrido de vectores de planetas, asteroides, estrellas

	stars;
	asteroids;

	if (detectCollisions(falcon.getPart(), pla/ast/sta)
	{

	}

	if (detectCollisions(load.getPart(), pla/ast/sta)
	{

	}
	
	//////////////////////////////////////////////////////////////////////////////////////

	//Nave con Combustible
	
	//recorrido de vector de fuelbox

	fuel;

		if (detectCollisions(falcon.getPart(), fbox)
	{

	}

	//////////////////////////////////////////////////////////////////////////////////////
	
	//Disparos con estrellas

	//Recorrido de vector de disparos
	//Recorrido de vector de estrellas

	falcon->getMunicion();
	stars;

	//////////////////////////////////////////////////////////////////////////////////////

	//Disparos con planetas

	//Recorrido de vector de disparos
	//Recorrido de vector de planetas

	falcon->getMunicion();

	//////////////////////////////////////////////////////////////////////////////////////

	//Disparos con asteroides

	//Recorrido de vector de disparos
	//Recorrido de vector de asteroides

	falcon->getMunicion();
	asteroids;

	//////////////////////////////////////////////////////////////////////////////////////
	
	//Asteroides con planetas

	//Recorrido de vector de asteroides
	//Recorrido de vector de planetas
	
	asteroids;

	//////////////////////////////////////////////////////////////////////////////////////

	//Asteroides con estrellas

	//Recorrido de vector de asteroides
	//Recorrido de vector de estrellas

	asteroids;
	stars;

	//////////////////////////////////////////////////////////////////////////////////////

	//Asteroides con asteroides

	//Recorrido de vector de asteroides
	//Recorrido de vector de asteroides
	//Comprobar que no es el mismo asteroide

	asteroids;

	//////////////////////////////////////////////////////////////////////////////////////

	//Planetas con estrellas

	//Recorrido de vector de planetas
	//Recorrido de vector de estrellas

	stars;

	//////////////////////////////////////////////////////////////////////////////////////

	//Planetas con otros planetas

	//Recorrido de vector de planetas
	//Recorrido de vector de planetas
	//Comprobar que no es el mismo planeta

	//////////////////////////////////////////////////////////////////////////////////////
	*/
	
}

bool detectCollisions(Particle* p1, Particle* p2)
{
		Vector3 distance = p2->getPosition() - p1->getPosition();

		float d = distance.magnitude();

		float sumaRadio = p1->getRadio() + p2->getRadio();

		if (d < sumaRadio)
		{
			return true;
			/*
			ParticleContact* cont = new ParticleContact();

					distance.normalize();

					cont->particle[0] = p2;
					cont->particle[1] = p1;
					cont->contactNormal = distance;
					cont->penetration = (sumaRadio - d);
					cont->restitution = 1;

					contactManager->addContact(cont);

			switch (type)
			{
				case 0: //Nave con objetivo
				{
					break;
				}
				case 1: //Player/Carga con cosa -> El primero pierde salud
				{
					break;
				}
				case 2: //Nave con combustible -> El primero gana fuel, el segundo se destruye
				{
					break;
				}
				case 3: //Disparo con estrellas -> El primero se destruye, el segundo se destruye y deja un generador de particulas ¡¡¡Ver colisiones con las particulas generadas!!!
				{
					break;
				}
				case 4: //Disparo con planetas -> El primero se destruye, el segundo se destruye y deja 2 asteroides de la mitad de tamaño
				{
					break;
				}
				case 5: //Disparo con asteroides -> El primero se destruye, el segundo se deja 2 asteroides de la mitad de tamaño si supera un minimo de tamaño

				{
					break;
				}
				case 6: //Asteroides con planetas -> El primero se destruye y deja 2 asteroides de la mitad de tamaño si supera un minimo de tamaño, el segundo se destruye y deja 2 asteroides de la mitad de tamaño
				{
					break;
				}
				case 7: //Asteroides con estrellas -> El primero se destruye y deja 2 asteroides de la mitad de tamaño si supera un minimo de tamaño, el segundo se destruye y deja un generador de particulas ¡¡¡Ver colisiones con las particulas generadas!!!
				{
					break;
				}
				case 8: //Planetas con estrellas -> El primero se destruye y deja 2 asteroides de la mitad de tamaño, el segundo se destruye y deja un generador de particulas ¡¡¡Ver colisiones con las particulas generadas!!!
				{
					break;
				}
				case 9: //Planetas con planetas -> El primero se destruye y deja 2 asteroides de la mitad de tamaño, el segundo se destruye y deja 2 asteroides de la mitad de tamaño
				{
					break;
				}
				default:
					break;*/
		}
		else
			return false;
}


// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	auto auxP = particles.begin();

	while (!particles.empty() && auxP != particles.end()) {
		Particle* p = (*auxP);

		if (p != nullptr)
		{

			pl->getAttach()->updateForce(p, t);

			p->update(t);

			if (p->deathTime(t)) {
				particles.erase(particles.begin());
				delete p;
				auxP = particles.begin();

			}

			else if (!particles.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				auxP++;
		}
	}

	auto gens = pxGens.begin();
	while (!pxGens.empty() && gens != pxGens.end()) {
		PxGenerator* px = (*gens);

		if (px != nullptr)
		{
			px->update(t);

			if (px->deathTime(t)) {
				pxGens.erase(pxGens.begin());
				delete px;
				gens = pxGens.begin();
			}

			else if (!pxGens.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				gens++;
		}
	}

	auto st = stars.begin();
	while (!stars.empty() && st != stars.end()) {
		Star* star = (*st);

		if (star != nullptr)
		{
			star->update(t);

			if (star->getPart()->deathTime(t)) {

				pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

				stars.erase(stars.begin());
				delete star;
				st = stars.begin();
			}

			else if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				st++;
		}
	}

	auto pla = planets.begin();
	while (!planets.empty() && pla != planets.end()) {
		Planet* planet = (*pla);

		if (planet != nullptr)
		{
			planet->update(t);

			/*if (planet->getPart()->deathTime(t)) {		//Estos no se destruyen por tiempo
				planets.erase(planets.begin());
				delete planet;
				pla = planets.begin();
			}*/

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				pla++;
		}
	}

	//detectCollisions();
	//contactManager->update(t);


	gScene->simulate(t);
	gScene->fetchResults(true);

	//movingCamera->setGlobalPose(PxTransform(falcon->getParticle()->getPosition().x - 50, falcon->getParticle()->getPosition().y, falcon->getParticle()->getPosition().z));
	//GetCamera()->setEye(movingCamera->getGlobalPose().p);

	/////////////////////////////////////////////////////////////////////////

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

	case 'P':
	{
		Particle* p = new Particle(0.1, Vector4(0.0, 1.0, 0.0, 1.0), GetCamera()->getTransform().p, 15.0f);
		p->setDirVel(Vector3(0.0, 0.0, 0.0), GetCamera()->getDir() * 15);
		particles.push_back(p);
		break;
	}

	/*case 'W':
	{
		falcon->handleEvent(key);
		//movingCamera->setLinearVelocity(Vector3(50, 0, 0));
		break;
	}
	case 'A':
	{
		falcon->handleEvent(key);
		//movingCamera->setLinearVelocity(Vector3(0, 0, -50));
		break;
	}
	case 'S':
	{
		falcon->handleEvent(key);
		//movingCamera->setLinearVelocity(Vector3(-50, 0, 0));

		break;
	}
	/*case 'D':
	{
		falcon->handleEvent(key);
		//movingCamera->setLinearVelocity(Vector3(0, 0, 50));

		break;
	}
	case 'X':
	{
		falcon->handleEvent(key);
		//movingCamera->setLinearVelocity(Vector3(0, 0, 0));

		break;
	}
	*/
	case ' ':
	{
		if (falcon->shoot())
			services_.getAudios()->playChannel(Resources::Laser, 0);
		//movingCamera->setLinearVelocity(Vector3(0, 0, 0));

		break;
	}//*/

	////////////////////////////////////////////////////////////

	case 'E':
	{
		if (p.x < 30.0f)
		{
			p.x = p.x + 1;
			cout << p.x << endl;
		}
		GetCamera()->setEye(p);

		break;
	}
	case 'Q':
	{
		if (p.x > -30.0f)
		{
			p.x = p.x - 1;
			cout << p.x << endl;
		}
		GetCamera()->setEye(p);

		break;
	}

	case 'R':
	{
		c.z = 0;
		p.x = 0;
		c.y = 0;

		GetCamera()->setEye(p);
		GetCamera()->setDir(c);

		break;
	}

	////////////////////////////////////////////////////////////
	case 'D':
	{
		if (c.z < 1.0f)
		{
			c.z = c.z + 0.1f;
			cout << c.z << endl;
		}
		GetCamera()->setDir(c);

		break;
	}
	case 'A':
	{
		if (c.z > -1.0f)
		{
			c.z = c.z - 0.1f;
			cout << c.z << endl;
		}
		GetCamera()->setDir(c);

		break;
	}
	////////////////////////////////////////////////////////////
	case 'I':
	{
		if (c.y < 1.0)
		{
			c.y = c.y + 0.1;
			cout << c.y << endl;
		}

		GetCamera()->setDir(c);

		break;
	}
	case 'K':
	{
		if (c.y > -1.0)
		{
			c.y = c.y - 0.1;
			cout << c.y << endl;
		}

		GetCamera()->setDir(c);

		break;
	}
	////////////////////////////////////////////////////////////


	case '+':
	{
		break;
	}
	case '-':
	{
		break;
	}
	default:
		break;
	}
}

void onCollision(physx::PxActor* actor1, physx::PxActor* actor2)
{
	PX_UNUSED(actor1);
	PX_UNUSED(actor2);
}

void initResources() {

	audio_.init();

	services_.setAudios(&audio_);
	services_.setRandomGenerator(&random_);

	for (auto& sound : Resources::sounds_) {
		audio_.loadSound(sound.id, sound.fileName);
	}

	for (auto& music : Resources::musics_) {
		audio_.loadSound(music.id, music.fileName);
	}

}

int main(int, const char* const*)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	initResources();

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