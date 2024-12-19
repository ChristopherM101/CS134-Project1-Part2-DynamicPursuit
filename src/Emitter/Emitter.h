#pragma once

#include "ofMain.h"
#include "Shape.h"
#include "Sprite.h"

//
//  Manages all Sprites in a system.  You can create multiple systems
//
class SpriteList {
public:
    void add(Sprite);
    void remove(int);
    void update();
    void draw();
    void clear() {
    sprites.clear(); // Clears all sprites from the list
    }
    vector<Sprite> sprites;
};


//  General purpose Emitter class for emitting sprites
//  This works similar to a Particle emitter
//
class Emitter : public Shape {
public:
    Emitter();
    void init();
    void draw();
    void start();
    void stop();
    void setLifespan(float);
    void setVelocity(const glm::vec3 v);
    void setChildImage(ofImage);
    void setImage(ofImage);
    void setRate(float);
    void setRotation(float);
    void setScale(float);
//    void update();
    void update(const glm::vec3& trianglePos, int nAgents);
    void clearSprites();
    

    // virtuals - can overloaded
    virtual void moveSprite(Sprite *, glm::vec3 playerPos, float rotationSpeed);
    virtual void spawnSprite();
    virtual bool inside(glm::vec3 p) {
        glm::vec3 s = glm::inverse(getTransform()) * glm::vec4(p, 1);
        return (s.x > -width / 2 && s.x < width / 2 && s.y > -height / 2 && s.y < height / 2);
    }
    virtual void getPlayerPosition(glm::vec3 p);
    
    SpriteList *sys;
    float rate;
    glm::vec3 velocity;
    float lifespan;
    float rotationSpeed;
    float scale;
    bool started;
    float lastSpawned;
    ofImage childImage;
    ofImage image;
    bool drawable;
    bool haveChildImage;
    bool haveImage;
    float width, height;
    glm::vec3 playerPosition;
};

