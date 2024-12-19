#include "ofApp.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"
#include "Particle.h"

void ofApp::integrate(float speedNum) {
    // Initialize current frame rate
    float framerate = ofGetFrameRate();
    float dt = 1.0 / framerate;
    
    // Implement Euler integrator here...
    //
    // linear motion
    //
    float angleRad = ofDegToRad(tri.rotation);
    glm::vec3 thrustForce = glm::vec3(cos(angleRad), sin(angleRad), 0 * speedNum);

    tri.force += thrustForce;
    tri.acceleration = tri.force / tri.mass;
    
    tri.velocity += tri.acceleration * dt;
    
    tri.velocity *= tri.damping;
    
    tri.pos += tri.velocity * dt;
    
    tri.rotation += tri.angularVelocity * dt;
    tri.angularVelocity += tri.angularAcceleration * dt;
    
    tri.angularVelocity *= tri.damping;
    
    // zero out forces
    //
    tri.force = glm::vec3(0, 0, 0);
    tri.acceleration = glm::vec3(0, 0, 0);
}

//--------------------------------------------------------------
void ofApp::playThrusterSound() {
    if (!thrusterSound.isPlaying()) {
        thrusterSound.play();
    }
}

//--------------------------------------------------------------
void ofApp::stopThrusterSound() {
    if (thrusterSound.isPlaying()) {
        thrusterSound.stop();
    }
}

//--------------------------------------------------------------

void ofApp::playCollisionSound() {
        collisionSound.play();
}

//--------------------------------------------------------------
void ofApp::stopCollisionSound() {
    if (collisionSound.isPlaying()) {
        collisionSound.stop();
    }
}

void ofApp::setup() {
    ofSetVerticalSync(true);
    ofSetFrameRate(60);
    gui.setup();
    gui.setPosition(50, 600);
    gui.add(bulletsVelocity.setup("Initial Velocity", ofVec3f(0, 10, 0), ofVec3f(0, 0, 0), ofVec3f(100, 100, 100)));
    gui.add(lifespan.setup("Lifespan", 2.0, .1, 10.0));
    gui.add(bulletsRate.setup("Rate", 1.0, .5, 60.0));
    gui.add(damping.setup("Damping", .99, .1, 1.0));
    gui.add(gravity.setup("Gravity", 10, 1, 20));
    gui.add(explosionRadius.setup("Radius", 0.75, .05, 2));
    gui.add(bulletRadius.setup("Bullet Radius", 10, 5, 20.0));
    gui.add(minimum2.setup("Min value 2", 0, -20.0, 20.0));
    gui.add(minimum3.setup("Min value 3", 0, -20.0, 20.0));

    gui.add(maximum1.setup("Max value 1", 0, -20.0, 20.0));
    gui.add(maximum2.setup("Max value 2", 0, -20.0, 20.0));
    gui.add(maximum3.setup("Max value 3", 0, -20.0, 20.0));

    particleBulletEmitter.start();

    ofSetFullscreen(true);
    setupSound();
    setupImages();
    setupGameState();
    setupEmitter();
    setupGUI();








    explosionTurbForce = new TurbulenceForce(ofVec3f(-20, -20, -20), ofVec3f(20, 20, 20));
    explosionGravityForce = new GravityForce(ofVec3f(0, -10, 0));
    explosionRadialForce = new ImpulseRadialForce(1000.0);

    explosionRingTurbForce = new TurbulenceForce(ofVec3f(-20, -0.1, -20), ofVec3f(20, 0.1, 20));

    explosionEmitter.sys->addForce(explosionTurbForce);
    explosionEmitter.sys->addForce(explosionGravityForce);
    explosionEmitter.sys->addForce(explosionRadialForce);

    // ring emitter
    explosionRingEmitter.sys->addForce(explosionTurbForce);
    //ringEmitter.sys->addForce(gravityForce);
    explosionRingEmitter.sys->addForce(explosionRadialForce);


    explosionEmitter.setVelocity(ofVec3f(0, 0, 0));
    explosionEmitter.setOneShot(true);
    explosionEmitter.setEmitterType(RadialEmitter);
    explosionEmitter.setGroupSize(50);

    explosionRingEmitter.setVelocity(ofVec3f(10, 0, 10));
    explosionRingEmitter.setOneShot(true);
    explosionRingEmitter.setEmitterType(RadialEmitter);
    explosionRingEmitter.setGroupSize(50);




}

