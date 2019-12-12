#pragma once
#include "Particle.h"
#include "sdl_includes.h"
#include <iostream>
#include <vector>
#include "Render/Camera.h"
class Player
{
private:
	Particle* falcon;
	float fuel = 1000;
	float health = 100;
	float throtle = 0;
	float leftright = 0;
	float topdown = 0;
	Vector3 pos;
	bool trapped = false;
public:
	Player(PxScene* scene_, PxPhysics* pxphy_);
	void update(float t);
	void handleEvent(char c);
	~Player();
	Particle* getParticle();
	float getHealth();
	void changeHealth(float value);
	float getFuel();
	void changeFuel(float value_);
	Vector3 getAcc();
	void setTrapped();
};

