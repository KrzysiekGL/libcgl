/*
 * Camera class was designed to simulate a camera behavior inside a OpenGL context
 * All it does is:
 * - determine UP, FRONT, and RIGHT of a camera
 * - get the VIEW MATRIX for transformation pipeline
 * - handle keyboard and mouse input to modify camera parameters accordingly
 */
#ifndef CAMERAH
#define CAMERAH

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace CGL {
	class Camera {
	public:
		/*
		 * Create a camera and assign default parameter's values
		 */
		Camera(glm::vec3 cameraPos, float pitch, float yaw, float camSensitivity, float camSpeed);
		Camera(glm::vec3 cameraPos);
		Camera();

		/*
		 * Get the VIEW MATRIX from a camera
		 */
		glm::mat4 GetViewMatrix() const;

		/*
		 * Get camera position
		 */
		glm::vec3 GetPosition() const;

		/*
		 * Get camera front vector
		 */
		glm::vec3 GetFront() const;

		/*
		 * Handle keyboard input
		 * Currently set to:
		 * W - move camera front
		 * S - move camera backwards
		 * A - strife camera left
		 * D - strife camera right
		 * L_CRTL - move camera up
		 * SPACE - move camera down
		 */
		void KeyInputProcess(GLFWwindow* window, float deltaTime);

		// TODO Implement mouse picking
		/*
		 * Handle mouse input
		 * Camera is using pitch and yaw of Euler angles (roll is not handled)
		 * Currently set to:
		 * M_VERT_UP - pitch camera up
		 * M_VERT_DOWN - pitch camera down
		 * M_HORIZ_LEFT - yaw camera left
		 * M_HORIZ_RIGHT - yaw camera right
		 */
		void MouseInputProcess(GLFWwindow* window);

		/*
		 * Change camera speed
		 */
		void SetCameraSpeed(float value);

	private:
		/*
		 * Update camera's parameters (fields)
		 * after they were changed, so the VIEW MATRIX
		 * will be changed as well
		 */
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
