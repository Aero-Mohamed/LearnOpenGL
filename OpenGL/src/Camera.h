#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera
{
private:
	GLFWwindow* m_window;
	glm::mat4 m_view;
	glm::vec3 m_cameraPos;
	glm::vec3 m_cameraFront;
	glm::vec3 m_upVec = glm::vec3(0.0f, 1.0f, 0.0f);

	float m_cameraSpeed;
	float m_deltaTime = 0.0f;
	float m_lastFrameTime = 0.0f;
	float m_currentFrameTime = 0.0f;

	bool m_allowMouseInput = false;
	bool m_firstMouseEnter = !false;
	double m_lastX;
	double m_lastY;
	float m_yaw;
	float m_pitch;
	float m_mouseSensitivity = 0.1f;

	void initMouseInput();

	
public:
	Camera(GLFWwindow* window, glm::vec3 position, glm::vec3 front);
	~Camera();

	void update();
	void processInput();
	void processMouseInput(double xpos, double ypos);
	inline glm::mat4 getViewMat() { return m_view; }
	inline void toggleMouseInput() {
		m_allowMouseInput = !m_allowMouseInput; 
		this->initMouseInput();
	}
};

