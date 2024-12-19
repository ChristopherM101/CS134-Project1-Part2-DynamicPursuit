#ifndef TriangleShape_h
#define TriangleShape_h
#include "MainPlayerShape.h"

class TriangleShape : public MainPlayerShape {
    public:
    
    float getWidth();
    float getHeight();
    glm::vec3 getPosition() const;
    
    TriangleShape();
        bool inside(glm::vec3 p) override;
        void draw() override;
    

    glm::vec3 velocity = glm::vec3(0, 0, 0);
    glm::vec3 acceleration = glm::vec3(0, 0, 0);
    glm::vec3 force = glm::vec3(0, 0, 0);

    
    float mass = 1.0;
    float damping = .99;

    float angularVelocity = 0.0f;
    float angularAcceleration = 0.0f;

    void drawHeading(bool b);

    void updateEndPoint();
    
    glm::vec3 headingVector;

    bool showHeadingVector = false;

    void calculateHeadingVector();
};


#endif /* TriangleShape_h */