void ofApp::setupSound() {
    thrusterSound.load("./Assets/thrusters-loop.wav");
    thrusterSound.setLoop(true);

    collisionSound.load("./Assets/minecraft_hit.mp3");
}

void ofApp::setupImages() {
    ofEnableAlphaBlending();
    backgroundImage.load("./Assets/galaxyBackground-Chris.JPG");

    if (defaultImage.load("./Assets/spriteAsteroid-Chris.PNG")) {
        imageLoaded = true;
    } else {
        cout << "Can't open image file" << endl;
        ofExit();
    }

    ofSetBackgroundColor(ofColor::black);
}

void ofApp::setupGameState() {
    gameStarted = false;
    gameOver = false;

    tri = TriangleShape();
    ship = ImageShape();

    ofSetVerticalSync(true);

    triEnergy = triangleEnergy;
    triSpeed = triangleSpeed;
}

void ofApp::setupEmitter() {
    emitter = new Emitter();
    emitter->pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);
    emitter->drawable = true;
    emitter->setChildImage(defaultImage);
    emitter->start();
}

void ofApp::setupGUI() {
    // Player GUI setup
    playerGui.setup("Player Settings");
    playerGui.setPosition(50, 200);
    playerGui.add(triangleEnergy.setup("Energy", 10, 1, 30));
    playerGui.add(triangleSpeed.setup("Speed", 400.0f, 5.0f, 1000.0f));
    playerGui.add(triangleScaleSlider.setup("Scale", 0.5f, 0.1f, 3.0f));
    playerGui.add(triangleRotationSpeed.setup("Rotation Speed", 120.0f, 0.0f, 400.0f));;
    playerGui.add(bShowHeadingToggle.setup("Show heading", false));;
    showHeading = bShowHeadingToggle;

    // Agents GUI setup
    agentsGui.setup("Agents Settings");
    agentsGui.setPosition(50, 400);
    agentsGui.add(rate.setup("Rate", 1.0f, 1.0f, 10.0f));
    agentsGui.add(life.setup("Life", 5.0f, .1f, 10.0f));
    agentsGui.add(velocity.setup("Velocity", 30.0f, 1.0f, 200.0f));
    agentsGui.add(scale.setup("Scale", .50f, .1f, 1.0f));
    agentsGui.add(rotationSpeed.setup("Rotation Speed", 1, 0, 10));
    agentsGui.add(levelSlider.setup("Level Slider", 0, 0, 3));
    agentsGui.add(bShowImageToggle.setup("Show Image", true));
    agentsGui.add(numberOfAgents.setup("Number Of Agents Spawned", 2, 0, 10));
}

//--------------------------------------------------------------
void ofApp::update() {
    if (gameOver) {
        return;
    }

    ofSeedRandom();


    explosionEmitter.setLifespan(lifespan);
    explosionEmitter.setVelocity(ofVec3f(bulletsVelocity->x, bulletsVelocity->y, bulletsVelocity->z));
    explosionEmitter.setRate(rate);
    explosionEmitter.setParticleRadius(explosionRadius);


    explosionRingEmitter.setLifespan(lifespan);
    explosionRingEmitter.setRate(rate);
    explosionRingEmitter.setParticleRadius(explosionRadius);



    explosionEmitter.update();
    explosionRingEmitter.update();


    // update heading bool
    showHeading = bShowHeadingToggle;
    
    // emitter
    particleBulletEmitter.position = tri.pos;
    particleBulletEmitter.directionalRotation = tri.rotation;
    
    triSpeed = triangleSpeed;
    

    particleBulletEmitter.update(); // Update the emitter

    //gravityValue = gravity;
    velocityValue = bulletsVelocity;

    newMinimumValue = glm::vec3(float(minimum1), float(minimum2), float(minimum3));
    newMaximumValue = glm::vec3(float(maximum1), float(maximum2), float(maximum3));

    particleBulletEmitter.setVelocity(velocityValue);
    particleBulletEmitter.setLifespan(lifespan);
    particleBulletEmitter.setRate(rate);
    particleBulletEmitter.setParticleRadius(bulletRadius);

    checkBounds();
    updateGameStatus();

    if (gameStarted && !gameOver) {
        handlePlayerMovement();
        updateEmitterSettings();
        updateSprites();
        checkGameOver();
    }
}

