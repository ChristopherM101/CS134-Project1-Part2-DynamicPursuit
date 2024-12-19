#pragma once

#include "ofMain.h"

class ParticleForceField;
typedef enum { ExplosionParticle, BulletParticle } ParticleType;
class Particle {
public:
	Particle();

	ofVec3f position;
	ofVec3f velocity;
	ofVec3f acceleration;
	ofVec3f forces;
	float	damping;
	float   mass;
	float   lifespan;
	float   radius;
    float   rotation;
	float   birthtime;
	void    integrateBullets();
    void    integrateExplosion();
	void    draw();
	float   age();        // sec
	ofColor color;
    ParticleType particleType;
};


