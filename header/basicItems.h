#ifndef BASIC_ITEMS_DEF
#define BASIC_ITEMS_DEF

#include <math.h>
#include <Box2D/Box2D.h>
#include <vector>
#include <map>
#include "item.h"
#include <AL/alut.h>

extern std::map<std::string, int> sounds;

class HealthPotion : public Item {
public:
	ALuint soundSource[1];
	HealthPotion(float x, float y, int amount) :
		Item(x, y, 0.2, "healthPotion"), m_amount(amount) {
			alGenSources(1, soundSource);
			alSourcei(soundSource[0], AL_BUFFER, sounds["heal"]);
			alSourcef(soundSource[0], AL_GAIN, 0.3);
			alSourcef(soundSource[0], AL_PITCH, 1);
			alSource3f(soundSource[0], AL_POSITION, x, y, 0.2);
		}
	void Pickup(Player *picker) override {
		picker->IncreaseHealth(m_amount);
		alSourcePlay(soundSource[0]);
	}
private:
	int m_amount;
};

#endif