void ofApp::checkBounds() {
    float halfWidth = tri.scale.x * (tri.getWidth() / 2.0f);
    float halfHeight = tri.scale.y * (tri.getHeight() / 2.0f);

    if (tri.pos.x - halfWidth < 0) {
        tri.pos.x = halfWidth;
    } else if (tri.pos.x + halfWidth > ofGetWidth()) {
        tri.pos.x = ofGetWidth() - halfWidth;
    }

    if (tri.pos.y - halfHeight < 0) {
        tri.pos.y = halfHeight;
    } else if (tri.pos.y + halfHeight > ofGetHeight()) {
        tri.pos.y = ofGetHeight() - halfHeight;
    }
}

void ofApp::updateGameStatus() {
    emitter->getPlayerPosition(tri.pos);
    elapsedTime = ofGetElapsedTimef() - gameStartTime;

    float scaleValue = triangleScaleSlider;
    tri.scale = glm::vec3(scaleValue, scaleValue, scaleValue);
}

void ofApp::handlePlayerMovement() {
    if (isRotatingLeft) {
        tri.angularVelocity = -triangleRotationSpeed;
    } else if (isRotatingRight) {
        tri.angularVelocity = triangleRotationSpeed;
    }

    if (isMovingForward) {
        applyThrust(-1);
    } else if (isMovingBackward) {
        applyThrust(1);
    }

    integrate(triangleSpeed);
}

void ofApp::applyThrust(int direction) {
    float angleRad = ofDegToRad(tri.rotation + 90);
    glm::vec3 facingDirection = glm::vec3(cos(angleRad), sin(angleRad), 0);
    tri.force += facingDirection * glm::vec3(triSpeed * direction, triSpeed * direction, 0);
}

void ofApp::updateEmitterSettings() {
    if (levelSlider == 0) {
        setEmitterConfiguration(rate, life, velocity, scale);
    } else if (levelSlider == 1) {
        setEmitterConfiguration(1, 5, 30, 0.20);
    } else if (levelSlider == 2) {
        setEmitterConfiguration(3, 6, 70, 0.25);
    } else if (levelSlider == 3) {
        setEmitterConfiguration(5, 7, 100, 0.45);
    }

    emitter->setRate(rate);
    emitter->setLifespan(life * 1000);
    emitter->setVelocity(ofVec3f(velocity, 0));
    emitter->update(tri.pos, numberOfAgents);
}

void ofApp::setEmitterConfiguration(float newRate, float newLife, float newVelocity, float newScale) {
    rate = newRate;
    life = newLife;
    velocity = newVelocity;
    scale = newScale;
}

void ofApp::updateSprites() {
    // Update each sprite's visibility and position
    for (int i = 0; i < emitter->sys->sprites.size(); i++) {
        emitter->sys->sprites[i].bShowImage = bShowImageToggle;

        float sc = scale;
        emitter->sys->sprites[i].scale = glm::vec3(sc, sc, sc);
        emitter->moveSprite(&emitter->sys->sprites[i], tri.pos, rotationSpeed);

        // Check if the sprite is inside the triangle
        if (tri.inside(emitter->sys->sprites[i].pos)) {
            handleSpriteExplosion(i);
            emitter->sys->remove(i);
            triEnergy -= 1;
            i--; // Adjust index after removal
            playCollisionSound();
            triangleEnergy = triEnergy;

        }
    }
    emitter->update(tri.pos, numberOfAgents);
    checkParticleCollisions();
}

void ofApp::handleSpriteExplosion(int i){
    explosionEmitter.position = emitter->sys->sprites[i].pos;
//        explosionEmitter.directionalRotation = emitter->sys->sprites[i].rotation;

    explosionRingEmitter.position = emitter->sys->sprites[i].pos;

    explosionEmitter.sys->reset();
    explosionEmitter.start();

    explosionRingEmitter.sys->reset();
    explosionRingEmitter.start();
}

void ofApp::checkParticleCollisions() {
    for (int j = 0; j < particleBulletEmitter.sys->particles.size(); j++) {
        auto& particle = particleBulletEmitter.sys->particles[j];

        glm::vec3 particlePos = particle.position;

        for (int k = 0; k < emitter->sys->sprites.size(); k++) {
            auto& sprite = emitter->sys->sprites[k];

            glm::vec3 spritePos = sprite.pos;
            float halfWidth = sprite.width / 2.0f;
            float halfHeight = sprite.height / 2.0f;

            bool collisionX = (particlePos.x >= spritePos.x - halfWidth) && (particlePos.x <= spritePos.x + halfWidth);
            bool collisionY = (particlePos.y >= spritePos.y - halfHeight) && (particlePos.y <= spritePos.y + halfHeight);

            if (collisionX && collisionY) {
                handleSpriteExplosion(k);
                particleBulletEmitter.sys->remove(j);
                emitter->sys->remove(k);
                triEnergy += 1;
                triangleEnergy = triEnergy;
                playCollisionSound();
                break;
            }
        }
    }
}


