#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#include <Box2D/Box2D.h>
#include <vector>

class Particle {
public:
	Particle(b2Vec2 pos, b2Vec2 vel, float scale, float rotation, float alpha);
	b2Vec2 pos;
	b2Vec2 vel;
	float scale;
	float rotation;
	float alpha;

	void Draw();
	void Update();
	void AddForce(b2Vec2 force);
};

class Emitter {
public:
	Emitter(b2Vec2 pos, b2Vec2 initialVelocity, b2Vec2 force, int particleCount, float lifespan, std::string texture);
	~Emitter();
	void Update();
	void Draw();
	void Start();
	void SetSpeed(float minSpeed, float maxSpeed);
	void SetScale(float startScale, float endScale);
	bool IsDone();

private:
	std::vector<Particle*> particles;
	b2Vec2 pos;
	b2Vec2 initialVelocity;
	b2Vec2 force;
	std::string texture;
	int particleCount;
	float lifespan;
	float lifespanTimer;
	
	float minSpeed;
	float maxSpeed;

	float startScale;
	float endScale;
};

class ParticleSystem {
public:
	void AddEmitter(Emitter* emitter);
	void Update();
	void Draw();

private:
	std::vector<Emitter*> emitters;
};

#endif