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
	int maxBullets_ = 15;
	Camera* cam_ = nullptr;
	std::vector<Particle*> municion;
	float fuel = 1000;

public:
	Player(Camera* c);
	void update(float t);
	void handleEvent(char c);
	~Player();
	Particle* getParticle();
	bool shoot();
	std::vector<Particle*> getMunicion();
};

