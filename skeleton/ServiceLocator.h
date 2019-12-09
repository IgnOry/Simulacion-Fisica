#pragma once

#include "AudioManager.h"
#include "RandomNumberGenerator.h"

/*
 *
 */
class ServiceLocator {
public:
	ServiceLocator();
	virtual ~ServiceLocator();

	AudioManager* getAudios() const;
	RandomNumberGenerator* getRandomGenerator() const;

	void setAudios(AudioManager* audioManager);
	void setRandomGenerator(RandomNumberGenerator* randomGenerator);

private:
	AudioManager* audio_;
	RandomNumberGenerator* random_;
};

