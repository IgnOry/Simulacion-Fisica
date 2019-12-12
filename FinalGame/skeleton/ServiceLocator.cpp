#include "ServiceLocator.h"

ServiceLocator::ServiceLocator() :
		audio_(nullptr), random_(nullptr) {
}

ServiceLocator::~ServiceLocator() {
}

AudioManager* ServiceLocator::getAudios() const {
	return audio_;
}

void ServiceLocator::setAudios(AudioManager* audioManager) {
	audio_ = audioManager;
}

RandomNumberGenerator* ServiceLocator::getRandomGenerator() const {
	return random_;
}

void ServiceLocator::setRandomGenerator(
		RandomNumberGenerator* randomGenerator) {
	random_ = randomGenerator;
}
