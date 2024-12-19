
#ifndef ImageShape_h
#define ImageShape_h
#include "MainPlayerShape.h"

class ImageShape : public MainPlayerShape {
public:
        ImageShape();
        void loadImage(std::string imagePath);  // Load image
        void draw() override;
        bool insideImage(glm::vec3 p);

        int getWidth() const;  // Get width of image
        int getHeight() const;  // Get height of image
        glm::vec3 localCenterPointImg;
        glm::vec3 localEndPointImg;
    bool showHeading;

    // gets the 4 points of the image

        glm::vec3 getTransformedCorner(const glm::vec3& corner) {
            glm::vec4 transformedCorner = getTransform() * glm::vec4(corner, 1.0f);
            return glm::vec3(transformedCorner.x, transformedCorner.y, transformedCorner.z);
        }
        glm::vec3 getTopLeftCorner() {
            return getTransformedCorner(glm::vec3(0, 0, 0));
        }

        glm::vec3 getTopRightCorner() {
            return getTransformedCorner(glm::vec3(ship.getWidth(), 0, 0));
        }

        glm::vec3 getBottomRightCorner() {
            return getTransformedCorner(glm::vec3(ship.getWidth(), ship.getHeight(), 0));
        }
        glm::vec3 getBottomLeftCorner() {
            return getTransformedCorner(glm::vec3(0, ship.getHeight(), 0));
        }

private:
    ofImage ship;
};

#endif /* ImageShape_h */
