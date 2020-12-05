/*
 * Scen.cpp
 *
 *  Created on: Dec 5, 2020
 *      Author: code
 */

#include "Scen.h"

namespace CGL {

Scen::Scen() {
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;
}

Scen::Scen(Camera camera) : camera(camera) {
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;
}

Scen::~Scen() {
	// TODO Auto-generated destructor stub
}

void Scen::AddShaderProgram(ShaderProgram shaderProgram) {
	shaderPrograms.push_back(shaderProgram);
}

ShaderProgram* Scen::GetShaderProgram(unsigned int index) {
	return &shaderPrograms[index];
}

void Scen::AddActor(Model model, ShaderProgram* shaderProgram, glm::mat4 modelMatrix) {
	Actor actor;
	actor.model = model;
	actor.modelMatrix = modelMatrix;
	actor.shaderProgram = shaderProgram;
	actors.push_back(actor);
}

void Scen::RunScene(GLFWwindow* window, float deltaTime, bool freeCam) {
	this->freeCam = freeCam;
	updateScenParameters(window);
	handleKeyboardInput(window, deltaTime);
	handleMouseInput(window);
	draw();
}

void Scen::updateScenParameters(GLFWwindow* window) {
	// update scr_width and scr_height fields
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	scr_width = (float)width;
	scr_height = (float)height;
}

void Scen::handleKeyboardInput(GLFWwindow* window, float deltaFrame){
	// for moving a camera
	if(freeCam) camera.KeyInputProcess(window, deltaFrame);
	else {
		// TODO control over a actor
	}
}

void Scen::handleMouseInput(GLFWwindow* window) {
	// for moving a camera
	if(freeCam) camera.MouseInputProcess(window);
}

void Scen::draw() {
	ShaderProgram shaderProg;

	glm::mat4 viewMatrix = camera.GetViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), scr_width/scr_height, .1f, 100.f);

	for(Actor &actor : actors) {
		shaderProg = *actor.shaderProgram;

		shaderProg.SetUniformMatrix4f("model", actor.modelMatrix);
		shaderProg.SetUniformMatrix4f("view", viewMatrix);
		shaderProg.SetUniformMatrix4f("projection", projectionMatrix);

		actor.model.Draw(shaderProg);
	}
}

} /* namespace CGL */
