#include "ofApp.h"
//----------------------------------------------------------------------------------
//
// This example code demonstrates the use of an "Emitter" class to emit Sprites
// and set them in motion. The concept of an "Emitter" is taken from particle
// systems (which we will cover next week).
//
// The Sprite class has also been upgraded to include lifespan, velocity and age
// members.   The emitter can control rate of emission and the current velocity
// of the particles. In this example, there is no acceleration or physics, the
// sprites just move simple frame-based animation.
//
// The code shows a way to attach images to the sprites and optional the
// emitter (which is a point source) can also have an image.  If there are
// no images attached, a placeholder rectangle is drawn.
// Emitters  can be placed anywhere in the window. In this example, you can drag
// it around with the mouse.
//
// OF has an add-in called ofxGUI which is a very simple UI that is useful for
// creating sliders, buttons and fields. It is not recommended for commercial
// game development, but it is useful for testing.  The "h" key will hide the GUI
//
// If you want to run this example, you need to use the ofxGUI add-in in your
// setup.
//
//
//  Kevin M. Smith - CS 134 SJSU




//  Add a Sprite to the Sprite System
//
void SpriteList::add(Sprite s) {
    sprites.push_back(s);
}

// Remove a sprite from the sprite system. Note that this function is not currently
// used. The typical case is that sprites automatically get removed when the reach
// their lifespan.
//
void SpriteList::remove(int i) {
    sprites.erase(sprites.begin() + i);
}

//  Update the SpriteSystem by checking which sprites have exceeded their
//  lifespan (and deleting).  Also the sprite is moved to it's next
//  location based on velocity and direction.
//
void SpriteList::update() {

    if (sprites.size() == 0) return;
    vector<Sprite>::iterator s = sprites.begin();
    vector<Sprite>::iterator tmp;

    // check which sprites have exceed their lifespan and delete
    // from list.  When deleting multiple objects from a vector while
    // traversing at the same time, use an iterator.
    //
    while (s != sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            //            cout << "deleting sprite: " << s->name << endl;
            tmp = sprites.erase(s);
            s = tmp;
        }
        else s++;
    }

    //  Move sprite
    //
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].pos += sprites[i].velocity / ofGetFrameRate();
    }
}

//  Render all the sprites
//
void SpriteList::draw() {
    for (int i = 0; i < sprites.size(); i++) {
        sprites[i].draw();
    }
}



Emitter::Emitter() {
    sys = new SpriteList();
    init();
}


void Emitter::init() {
    lifespan = 3000;    // default milliseconds
    started = false;

    lastSpawned = 0;
    rate = 1;    // sprites/sec
    haveChildImage = false;
    haveImage = false;
    velocity = ofVec3f(100, 100, 0);
    drawable = true;
    width = 50;
    height = 50;
}



//  Draw the Emitter if it is drawable. In many cases you would want a hidden emitter
//
//
void Emitter::draw() {
    sys->draw();
}

//  Update the Emitter. If it has been started, spawn new sprites with
//  initial velocity, lifespan, birthtime.
//
void Emitter::update(const glm::vec3 &trianglePos, int nAgents) {
    if (!started) return;

    float time = ofGetElapsedTimeMillis();

    int spawnCount = std::max(nAgents, 1);

    if ((time - lastSpawned) > (1000.0 / rate)) {
        for (int i = 0; i < spawnCount; i++) {
            spawnSprite();
        }
        lastSpawned = time;
    }

    if (sys->sprites.size() == 0) return;
    vector<Sprite>::iterator s = sys->sprites.begin();
    vector<Sprite>::iterator tmp;

    while (s != sys->sprites.end()) {
        if (s->lifespan != -1 && s->age() > s->lifespan) {
            tmp = sys->sprites.erase(s);
            s = tmp;
        } else {
            s++;
        }
    }

    for (int i = 0; i < sys->sprites.size(); i++) {
        moveSprite(&sys->sprites[i], trianglePos, rotationSpeed);
    }
}


// virtual function to move sprite (can be overloaded)
//
void Emitter::moveSprite(Sprite* sprite, glm::vec3 playerPosition, float rotationSpeed) {
    glm::vec3 direction = playerPosition - sprite->pos;

    float length = glm::length(direction);
    if (length > 0) {
        direction /= length; // Normalize the direction

        float speed = glm::length(sprite->velocity); // Use magnitude of velocity as speed

        // Update sprite position in both x and y directions
        sprite->pos += direction * speed / ofGetFrameRate();

        // Calculate the target angle to rotate the sprite to face the player
        float targetAngle = atan2(direction.y, direction.x) * (180.0 / PI); // Convert to degrees

        targetAngle -= 270.0f; // Adjust for sprite's top orientation

        // Calculate angle difference, ensuring we rotate in the shortest direction
        float angleDifference = targetAngle - sprite->rot;
        if (angleDifference > 180.0f) {
            angleDifference -= 360.0f;
        } else if (angleDifference < -180.0f) {
            angleDifference += 360.0f;
        }

        // Adjust the rotation based on the rotationSpeed slider
        sprite->rot += angleDifference * (rotationSpeed / ofGetFrameRate());

        // Prevent over-rotation
        if (sprite->rot > 360.0f) sprite->rot -= 360.0f;
        if (sprite->rot < -360.0f) sprite->rot += 360.0f;
    }
}

// virtual function to spawn sprite (can be overloaded)
//
void Emitter::spawnSprite() {
    Sprite sprite;
    if (haveChildImage) sprite.setImage(childImage);
    sprite.velocity = velocity;
    sprite.lifespan = lifespan;
    sprite.pos = glm::vec3(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), 0);
    sprite.rot = ofRandom(0, 360);
    sprite.birthtime = ofGetElapsedTimeMillis();
    sys->add(sprite);
}

// Start/Stop the emitter.
//
void Emitter::start() {
    started = true;
    lastSpawned = ofGetElapsedTimeMillis();
}

void Emitter::stop() {
    started = false;
}


void Emitter::setLifespan(float life) {
    lifespan = life;
}

void Emitter::setVelocity(const glm::vec3 v) {
    velocity = v;
}

void Emitter::setChildImage(ofImage img) {
    childImage = img;
    haveChildImage = true;
}

void Emitter::setImage(ofImage img) {
    image = img;
}

void Emitter::setScale (float s) {
    scale = s;
}

void Emitter::setRate(float r) {
    rate = r;
}

void Emitter::getPlayerPosition(glm::vec3 p){
    playerPosition = p;
}

void Emitter::clearSprites() {
    sys->clear(); // Calls the clear function of SpriteList
}