void ofApp::checkGameOver() {
    if (triEnergy <= 0) {
        gameOver = true;
        gameStarted = false;
    }
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofSetBackgroundColor(ofColor::black);
    if (gameStarted) {
        gui.draw();
        ofPushMatrix();
        ofRotate(90, 0, 0, 1);
        ofSetLineWidth(1);
        ofSetColor(ofColor::dimGrey);
        ofPopMatrix();
        drawGameScene();
    } else if (!gameStarted && !gameOver) {
        drawStartScreen();
    } else if (gameOver) {
        drawGameOverScreen();
    }

    drawGUI();
}

void ofApp::drawGameScene() {
    backgroundImage.draw(0, 0, ofGetWidth(), ofGetHeight());
    tri.draw();
    emitter->draw();
    
    ofSetColor(ofColor::dimGrey);


    explosionEmitter.draw();
    explosionRingEmitter.draw();

    particleBulletEmitter.draw();
    tri.drawHeading(showHeading);

    drawTimeElapsedText(gameStarted, elapsedTime);
    drawScreenBounds();
    drawPlayerEnergyText(gameStarted, triangleEnergy);
    drawFrameRateText();
    drawInstructionsText();
}

//--------------------------------------------------------------
void ofApp::drawScreenBounds() {
    ofSetColor(255, 0, 0, 255);
    ofDrawRectangle(0, 0, ofGetWidth(), 10);
    ofDrawRectangle(0, ofGetHeight() - 10, ofGetWidth(), 10);
    ofDrawRectangle(0, 0, 10, ofGetHeight());
    ofDrawRectangle(ofGetWidth() - 10, 0, 10, ofGetHeight());
}


void ofApp::drawStartScreen() {
    ofSetColor(ofColor::white);
    ofDrawBitmapString("Avoid the enemies for as long as you can!", ofGetWindowWidth() / 2 - 200, ofGetWindowHeight() / 2 - 70);
    ofDrawBitmapString("You can show all the panels by pressing 'h'.", ofGetWindowWidth() / 2 - 200, ofGetWindowHeight() / 2 - 50);
    ofDrawBitmapString("Press SPACE to begin the game", ofGetWindowWidth() / 2 - 150, ofGetWindowHeight() / 2);
}

void ofApp::drawGameOverScreen() {
    if(gameOver){
        ofSetColor(255, 0, 0);
        std::string gameOverText = "Game Over! Time Elapsed: " + ofToString(elapsedTime, 2) + " seconds";
        float textWidth = gameOverText.size() * 8;
        float xPosition = (ofGetWidth() - textWidth) / 2;
        float yPosition = ofGetHeight() / 2;
        ofDrawBitmapString(gameOverText, xPosition, yPosition);

        std::string restartText = "Press SPACE to restart the game";
        float restartTextWidth = restartText.size() * 8;
        float restartXPosition = (ofGetWidth() - restartTextWidth) / 2;
        float restartYPosition = yPosition + 30;
        ofDrawBitmapString(restartText, restartXPosition, restartYPosition);
    }
}

void ofApp::drawPlayerEnergyText(bool gameStarted, float triangleEnergy) {
    ofSetColor(255);

    std::string displayText;
    if (gameStarted) {
        displayText = "Player Energy: " + ofToString(triangleEnergy, 2);
    } else {
        displayText = "Game Over when Player Energy Reaches 0";
    }

    float textWidth = displayText.size() * 8;
    float xPosition = (ofGetWidth() - textWidth) / 2;
    float yPosition = 100;

    ofDrawBitmapString(displayText, xPosition, yPosition);
}

void ofApp::drawTimeElapsedText(bool gameStarted, float elapsedTime) {
    ofSetColor(255);

    std::string displayText;
    if(gameStarted){
        displayText = "Time Elapsed: " + ofToString(elapsedTime, 2) + " seconds";
    } else {
        displayText = "Press SPACE to start the game";
    }

    float textWidth = displayText.size() * 8;
    float xPosition = (ofGetWidth() - textWidth) / 2;
    float yPosition = 50;

    ofDrawBitmapString(displayText, xPosition, yPosition);
}

void ofApp::drawFrameRateText() {
    std::string fpsText = "FPS: " + ofToString(ofGetFrameRate(), 2);

    float textWidth = fpsText.size() * 16;
    float xPosition = (ofGetWidth() - textWidth);
    float yPosition = 50;
    ofDrawBitmapString(fpsText, xPosition, yPosition);
}

