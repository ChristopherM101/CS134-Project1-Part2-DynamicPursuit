
#pragma once
#include "ImageShape.h"
#include "ofApp.h"

//Tried many ways to implement this function, this was the closest I could get to it working
bool ImageShape::insideImage(glm::vec3 p0) {
    // Get the transformed corner points
    glm::vec3 topLeft = getTopLeftCorner();
    glm::vec3 topRight = getTopRightCorner();
    glm::vec3 bottomRight = getBottomRightCorner();
    glm::vec3 bottomLeft = getBottomLeftCorner();

    // Check if the mouse is within the bounding box
    return p0.x >= topLeft.x && p0.x <= bottomRight.x &&
           p0.y >= topLeft.y && p0.y <= bottomRight.y;
}

ImageShape::ImageShape() {
    pos = glm::vec3(ofGetWindowWidth() / 2, ofGetWindowHeight() / 2, 0);  // Position at the center of the window
    scale = glm::vec3(1, 1, 1);
    showHeading = false;
    centerPointImg = glm::vec3(ship.getHeight() / 2.0, ship.getWidth() / 2.0, 0);
    endPointImg = glm::vec3(ship.getHeight(), ship.getWidth() / 2.0 , 0);
}

void ImageShape::loadImage(std::string imagePath) {
    ship.load(imagePath);  // Load the image

    // Center point is at half the width and half the height
    glm::vec3 localCenterPointImg = glm::vec3(ship.getWidth() / 2.0f, ship.getHeight() / 2.0f, 0);

    // Top midpoint is at half the width and y = 0 (top edge)
    glm::vec3 localEndPointImg = glm::vec3(ship.getWidth() / 2.0f, 0, 0);
}

void ImageShape::draw() {
    ofPushMatrix();

    // Apply transformations
    ofTranslate(pos);
    ofTranslate(ship.getWidth() / 2.0f, ship.getHeight() / 2.0f);
    ofRotateDeg(rotation);
    ofScale(scale.x, scale.y);  // Apply scaling
    ofTranslate(-ship.getWidth() / 2.0f, -ship.getHeight() / 2.0f);

    // Draw the image
    ofSetColor(255);  // Set color to white
    ship.draw(0, 0);

    // Draw heading vector if true
    if (showHeading) {
        glm::vec3 localCenterPointImg = glm::vec3(ship.getWidth() / 2.0f, ship.getHeight() / 2.0f, 0);
        glm::vec3 headingVector = glm::normalize(glm::vec3(0, -1, 0));
        glm::vec3 localEndPointImg = localCenterPointImg + headingVector * (ship.getHeight() * 2.0f);
        ofSetColor(ofColor::black);
        ofDrawLine(localCenterPointImg, localEndPointImg);
    }

    // Draw the bounding rectangle
    ofNoFill();  // Set to no fill to only draw the outline
    ofSetColor(ofColor::red);  // Set color to red for visibility
    ofDrawRectangle(0, 0, ship.getWidth(), ship.getHeight());

    ofPopMatrix();
}
