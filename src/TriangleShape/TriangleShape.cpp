#pragma once
#include "MainPlayerShape.h"
#include "TriangleShape.h"
#include "ofApp.h"


bool TriangleShape::inside(glm::vec3 p0) {
    glm::vec3 p = glm::inverse(getTransform()) * glm::vec4(p0, 1);
    glm::vec3 v1 = glm::normalize(verts[0] - p);
    glm::vec3 v2 = glm::normalize(verts[1] - p);
    glm::vec3 v3 = glm::normalize(verts[2] - p);

    float a1 = glm::orientedAngle(v1, v2, glm::vec3(0, 0, 1));
    float a2 = glm::orientedAngle(v2, v3, glm::vec3(0, 0, 1));
    float a3 = glm::orientedAngle(v3, v1, glm::vec3(0, 0, 1));
    if (a1 < 0 && a2 < 0 && a3 < 0) return true;
else return false;
}

TriangleShape::TriangleShape() {
    verts.push_back(glm::vec3(50, 50, 0)); // botright
    verts.push_back(glm::vec3(0, -100, 0)); // top/head
    verts.push_back(glm::vec3(-50, 50, 0)); // botleft

    scale = glm::vec3(1, 1, 1);
    pos = glm::vec3(ofGetWindowWidth() / 2.0, ofGetWindowHeight() / 2.0, 0);

    // calculates the length of the triangles sides in order to find the height
    triangleBaseLength = glm::distance(verts[0], verts[2]);
    triangleEqualSideLength =glm::distance(verts[0], verts[1]);

    // finds height of the triangle
    height = sqrt(triangleEqualSideLength * triangleEqualSideLength - (triangleBaseLength * triangleBaseLength / 4.0f));

    // gets the center point
    center = (verts[0] + verts[1] + verts[2])/3;

    // gets direction vector
    directionVector = glm::normalize(verts[1] - center);

    // calculates the endpoint according to direction and the height of triangle
    endPoint = center + directionVector * (height * 2.0);
}

void TriangleShape::draw() {
    ofPushMatrix();
    ofMultMatrix(getTransform());

    ofFill();
    ofSetColor(color);
    ofDrawTriangle(verts[0], verts[1], verts[2]);

    ofPopMatrix();
}

void TriangleShape::drawHeading(bool b) {
    ofPushMatrix();
    ofMultMatrix(getTransform());

    if(b){
        ofFill();
        ofSetColor(255, 255, 0);
        ofDrawLine(center, endPoint);
    }
    ofPopMatrix();
}

// Got these ideas from Chat GPT
float TriangleShape::getWidth() {
    // Calculate the width as the distance between the leftmost and rightmost vertices
    float minX = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();

    for (const auto& vert : verts) {
        float worldX = (getTransform() * glm::vec4(vert, 1.0)).x; // Transform the vertex to world coordinates
        minX = std::min(minX, worldX);
        maxX = std::max(maxX, worldX);
    }
    
    return maxX - minX; // Width is the difference between max and min x-coordinates
}

float TriangleShape::getHeight() {
    // Height can be defined as the vertical distance from the lowest vertex to the highest vertex
    float minY = std::numeric_limits<float>::max();
    float maxY = std::numeric_limits<float>::min();

    for (const auto& vert : verts) {
        float worldY = (getTransform() * glm::vec4(vert, 1.0)).y; // Transform the vertex to world coordinates
        minY = std::min(minY, worldY);
        maxY = std::max(maxY, worldY);
    }
    
    return maxY - minY; // Height is the difference between max and min y-coordinates
}

glm::vec3 TriangleShape::getPosition() const {
    return center; // Return the center of the triangle
}
