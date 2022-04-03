#include "Camera.h"
#include <iostream>


void Camera::initMouseInput()
{
    if (m_allowMouseInput) {
        glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        glfwSetWindowUserPointer(m_window, this);
        glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xpos, double ypos) {
            Camera* thisCamera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
            thisCamera->processMouseInput(xpos, ypos);
        });
    }
    
}

Camera::Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 front)
	:m_view(glm::mat4(1.0f))
{
	m_cameraPos = position;
	m_cameraFront = front;

    m_window = window;

    this->initMouseInput();

	this->update();
}

Camera::~Camera()
{
}

void Camera::update()
{
	m_view = glm::lookAt(
		m_cameraPos, m_cameraPos + m_cameraFront, glm::vec3(0.0f, 1.0f, 0.0f)
	);
}

void Camera::processInput()
{
    float ct = glfwGetTime();
    m_deltaTime = ct - m_lastFrameTime;
    m_lastFrameTime = ct;

    m_cameraSpeed = 2.5f * m_deltaTime;
    if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
        m_cameraPos += m_cameraSpeed * m_cameraFront;
    if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
        m_cameraPos -= m_cameraSpeed * m_cameraFront;
    if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
        m_cameraPos -= glm::normalize(glm::cross(m_cameraFront, m_upVec)) *
        m_cameraSpeed;
    if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
        m_cameraPos += glm::normalize(glm::cross(m_cameraFront, m_upVec)) *
        m_cameraSpeed;

    this->update();
}

void Camera::processMouseInput(double xpos, double ypos)
{
    if (m_firstMouseEnter) {
        m_lastX = xpos;
        m_lastY = ypos;
        m_firstMouseEnter = false;
    }

    float xoffset = xpos - m_lastX;
    float yoffset = m_lastY - ypos;
    m_lastX = xpos;
    m_lastY = ypos;

    m_yaw += xoffset * m_mouseSensitivity;
    m_pitch += yoffset * m_mouseSensitivity;

    if (m_pitch > 89.9f)
        m_pitch = 89.9f;
    if (m_pitch < -89.9f)
        m_pitch = -89.9f;

    glm::vec3 direction;
    direction.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    direction.y = sin(glm::radians(m_pitch));
    direction.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_cameraFront = glm::normalize(direction);
    
    this->update();
}
