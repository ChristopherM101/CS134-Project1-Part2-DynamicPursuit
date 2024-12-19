#pragma once
//  Kevin M. Smith - CS 134 SJSU

#include "ofMain.h"
#include "Particle.h"


//  Pure Virtual Function Class - must be subclassed to create new forces.
//
class ParticleForce {
public:
    bool applyOnce = false;
    bool applied = false;
	virtual void updateForce(Particle *) = 0;
};

class ParticleSystem {
public:
	void add(const Particle &);
	void addForce(ParticleForce *);
	void remove(int);
    void reset();
	void update();
	int removeNear(const ofVec3f & point, float dist);
	void draw();
	vector<Particle> particles;
	vector<ParticleForce *> forces;
};

class GravityForce: public ParticleForce {
	ofVec3f gravity;
public:
	GravityForce(const ofVec3f & gravity);
	void updateForce(Particle *);

    //void setGravity(const ofVec3f &newGravity) {
        //gravity = newGravity;
    //}
};

class TurbulenceForce : public ParticleForce {
	ofVec3f tmin, tmax;
public:
	TurbulenceForce(const ofVec3f & min, const ofVec3f &max);
	void updateForce(Particle *);

    void setTurbulence(const ofVec3f &newMinimumTurbulence, const ofVec3f &newMaximumTurbulence) {
        tmin = newMinimumTurbulence;
        tmax = newMaximumTurbulence;
    }

};

class ImpulseRadialForce : public ParticleForce {
    float magnitude;
public:
    ImpulseRadialForce(float magnitude);
    void updateForce(Particle *);
};
