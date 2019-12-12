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
///////////////////////////////////////////////////////
//Variables
Player* falcon;
Planet* target;
Load* load;
physx::PxRigidDynamic* movingCamera = nullptr;
Vector3 c;
Vector3 p;
Firework* victory = nullptr;
bool playing = false;
bool win = false;
int zoom = 50;
PxDistanceJoint* j;

///////////////////////////////////////////////////////
//Vectores
std::vector<Star*> stars;
std::vector<Ship*> ships;
std::vector<Planet*> planets;
std::vector<PxGenerator*> pxGens;
std::vector<FuelBox*> fuel;
std::vector<ParticleContact*> contacts;
std::vector<Particle*> municion;
std::vector<Particle*> Generated;
///////////////////////////////////////////////////////
//SDL
ServiceLocator services_; // (textures, font, music, etc)
SDLAudioManager audio_;
SRandBasedGenerator random_;

//////////////////////////////////////////////////////////////////////////////////////

Vector3 RandomVecPos()
{
	float x = services_.getRandomGenerator()->nextInt(-1000, 1000);
	float y = services_.getRandomGenerator()->nextInt(-1000, 1000);
	float z = services_.getRandomGenerator()->nextInt(-1000, 1000);

	Vector3 vec = Vector3(x, y, z);

	return vec;
}

Vector4 RandomVecCol()
{
	float a = services_.getRandomGenerator()->nextInt(0, 100);
	float b = services_.getRandomGenerator()->nextInt(0, 100);
	float c = services_.getRandomGenerator()->nextInt(0, 100);

	float x = a / 100.f;
	float y = b / 100.f;
	float z = c / 100.f;

	Vector4 vec = Vector4(x, y, z, 1);

	return vec;
}

void generateRandom()
{
	int nStars = services_.getRandomGenerator()->nextInt(0, 10);
	int nPlanets = services_.getRandomGenerator()->nextInt(0, 10);
	int nShips = services_.getRandomGenerator()->nextInt(0, 10);

	for (int i = 0; i < nPlanets; i++)
	{
		float a = services_.getRandomGenerator()->nextInt(0, 100);
		float rot = a / 100.f;
		float radio = services_.getRandomGenerator()->nextInt(10, 20);
		float sp = services_.getRandomGenerator()->nextInt(20, 100);
		float speed = sp / 100.0f;
		planets.push_back(new Planet(radio, RandomVecCol(), RandomVecPos(), sp, rot));
	}
	for (int j = 0; j < nStars; j++)
	{
		float time = services_.getRandomGenerator()->nextInt(10, 50);
		float radio = services_.getRandomGenerator()->nextInt(8, 20);

		stars.push_back(new Star(radio, RandomVecCol(), RandomVecPos(), time));
	}
	for (int k = 0; k < nShips; k++)
	{
		Ship* s = new Ship(RandomVecPos());
		s->getSpring()->getOther()->setVelocity(RandomVecPos());

		ships.push_back(s);
	}

	target = new Planet(5, Vector4(0.5, 0.5, 0.5, 1), RandomVecPos());
}

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

	falcon = new Player(gScene, gPhysics);
	falcon->getParticle()->getDin()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//gScene->addActor(*falcon->getParticle()->getDin());

	load = new Load(Vector3(-10, 0, 0), gScene, gPhysics);
	load->getPart()->getDin()->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	//gScene->addActor(*load->getPart()->getDin());


	j = PxDistanceJointCreate(*gPhysics, falcon->getParticle()->getDin(), PxTransform(Vector3(0, 0, 0)), load->getPart()->getDin(), PxTransform(Vector3(0, 0, 0)));
	j->setMaxDistance(25);
	j->setMinDistance(60);
	j->setStiffness(100.0f);

	j->setDistanceJointFlag(PxDistanceJointFlag::eSPRING_ENABLED, true);
	j->setDistanceJointFlag(PxDistanceJointFlag::eMIN_DISTANCE_ENABLED, true);
	j->setDistanceJointFlag(PxDistanceJointFlag::eMAX_DISTANCE_ENABLED, true);

	contactManager = new ParticleContactManager();

	movingCamera = gPhysics->createRigidDynamic(GetCamera()->getTransform());
	movingCamera->setGlobalPose(PxTransform(falcon->getParticle()->getDin()->getGlobalPose().p.x - 50, falcon->getParticle()->getDin()->getGlobalPose().p.y + 10, falcon->getParticle()->getDin()->getGlobalPose().p.z));
	movingCamera->setActorFlag(PxActorFlag::eDISABLE_GRAVITY, true);
	gScene->addActor(*movingCamera);

	c = GetCamera()->getDir();
	p = GetCamera()->getEye();

	generateRandom();
}

