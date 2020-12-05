/*
 * Scene.h
 *
 *  Created on: Dec 5, 2020
 *      Author: code
 */

#ifndef SCEN_H_
#define SCEN_H_

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>


namespace CGL {

struct Actor {
	Model model;
	glm::mat4 modelMatrix;
	ShaderProgram* shaderProgram;
};

class Scen {
public:
	Scen();
	Scen(Camera camera);
	virtual ~Scen();

	void AddShaderProgram(ShaderProgram shaderProgram);
	ShaderProgram* GetShaderProgram(unsigned int index);
	void AddActor(Model model, ShaderProgram* shaderProgram, glm::mat4 modelMatrix = glm::mat4(1.f));

	void RunScene(GLFWwindow* window, float deltaFrame, bool freeCam);

private:
	std::vector<Actor> actors;
	std::vector<ShaderProgram> shaderPrograms;
	Camera camera;
	bool freeCam;
	float scr_width; float scr_height;

	void draw();
	void updateScenParameters(GLFWwindow* window);
	void handleKeyboardInput(GLFWwindow* window, float deltaTime);
	void handleMouseInput(GLFWwindow* window);
};

} /* namespace CGL */

#endif /* SCEN_H_ */
