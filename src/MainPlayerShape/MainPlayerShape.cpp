
#include "ofMain.h"
#include "ofxGui.h"
#include "MainPlayerShape.h"

glm::mat4 MainPlayerShape::getTransform() {
    glm::mat4 trans = glm::translate(glm::mat4(1.0), glm::vec3(pos));
    glm::mat4 rot = glm::rotate(glm::mat4(1.0), glm::radians(rotation), glm::vec3(0, 0, 1));
    glm::mat4 scaleMat = glm::scale(glm::mat4(1.0), this->scale);

    return (trans * rot * scaleMat);
}
