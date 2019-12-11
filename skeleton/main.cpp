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
#include <stdlib.h>

#undef main

///////////////////////////////////////////////

#include "Player.h"
#include "Star.h"
#include "Planet.h"
#include "FuelBox.h"
#include "Ship.h"
#include "Load.h"

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

///////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////
//Variables
Player* falcon;
Planet* target;
Load* load;
physx::PxRigidDynamic* movingCamera = nullptr;
Vector3 c;
Vector3 p;
Firework* victory = nullptr;
bool console = false;
///////////////////////////////////////////////////////
//Vectores
std::vector<Star*> stars;
std::vector<Ship*> ships;
std::vector<Planet*> planets;
std::vector<PxGenerator*> pxGens;
std::vector<FuelBox*> fuel;
std::vector<ParticleContact*> contacts;
///////////////////////////////////////////////////////
//SDL
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
	target = new Planet(5, Vector4(0.5, 0.5, 0.5, 0), Vector3(100, 100, 0));
	load = new Load(Vector3(500, 0, 0));
	contactManager = new ParticleContactManager();

	//movingCamera = gPhysics->createRigidDynamic(GetCamera()->getTransform());
	//movingCamera->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//gScene->addActor(*movingCamera);

	//pxGens.push_back(new PxGenerator(Vector3(5, 0, 0), gScene, gPhysics));

	ships.push_back(new Ship(Vector3(20, 0, 0)));
	
	c = GetCamera()->getDir();
	p = GetCamera()->getEye();
}

Vector3 RandomVec()
{
	float x = services_.getRandomGenerator()->nextInt(-5, 5);
	float y = services_.getRandomGenerator()->nextInt(-5, 5);
	float z = services_.getRandomGenerator()->nextInt(-5, 5);

	Vector3 vec = Vector3(x, y, z);

	return vec;
}

