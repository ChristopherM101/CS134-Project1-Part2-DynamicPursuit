
#pragma once
#include "ofMain.h"
#include "ofxGui.h"

class MainPlayerShape {
public:
    // variables needed for the header vector
    float height;
    float triangleEqualSideLength;
    float triangleBaseLength;
    glm::vec3 center;
    glm::vec3 endPoint;
    glm::vec3 directionVector;
    std::vector<glm::vec3> verts;

    glm::vec3 centerPointImg;
    glm::vec3 endPointImg;

    glm::mat4 getTransform();

    // transformations
    //
    glm::vec3 pos = glm::vec3(0, 0, 0);
    float rotation = 0;
    glm::vec3 scale = glm::vec3(1, 1, 1);

    // methods of shape
    //
    virtual bool inside(glm::vec3 p) { return false; }
    virtual ofColor getColor() { return color; }

    // OF methods
    //
    virtual void draw() {}

    ofColor color = ofColor::orangeRed;
};
