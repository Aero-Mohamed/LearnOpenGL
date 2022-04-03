#pragma once

#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


class WindowObject {
private:
    int m_width;
    int m_height;
    int m_aspectRatioDivider;

    glm::mat4 m_projMat;

public:
    WindowObject(int width, int height)
        :m_width(width), m_height(height)
    {
        this->estimateAspectRatioDivider(width, height);
        this->updateProjMat();
    }
    ~WindowObject() {};

    inline int getWidth() { return m_width; }
    inline int getHeight() { return m_height; }
    inline glm::mat4 getProjMat() { return m_projMat; }

    void setWidth(int width) {
        m_width = width;
    }

    void setHeight(int height) {
        m_height = height;
    }

    void estimateAspectRatioDivider(int width, int height) {
        if (height == 0)
            m_aspectRatioDivider = width;
        else
            this->estimateAspectRatioDivider(height, width % height);
    }

    void updateProjMat() {

        float a = (float)(m_width / (2.0 * m_aspectRatioDivider));
        float b = (float)(m_height / (2.0 * m_aspectRatioDivider));

        m_projMat = glm::ortho(
            -a / std::max(a, b), // -ve x
            a / std::max(a, b),  // +ve x
            -b / std::max(a, b), // -ve y
            b / std::max(a, b),  // +ve y
            -1.0f,               // -ve z
            1.0f                 // +ve z
        ); 
    }

    void update(int width, int height) {
        this->estimateAspectRatioDivider(width, height);
        m_width = width;
        m_height = height;
        this->updateProjMat();
    }
};