void deleteStuff()
{
	auto it = stars.begin();
	while (!stars.empty())
	{
		Star* star_ = *it;
		delete star_;
		stars.erase(it);
		it = stars.begin();
	}
	
	auto it1 = planets.begin();
	while (!planets.empty())
	{
		Planet* planet_ = *it1;
		delete planet_;
		planets.erase(it1);
		it1 = planets.begin();
	}
	
	auto it2 = ships.begin();
	while (!ships.empty())
	{
		Ship* ship_ = *it2;
		delete ship_;
		ships.erase(it2);
		it2 = ships.begin();
	}

	auto it3 = fuel.begin();
	while (!fuel.empty())
	{
		FuelBox* fuel_ = *it3;
		delete fuel_;
		fuel.erase(it3);
		it3 = fuel.begin();
	}

	auto it4 = pxGens.begin();
	while (!pxGens.empty())
	{
		PxGenerator* pxGen_ = *it4;
		delete pxGen_;
		pxGens.erase(it4);
		it4 = pxGens.begin();
	}

	auto it5 = contacts.begin();
	while (!contacts.empty())
	{
		ParticleContact* contact_ = *it5;
		delete contact_;
		contacts.erase(it5);
		it5 = contacts.begin();
	}

	target->getPart()->setPosition(Vector3(0, -10000000, 0));

	//gScene->removeActor(*falcon->getParticle()->getDin());
	//falcon->getParticle()->getDin()->setGlobalPose(PxTransform(Vector3(0, -10000000, 0))); 
	falcon->getParticle()->getDin()->setGlobalPose(PxTransform(Vector3(0, -10000000, 0))); 

	//gScene->removeActor(*load->getPart()->getDin()); 
	load->getPart()->getDin()->setGlobalPose(PxTransform(Vector3(0, -10000000, 0)));
}

void updateConsole() //X y Z cambiados por la camara
{
	system("CLS");

	std::cout << "Posicion del planeta objetivo: " << std::endl;
	std::cout << target->getPart()->getPosition().z << " " << target->getPart()->getPosition().y << " " << target->getPart()->getPosition().x << std::endl;
	
	std::cout << "Posicion actual: " << std::endl;
	std::cout << falcon->getParticle()->getDin()->getGlobalPose().p.z << " " << falcon->getParticle()->getDin()->getGlobalPose().p.y << " " << falcon->getParticle()->getDin()->getGlobalPose().p.x << std::endl;

	std::cout << "Velocidad actual: " << std::endl;
	std::cout << falcon->getAcc().x << " " << falcon->getAcc().y << " " << falcon->getAcc().z << std::endl;

	std::cout << "Salud de la nave: " << std::endl;
	std::cout << falcon->getHealth() << std::endl;

	std::cout << "Salud de la carga: " << std::endl;
	std::cout << load->getHealth() << std::endl;

	std::cout << "Fuel de la nave: " << std::endl;
	std::cout << falcon->getFuel() << std::endl;

	std::cout << "Municion restante: " << std::endl;
	std::cout << (15 - municion.size()) << std::endl;
}