void deleteStuff()
{
	auto st = stars.begin();
	while (!stars.empty() && st != stars.end()) {
		Star* star = (*st);

		if (star != nullptr)
		{
			stars.erase(stars.begin());
			delete star;
			st = stars.begin();

			if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				st++;
		}
	}

	auto ast = ships.begin();
	while (!ships.empty() && ast != ships.end()) {
		Ship* sh = (*ast);

		if (sh != nullptr)
		{
			ships.erase(ships.begin());
			delete sh;
			ast = ships.begin();

			if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				ast++;
		}
	}

	auto pl = planets.begin();
	while (!planets.empty() && pl != planets.end()) {
		Planet* pla = (*pl);

		if (pla != nullptr)
		{
			planets.erase(planets.begin());
			delete pla;
			pl = planets.begin();

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				pl++;
		}
	}

	auto gens = pxGens.begin();
	while (!pxGens.empty() && gens != pxGens.end()) {
		PxGenerator* px = (*gens);

		if (px != nullptr)
		{
			pxGens.erase(pxGens.begin());
			delete px;
			gens = pxGens.begin();

			if (!pxGens.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				gens++;
		}
	}

	auto fu = fuel.begin();
	while (!fuel.empty() && fu != fuel.end()) {
		FuelBox* fue = (*fu);

		if (fue != nullptr)
		{
			fuel.erase(fu);
			delete fue;
			fu = fuel.begin();

			if (!fuel.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				fu++;
		}
	}

	auto cont = contacts.begin();
	while (!contacts.empty() && cont != contacts.end()) {
		ParticleContact* conta = (*cont);

		if (conta != nullptr)
		{
			contacts.erase(contacts.begin());
			delete conta;
			cont = contacts.begin();

			if (!contacts.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				cont++;
		}
	}

	//target
	//falcon
	//load
}

void generateRandom()
{
	//Stars
	//Planets
	//Ships
	//Target
}

void updateConsole()
{
	system("CLS");

	/*
	Stuff
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
	}
	else
		return false;
}

void collisionSystem()
{
	//////////////////////////////////////////////////////////////////////////////////////

	//Nave con objetivo

	if (detectCollisions(falcon->getParticle(), target->getPart()))
	{
		console = false;
		services_.getAudios()->playChannel(Resources::Laser, 0); //Cambiar por sonido de victoria
		victory = new Firework(15, falcon->getParticle()->getPosition(), Vector3(0, 0, 0), 0, 1, 3, Vector3(0,-5,0));
		deleteStuff();
	}

	//Nave y carga contra planetas, estrellas y naves

	auto st = stars.begin();
	while (!stars.empty() && st != stars.end()) {
		Star* star = (*st);

		if (star != nullptr)
		{
			if (detectCollisions(falcon->getParticle(), star->getPart()))
			{
				falcon->changeHealth(-10);
				pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));
				stars.erase(stars.begin());
				delete star;
				st = stars.begin();
			}

			if (detectCollisions(load->getPart(), star->getPart()))
			{
				load->changeHealth(-10);
				pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));
				stars.erase(stars.begin());
				delete star;
				st = stars.begin();
			}

			if (!stars.empty())
				st++;
		}
	}

	auto pl = planets.begin();
	while (!planets.empty() && pl != planets.end()) {
		Planet* pla = (*pl);

		if (pla != nullptr)
		{
			if (detectCollisions(falcon->getParticle(), pla->getPart()))
			{
				falcon->changeHealth(-10);
			}

			if (detectCollisions(load->getPart(), pla->getPart()))
			{
				load->changeHealth(-10);
			}

			//if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				pl++;
		}
	}

	auto ast = ships.begin();
	while (!ships.empty() && ast != ships.end()) {
		Ship* sh = (*ast);

		if (sh != nullptr)
		{
			//////////////////////////////////////////////////////////////////////////////////////
			//Player

			if (detectCollisions(falcon->getParticle(), sh->getSpring()->getOther()))
			{
				falcon->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(falcon->getParticle(), sh->get1()))
			{
				falcon->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(falcon->getParticle(), sh->get2()))
			{
				falcon->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(falcon->getParticle(), sh->get3()))
			{
				falcon->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(falcon->getParticle(), sh->get4()))
			{
				falcon->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			//////////////////////////////////////////////////////////////////////////////////////
			//Carga

			if (detectCollisions(load->getPart(), sh->getSpring()->getOther()))
			{
				load->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(load->getPart(), sh->get1()))
			{
				load->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(load->getPart(), sh->get2()))
			{
				load->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(load->getPart(), sh->get3()))
			{
				load->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (detectCollisions(load->getPart(), sh->get4()))
			{
				load->changeHealth(-10);
				ParticleContact* cont = new ParticleContact();

				Vector3 norm = sh->getSpring()->getOther()->getPosition() - falcon->getParticle()->getPosition();
				float radios = sh->getSpring()->getOther()->getRadio() + falcon->getParticle()->getRadio();
				float d = norm.magnitude();

				cont->particle[0] = falcon->getParticle();
				cont->particle[1] = sh->getSpring()->getOther();

				cont->contactNormal = norm;
				cont->penetration = radios - d;
				cont->restitution = 1;

				contactManager->addContact(cont);
			}

			if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				ast++;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//Municion contra planetas, estrellas y navez

	auto shoot = falcon->getMunicion().begin();
	while (!falcon->getMunicion().empty() && shoot != falcon->getMunicion().end()) {
		Particle* shot = (*shoot);

		if (shot != nullptr)
		{
			//////////////////////////////////////////////////////////////////////////////////////
			auto st = stars.begin();
			while (!stars.empty() && st != stars.end()) {
				Star* star = (*st);

				if (star != nullptr)
				{
					if (detectCollisions(shot, star->getPart()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();

						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						st++;
				}
			}

			auto ast = ships.begin();
			while (!ships.empty() && ast != ships.end()) {
				Ship* sh = (*ast);

				if (sh != nullptr)
				{
					//////////////////////////////////////////////////////////////////////////////////////
					//Player

					if (detectCollisions(shot, sh->getSpring()->getOther()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();

						fuel.push_back(new FuelBox(sh->getSpring()->getOther()->getRadio(), sh->getSpring()->getOther()->getPosition()));

						ships.erase(ships.begin());
						delete sh;
						ast = ships.begin();
					}

					if (detectCollisions(shot, sh->get1()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();
					}

					if (detectCollisions(shot, sh->get2()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();
					}

					if (detectCollisions(shot, sh->get3()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();
					}

					if (detectCollisions(shot, sh->get4()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();
					}

					if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						ast++;
				}
			}

			auto pl = planets.begin();
			while (!planets.empty() && pl != planets.end()) {
				Planet* pla = (*pl);

				if (pla != nullptr)
				{					
					if (detectCollisions(shot, pla->getPart()))
					{
						falcon->getMunicion().erase(falcon->getMunicion().begin());
						delete shot;
						shoot = falcon->getMunicion().begin();

						Ship* s = new Ship(pla->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVec());
						ships.push_back(s);
						
						planets.erase(planets.begin());
						delete pla;
						pl = planets.begin();
					}

					if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						pl++;
				}
			}

			if (!falcon->getMunicion().empty()) 
				shoot++;
		}
	}	

	auto fu = fuel.begin();
	while (!fuel.empty() && fu != fuel.end()) {
		FuelBox* fue = (*fu);

		if (fue != nullptr)
		{
			if (detectCollisions(falcon->getParticle(), fue->getPart()))
			{
				falcon->changeFuel(fue->getFuel());

				fuel.erase(fuel.begin());
				delete fue;
				fu = fuel.begin();
			}

			if (!fuel.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				fu++;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//Recorridos de vectores que no afectan al jugador

	//Planetas con estrellas y naves, y otros planetas
	auto pl1 = planets.begin();
	while (!planets.empty() && pl1 != planets.end()) {
		Planet* pla1 = (*pl1);

		if (pla1 != nullptr)
		{
			auto st = stars.begin();
			while (!stars.empty() && st != stars.end()) {
				Star* star = (*st);

				if (star != nullptr)
				{
					if (detectCollisions(pla1->getPart(), star->getPart()))
					{
						Ship* s = new Ship(pla1->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVec());
						ships.push_back(s);

						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						st++;
				}
			}

			auto ast = ships.begin();
			while (!ships.empty() && ast != ships.end()) {
				Ship* sh = (*ast);

				if (sh != nullptr)
				{
					//////////////////////////////////////////////////////////////////////////////////////
					//Player

					if (detectCollisions(pla1->getPart(), sh->getSpring()->getOther()))
					{
						Ship* s = new Ship(pla1->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVec());
						ships.push_back(s);

						fuel.push_back(new FuelBox(sh->getSpring()->getOther()->getRadio(), sh->getSpring()->getOther()->getPosition()));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();

						ships.erase(ships.begin());
						delete sh;
						ast = ships.begin();
					}

					if (detectCollisions(pla1->getPart(), sh->get1()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();
					}

					if (detectCollisions(pla1->getPart(), sh->get2()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();
					}

					if (detectCollisions(pla1->getPart(), sh->get3()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();
					}

					if (detectCollisions(pla1->getPart(), sh->get4()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();
					}

					if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						ast++;
				}
			}

			auto pl2 = planets.begin();
			while (!planets.empty() && pl2 != planets.end()) {
				Planet* pla2 = (*pl2);

				if (pla2 != nullptr && pla1 != pla2)
				{
					if (detectCollisions(pla1->getPart(), pla2->getPart()))
					{
						Ship* s = new Ship(pla1->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVec());
						ships.push_back(s);

						Ship* s1 = new Ship(pla2->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVec());
						ships.push_back(s1);

						planets.erase(planets.begin());
						delete pla1;
						pl1 = planets.begin();
						
						planets.erase(planets.begin());
						delete pla2;
						pl2 = planets.begin();
					}

					if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						pl2++;
				}
			}

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				pl2++;
		}
	}

	//Naves con estrellas (el choque no afecta contra otras naves, y las estrellas no se mueven asi que no hay que comprobar estrella con estrella)

	auto ast1 = ships.begin();
	while (!ships.empty() && ast1 != ships.end()) {
		Ship* sh = (*ast1);

		if (sh != nullptr)
		{
			//////////////////////////////////////////////////////////////////////////////////////
			//Player

			auto st = stars.begin();
			while (!stars.empty() && st != stars.end()) {
				Star* star = (*st);

				if (star != nullptr)
				{
					if (detectCollisions(star->getPart(), sh->getSpring()->getOther()))
					{
						fuel.push_back(new FuelBox(sh->getSpring()->getOther()->getRadio(), sh->getSpring()->getOther()->getPosition()));
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						ships.erase(ships.begin());
						delete sh;
						ast = ships.begin();

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (detectCollisions(star->getPart(), sh->get1()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (detectCollisions(star->getPart(), sh->get2()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (detectCollisions(star->getPart(), sh->get3()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (detectCollisions(star->getPart(), sh->get4()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						stars.erase(stars.begin());
						delete star;
						st = stars.begin();
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						st++;
				}
			}

			if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				ast1++;
		}
	}
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	PX_UNUSED(interactive);

	auto gens = pxGens.begin();
	while (!pxGens.empty() && gens != pxGens.end()) {
		PxGenerator* px = (*gens);

		if (px != nullptr)
		{
			px->update(t);

			if (px->getPart()->deathTime(t)) {
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

			//Estos no se destruyen por tiempo

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				pla++;
		}
	}

	falcon->update(t);

	collisionSystem();
	contactManager->update(t);

	if (victory != nullptr)
	{
		if (!victory->deathTime(t))
			victory->update(t);
		else
			delete victory;
	}

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