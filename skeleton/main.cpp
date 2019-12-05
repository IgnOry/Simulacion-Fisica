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
#include "SDLTexturesManager.h"
#include "SDLAudioManager.h"
#include "SDLFontsManager.h"
#include "ServiceLocator.h"
#include "SRandBasedGenerator.h"
#include "sdl_includes.h"

#undef main

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
ParticleRod* barra;
ParticleCable* cable;
ParticleContact* contact;
ParticleContact* contact2;

PxGenerator* gene;

Particle* p1;
Particle* p2;
Particle* p3;
Particle* p4;
int fireworkModes = 1;
int counter = 3;
float g = 1.0;

/////////////////////////
ServiceLocator services_; // (textures, font, music, etc)
SDLFontsManager fonts_;
SDLTexturesManager textures_;
SDLAudioManager audio_;
SRandBasedGenerator random_;

SDL_Window* window_; // the window
SDL_Renderer* renderer_;  // the renderer

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

	gene = new PxGenerator(Vector3(5, 5, 5), gScene, gPhysics);

	p1 = new Particle(50, Vector3(0, 0, 0), gScene, gPhysics, false, 1);
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

	gene->update(t);

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
		Firework* f = new Firework(0.05f, Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), 0, fireworkModes, counter, Vector3(0, -g, 0));
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
		if (counter < 15)
			counter++;
		else
			counter = 3;

		std::cout << "Particulas: " << counter << std::endl;
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

	/*case 'V':
	{
		p1->setVelocity(Vector3(1.0, 0.0, 0.0));

	}
	break;

	case 'B':
	{
		p1->setVelocity(Vector3(0.0, 0.0, 0.0));
	}
	break;
	case 'H':
	{
		p3->setVelocity(Vector3(1.0, 0.0, 0.0));
	}
	break;
	case 'J':
	{
		p3->setVelocity(Vector3(0.0, 0.0, 0.0));
	}
	break;*/

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

	fonts_.init();
	textures_.init();
	audio_.init();

	services_.setTextures(&textures_);
	services_.setAudios(&audio_);
	services_.setFonts(&fonts_);
	services_.setRandomGenerator(&random_);

	for (auto& image : Resources::specialImages_) {
		textures_.loadFromImg(image.id, renderer_, image.fileName, image.width, image.height, image.columns, image.rows, image.frameTotal);
	}

	for (auto& image : Resources::images_) {
		textures_.loadFromImg(image.id, renderer_, image.fileName, image.width, image.height, image.columns, image.rows, image.frameTotal);
	}

	for (auto& font : Resources::fonts_) {
		fonts_.loadFont(font.id, font.fileName, font.size);
	}

	for (auto& txtmsg : Resources::messages_) {
		textures_.loadFromText(txtmsg.id, renderer_, txtmsg.msg,
			fonts_[txtmsg.fontId], txtmsg.color);
	}

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