#ifndef CAMERAH
#define CAMERAH

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CGL {
	class Camera {
	public:
		Camera(glm::vec3 cameraPos, float pitch, float yaw, float camSensitivity, float camSpeed);
		Camera(glm::vec3 cameraPos);
		Camera();
		~Camera();

		glm::mat4 GetViewMatrix();
		void KeyInputProcess(GLFWwindow* window, float deltaTime);
		// TODO Implement mouse picking
		void MouseInputProcess(GLFWwindow* window);

	private:
		void updateCameraVectors();

		float
			cameraSpeed,
			cameraSensitivity,
			pitch,
			yaw;

		glm::vec3
			cameraPos,
			cameraRevDir,
			cameraRight,
			cameraUp;
	};
} // namespace CGL

#endif // !CAMERAH 