bool detectCollisions(Particle* p1, Particle* p2)
{
	Vector3 a;
	Vector3 b;

	if (p2 != nullptr && p2->getDin() != nullptr)
	{
		a = p2->getDin()->getGlobalPose().p;
	}
	else if (p2 != nullptr)
	{
		a = p2->getPosition();
	}
	else
		return false;

	if (p1 != nullptr && p1->getDin() != nullptr)
	{
		b = p1->getDin()->getGlobalPose().p;
	}
	else if (p1 != nullptr)
	{
		b = p1->getPosition();
	}
	else
		return false;

	Vector3 distance = a - b;

	//Vector3 distance = p2->getPosition() - p1->getPosition();

	float d = distance.magnitude();

	float sumaRadio = p1->getRadio() + p2->getRadio();

	if (d < sumaRadio)
		return true;
	else
		return false;
}

void collisionSystem()
{
	//////////////////////////////////////////////////////////////////////////////////////

	//Nave con objetivo

	if (detectCollisions(falcon->getParticle(), target->getPart()))
	{
		playing = false;
		services_.getAudios()->playChannel(Resources::Laser, 0); //Cambiar por sonido de victoria
		win = true;
		victory = new Firework(0.05f, GetCamera()->getEye(), Vector3(0.0f, 0.0f, 0.0f), 0, 1, 3, Vector3(0, 0, 0));
		Vector3 v = victory->getPosition();
		v.x = v.x - 50;
		v.y = v.y - 30;
		movingCamera->setGlobalPose(PxTransform(v));
		GetCamera()->setEye(movingCamera->getGlobalPose().p);
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
				
				if (!stars.empty())
				{
					stars.erase(stars.begin());
					delete star;
					st = stars.begin();
				}
			}

			if (detectCollisions(load->getPart(), star->getPart()))
			{
				load->changeHealth(-10);
				pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));
				
				
				if (!stars.empty())
				{
					stars.erase(stars.begin());
					delete star;
					st = stars.begin();
				}
			}

			if (!stars.empty())
				++st;
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

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				++pl;
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
				++ast;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////////
	//Municion contra planetas, estrellas y navez

	auto shoot = municion.begin();
	while (!municion.empty() && shoot != municion.end()) {
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
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}

						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++st;
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

						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}

						fuel.push_back(new FuelBox(sh->getSpring()->getOther()->getRadio(), sh->getSpring()->getOther()->getPosition()));

						if (!ships.empty())
						{
							ships.erase(ast);
							delete sh;
							ast = ships.begin();
						}
					}

					if (detectCollisions(shot, sh->get1()))
					{
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}
					}

					if (detectCollisions(shot, sh->get2()))
					{
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}
					}

					if (detectCollisions(shot, sh->get3()))
					{
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}
					}

					if (detectCollisions(shot, sh->get4()))
					{
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}
					}

					if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
					  ++ast;
				}
			}

			auto pl = planets.begin();
			while (!planets.empty() && pl != planets.end()) {
				Planet* pla = (*pl);

				if (pla != nullptr)
				{					
					if (detectCollisions(shot, pla->getPart()))
					{
						if (!municion.empty())
						{
							municion.erase(shoot);
							delete shot;
							shoot = municion.begin();
						}

						Ship* s = new Ship(pla->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVecPos());
						ships.push_back(s);
						
						if (!planets.empty())
						{
							planets.erase(pl);
							delete pla;
							pl = planets.begin();
						}
					}

					if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++pl;
				}
			}

			if (!municion.empty())
				++shoot;
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

				if (!fuel.empty())
				{
					fuel.erase(fu);
					delete fue;
					fu = fuel.begin();
				}
			}

			if (!fuel.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				++fu;
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
						s->getSpring()->getOther()->setVelocity(RandomVecPos());
						ships.push_back(s);

						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++st;
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
						s->getSpring()->getOther()->setVelocity(RandomVecPos());
						ships.push_back(s);

						fuel.push_back(new FuelBox(sh->getSpring()->getOther()->getRadio(), sh->getSpring()->getOther()->getPosition()));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}

						if (!ships.empty())
						{
							ships.erase(ast);
							delete sh;
							ast = ships.begin();
						}
					}

					if (detectCollisions(pla1->getPart(), sh->get1()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}
					}

					if (detectCollisions(pla1->getPart(), sh->get2()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}
					}

					if (detectCollisions(pla1->getPart(), sh->get3()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}
					}

					if (detectCollisions(pla1->getPart(), sh->get4()))
					{
						ships.push_back(new Ship(pla1->getPart()->getPosition()));

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}
					}

					if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++ast;
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
						s->getSpring()->getOther()->setVelocity(RandomVecPos());
						ships.push_back(s);

						Ship* s1 = new Ship(pla2->getPart()->getPosition());
						s->getSpring()->getOther()->setVelocity(RandomVecPos());
						ships.push_back(s1);

						if (!planets.empty())
						{
							planets.erase(pl1);
							delete pla1;
							pl1 = planets.begin();
						}

						if (!planets.empty())
						{
							planets.erase(pl2);
							delete pla2;
							pl2 = planets.begin();
						}
					}

					if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++pl2;
				}

				else if (pl1 == pl2)
					++pl2;
			}

			if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				++pl1;
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

						if (!ships.empty())
						{
							ships.erase(ast1);
							delete sh;
							ast1 = ships.begin();
						}
						
						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (detectCollisions(star->getPart(), sh->get1()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (detectCollisions(star->getPart(), sh->get2()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (detectCollisions(star->getPart(), sh->get3()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (detectCollisions(star->getPart(), sh->get4()))
					{
						pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

						if (!stars.empty())
						{
							stars.erase(st);
							delete star;
							st = stars.begin();
						}
					}

					if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
						++st;
				}
			}

			if (!ships.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
				++ast1;
		}
	}
}

// Function to configure what happens in each step of physics
// interactive: true if the game is rendering, false if it offline
// t: time passed since last call in milliseconds
void stepPhysics(bool interactive, double t)
{
	if (playing)
	{
		PX_UNUSED(interactive);

		auto gens = pxGens.begin();
		while (!pxGens.empty() && gens != pxGens.end()) {
			PxGenerator* px = (*gens);

			if (px != nullptr)
			{
				px->update(t);

				if (px->getPart()->deathTime(t)) {
					pxGens.erase(gens);
					delete px;
					gens = pxGens.begin();
				}

				else if (!pxGens.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
					++gens;
			}
		}

		auto st = stars.begin();
		while (!stars.empty() && st != stars.end()) {
			Star* star = (*st);

			if (star != nullptr)
			{
				if (star->getPart() != nullptr && star->getPart()->deathTime(t)) {

					pxGens.push_back(new PxGenerator(star->getPart()->getPosition(), gScene, gPhysics));

					stars.erase(st);
					delete star;
					st = stars.begin();
				}

				else if (!stars.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
					++st;
			}
		}

		auto pla = planets.begin();
		while (!planets.empty() && pla != planets.end()) {
			Planet* planet = (*pla);

			if (planet != nullptr)
			{
				planet->update(t);

				planet->getAttach()->PxUpdateForce(falcon, t);

				if (!planets.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
					++pla;
			}
		}

		auto it = municion.begin();
		while (!municion.empty() && it != municion.end()) {
			Particle* p = (*it);

			if (p != nullptr)
			{
				p->update(t);

				if (p->deathTime(t)) {
					municion.erase(it);
					delete p;
					it = municion.begin();
				}

				else if (!municion.empty()) //si no esta vacio (se ha borrado el ultimo) avanza el iterador
					++it;
			}
		}

		falcon->update(t);

		collisionSystem();
		contactManager->update(t);

		gScene->simulate(t);
		gScene->fetchResults(true);

		if (playing)
			updateConsole();

		movingCamera->setGlobalPose(PxTransform(falcon->getParticle()->getDin()->getGlobalPose().p.x - zoom, falcon->getParticle()->getDin()->getGlobalPose().p.y + 10, falcon->getParticle()->getDin()->getGlobalPose().p.z));
		//GetCamera()->setDir(falcon->getParticle()->getDin()->getLinearVelocity());
		GetCamera()->setEye(movingCamera->getGlobalPose().p);

		if (falcon->getFuel() < 0 || falcon->getHealth() <= 0 || load->getHealth() <= 0)
		{
			playing = false;
			win = false;
		}
	}
	else 
	{
		if (win)
		{ 
			if (victory != nullptr && victory->update(t)) {
				victory = nullptr;
			}

			system("CLS");
			std::cout << "sos un genio" << std::endl;
			deleteStuff();
		}
		else
		{
			system("CLS");
			std::cout << "sos malisimo hermano" << std::endl;
			deleteStuff();
		}
	}
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
	case 'W':
	{
		falcon->handleEvent(key);
		break;
	}
	case 'A':
	{
		falcon->handleEvent(key);
		break;
	}
	case 'S':
	{
		falcon->handleEvent(key);

		break;
	}
	case 'D':
	{
		falcon->handleEvent(key);

		break;
	}
	case 'E':
	{
		falcon->handleEvent(key);

		break;
	}
	case 'Q':
	{
		falcon->handleEvent(key);

		break;
	}
	case 'X':
	{
		falcon->handleEvent(key);

		break;
	}
	case ' ':
	{
		if (municion.size() < 15)
		{
			Particle* p1 = new Particle(1, Vector4(0.0, 1.0, 0.0, 1.0), falcon->getParticle()->getDin()->getGlobalPose().p, 3.0, 0, 0, 0);
			p1->setDirVel(Vector3(0, 0, 0), GetCamera()->getDir() * 100);

			municion.push_back(p1);

			services_.getAudios()->playChannel(Resources::Laser, 0);

		}
		break;
	}

	////////////////////////////////////////////////////////////
	//Camara

	case 'J':
	{
		if (c.z > -1.0f)
		{
			c.z = c.z - 0.01;
		}
		GetCamera()->setDir(c);

		break;
	}
	case 'L':
	{
		if (c.z < 1.0f)
		{
			c.z = c.z + 0.01;
		}
		GetCamera()->setDir(c);

		break;
	}
	case 'K':
	{
		if (c.y > -1.0f)
		{
			c.y = c.y - 0.01;
		}
		GetCamera()->setDir(c);

		break;
	}
	case 'I':
	{
		if (c.y < 1.0f)
		{
			c.y = c.y + 0.01;
		}
		GetCamera()->setDir(c);

		break;
	}

	case 'U':
	{
		if (zoom < 100)
		{
			zoom++;
		}

		break;
	}
	case 'O':
	{
		if (zoom > -50)
		{
			zoom--;
		}

		break;
	}

	case 'M':
	{
		c.z = 0;
		c.x = 1;
		c.y = 0;
		zoom = 50;

		GetCamera()->setDir(c);

		break;
	}

	////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
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
	char a = ' ';
	
	std::cout << "Hola, joven piloto." << "\n" "Bienvenid@ al simulador de vuelo del modelo." << "\n" "Tu objetivo es transportar la carga al planeta marcado sin daños." << "\n" "En tu camino habrá asteroides, estrellas moribundas y planetas" << "\n" "Por suerte cuentas con disparos láser y tu destreza de pilotaje." << "\n" << "Los controles son los siguientes:" << "\n" << "- Control de la direccion con WASD" << "\n" << "- Control de la altura con QE" << "\n" << "- Control de la camara con IJKL" << "\n" << "- Control del zoom con UO" << "\n" << "Dispara con W" << "\n" << "Mira la consola para informacion importante" << "\n" << "\n" << "Introduce el codigo 'Y' para comenzar" << "\n";

	while (toupper(a) != 'Y')
	{
		std:cin >> a;

		if (toupper(a) == 'Y')
			playing = true;
	}

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

	services_.~ServiceLocator();
	audio_.~SDLAudioManager();
	random_.~SRandBasedGenerator();

	SDL_Quit();

	return 0;
}