void ofApp::drawInstructionsText() {
    std::string instructions =
        "Instructions:\n"
        "Press R to restart the game\n"
        "Press H to hide panels\n"
        "Press LEFT or RIGHT arrows to rotate\n"
        "Press UP to move forward\n"
        "Press DOWN to move backward\n"
        "Press F for fullscreen";

    float xPosition = 50;
    float yPosition = 50;

    ofDrawBitmapString(instructions, xPosition, yPosition);
}


void drawTimeElapsedText(bool gameStarted, float elapsedTime) {
    ofSetColor(255);

    std::string displayText;
    if(gameStarted){
        displayText = "Time Elapsed: " + ofToString(elapsedTime, 2) + " seconds";
    } else {
        displayText = "Press SPACE to start the game";
    }

    float textWidth = displayText.size() * 8;
    float xPosition = (ofGetWidth() - textWidth) / 2;
    float yPosition = 50;

    ofDrawBitmapString(displayText, xPosition, yPosition);

}
void ofApp::drawGUI() {
    if (showingGUI) {
        playerGui.draw();
        agentsGui.draw();
        gui.draw();
    }
}



//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    glm::vec3 pos = glm::vec3(x, y, 0);

    if (isDraggingItem) {
        glm::vec3 delta = pos - mousePrevPos;
            tri.pos += delta;
        mousePrevPos = pos;
    }
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    glm::vec3 mousePos = glm::vec3(x, y, 0);
        isDraggingItem = tri.inside(mousePos);
    if (isDraggingItem){
        mousePrevPos = mousePos;
    }
}


//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
}

void ofApp::keyPressed(int key){
    float movementAmount = triangleSpeed;

    switch (key) {
        case 'f':
        case 'F':
            ofToggleFullscreen();
            break;
            
        case ' ': // Space bar case
            if(!gameStarted || gameOver) {
                resetGame();
                showingGUI = true;
            }
            break;
            
        case 'r': // r case
        case 'R': // Space bar case
            if(gameStarted) {
                resetGame();
            }
            break;

        case 'x': // r case
        case 'X': // Space bar case
            explosionEmitter.sys->reset();
            explosionEmitter.start();

            explosionRingEmitter.sys->reset();
            explosionRingEmitter.start();
            break;

        case 'h': // i case
        case 'H': // I case
            if(showingGUI) {
                showingGUI = false;
            } else {
                showingGUI = true;
            }
            break;


        // Rotation Keys
        case OF_KEY_LEFT:
            isRotatingLeft = true;
            playThrusterSound();
            break;
        case OF_KEY_RIGHT:
            isRotatingRight = true;
            playThrusterSound();
            break;

        // Moving Keys
        case OF_KEY_UP:
            //forwards movement
            isMovingForward = true;
            playThrusterSound();
            break;
        case OF_KEY_DOWN:
            //backwards movement
            isMovingBackward = true;
            playThrusterSound();
            break;

        default:
            break;
    }
}



//--------------------------------------------------------------
void ofApp::keyReleased(int key) {
    switch (key) {
    case OF_KEY_LEFT:
            isRotatingLeft = false;
            stopThrusterSound();
            break;
    case OF_KEY_RIGHT:
            isRotatingRight = false;
            stopThrusterSound();
            break;
    case OF_KEY_UP:
            isMovingForward = false;
            stopThrusterSound();
            break;
    case OF_KEY_DOWN:
            isMovingBackward = false;
            stopThrusterSound();
        break;
    case OF_KEY_ALT:
        break;
    case OF_KEY_CONTROL:
        break;
    case OF_KEY_SHIFT:
        break;
    }
}

void ofApp::resetGame() {
    gameStarted = true;
    gameOver = false;
    gameStartTime = ofGetElapsedTimef();
    elapsedTime = 0;

    gameStartTime = ofGetElapsedTimef();

    // reset agents settings
    levelSlider = 0;
    triangleEnergy = 10;
    numberOfAgents = 2;
    rate = 1;
    life = 5;
    velocity =  30.0f;
    scale = .50f;
    rotationSpeed =  1;


    // reset triangle/player settings
    currentSpeed = 0;
    triEnergy = triangleEnergy;
    triangleSpeed = 400.0f;
    triangleScaleSlider = 0.5f;
    triangleRotationSpeed = 120.0f;
    bShowHeadingToggle = false;
    tri = TriangleShape();

    emitter->clearSprites();
}
