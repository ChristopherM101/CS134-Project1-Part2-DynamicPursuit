#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "Emitter.h"
#include "Shape.h"
#include "TriangleShape.h"
#include "ImageShape.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Particle.h"

#include <vector>



class ofApp : public ofBaseApp{

    bool isDragging = false; // To track dragging state

    public:
    bool showHeading;
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void resetGame();
    void integrate(float thrustNum);

    // for smoother rotations
    float targetRotation = 0.0f;
    bool isRotatingLeft = false;
    bool isRotatingRight = false;
    float smoothingFactor = 0.1f;
    enum class RotationDirection { None, Left, Right };
    RotationDirection lastRotationDirection = RotationDirection::None;

    // for smoother speed
    float currentSpeed = 0.0f;
    float acceleration = 5.0f;
    bool isMovingForward = false;
    bool isMovingBackward = false;
    enum class MovingDirection { None, Forward, Backwards};
    MovingDirection lastMovingDirection = MovingDirection::None;

    float triSpeed = 0.0f;


    TriangleShape tri;
    ImageShape ship;


    glm::vec3 dragOffset;//for dragging implementation
    Emitter  *emitter = NULL;
    Emitter* selectedEmitter = nullptr;
    std::vector<Emitter*> emitters;

    ofImage backgroundImage;
    ofImage defaultImage;
    ofImage customImage;
    ofVec3f mouse_last;
    bool imageLoaded;

    // Some basic UI
    //
    bool showingGUI = false;
    bool gameStarted;
    float gameStartTime;
    float gameOver;

    float elapsedTime;

    int triEnergy;

    float mass = 1.0;

    // sound
    ofSoundPlayer thrusterSound;
    void playThrusterSound();
    void stopThrusterSound();

    ofSoundPlayer collisionSound;
    void playCollisionSound();
    void stopCollisionSound();

    ofxFloatSlider rate;
    ofxFloatSlider life;
    ofxFloatSlider velocity;
    ofxLabel screenSize;
    ofxFloatSlider scale;
    ofxFloatSlider rotationSpeed;

    ofxIntSlider triangleEnergy;
    ofxFloatSlider triangleSpeed;
    ofxFloatSlider triangleScaleSlider;
    ofxFloatSlider triangleRotationSpeed;

    ofxToggle bShowImageToggle;
    
    //heading
    ofxToggle bShowHeadingToggle;
    
    // level slider
    ofxIntSlider levelSlider;

    ofxPanel playerGui;
    ofxPanel agentsGui;

    ofxIntSlider numberOfAgents;

    // Update Functions
    void checkBounds();
    void updateGameStatus();
    void handlePlayerMovement();
    void applyThrust(int direction);
    void updateEmitterSettings();
    void setEmitterConfiguration(float newRate, float newLife, float newVelocity, float newScale);
    void updateSprites();
    void checkGameOver();


    // Draw Functions
    void drawGameScene();
    void drawStartScreen();
    void drawGameOverScreen();
    void drawPlayerEnergyText(bool gameStarted, float triangleEnergy);
    void drawTimeElapsedText(bool gameStarted, float elapsedTime);
    void drawResetMessage();
    void drawGUI();
    void drawFrameRateText();
    void drawInstructionsText();
    void drawScreenBounds();

    void handleSpriteExplosion(int i);

    bool isDraggingItem = false;
    glm::vec3 mousePrevPos;
    ofEasyCam    cam;


    ParticleEmitter particleBulletEmitter;
    ParticleEmitter explosionEmitter;
    ParticleEmitter explosionRingEmitter;

    TurbulenceForce *explosionTurbForce;
    TurbulenceForce *explosionRingTurbForce;
    GravityForce *explosionGravityForce;
    ImpulseRadialForce *explosionRadialForce;


    ofxPanel gui;

    float gravityValue;

    GravityForce* gForce = nullptr;
    TurbulenceForce* turbulence = nullptr;

    glm::vec3 velocityValue;

    glm::vec3 newMinimumValue;

    glm::vec3 newMaximumValue;

    ofxFloatSlider minimum1;
    ofxFloatSlider minimum2;
    ofxFloatSlider minimum3;

    ofxFloatSlider maximum1;
    ofxFloatSlider maximum2;
    ofxFloatSlider maximum3;

    ofxFloatSlider gravity;
    ofxFloatSlider damping;
    ofxFloatSlider explosionRadius;
    ofxFloatSlider bulletRadius;
    ofxVec3Slider bulletsVelocity;
    ofxFloatSlider lifespan;
    ofxFloatSlider bulletsRate;

    private:

    // Setup Functions
    void setupSound();
    void setupImages();
    void setupGameState();
    void setupEmitter();
    void setupGUI();

    void checkParticleCollisions();

};
