#include "Particle.h"


Particle::Particle() {

	// initialize particle with some reasonable values first;
	//
	velocity.set(0, 0, 0);
	acceleration.set(0, 0, 0);
	position.set(0, 0, 0);
	forces.set(0, 0, 0);
	lifespan = 5;
	birthtime = 0;
	radius = .1;
	damping = .99;
	mass = 1;
	color = ofColor::aquamarine;
}

void Particle::draw() {
    ofSetColor(color);
    ofDrawCircle(position.x, position.y, radius);
}


void Particle::integrateBullets() {
    // Convert rotation angle from degrees to radians
    float angleRad = ofDegToRad(rotation - 90);

    // Interval for this step
    float dt = 1.0 / ofGetFrameRate();

    // Calculate the direction based on the rotation
    float directionX = cos(angleRad);
    float directionY = sin(angleRad);
    
    position.x += directionX * velocity.length();
    position.y += directionY * velocity.length();
}

void Particle::integrateExplosion() {


    // interval for this step
    //
    float dt = 1.0 / ofGetFrameRate();

    // update position based on velocity
    //
    position += (velocity * dt);

    // update acceleration with accumulated paritcles forces
    // remember :  (f = ma) OR (a = 1/m * f)
    //
    ofVec3f accel = acceleration;    // start with any acceleration already on the particle
    accel += (forces * (1.0 / mass));
    velocity += accel * dt;

    // add a little damping for good measure
    //
    velocity *= damping;

    // clear forces on particle (they get re-added each step)
    //
    forces.set(0, 0, 0);
}
//  return age in seconds
//
float Particle::age() {
	return (ofGetElapsedTimeMillis() - birthtime)/1000.0;
}


