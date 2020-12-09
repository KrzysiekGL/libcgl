#include "Scene.h"

namespace CGL {

Scene::Scene() {
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;
}

Scene::Scene(Camera camera) : camera(camera) {
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;
}

Scene::~Scene() {
	// TODO Auto-generated destructor stub
}

void Scene::AddShaderProgram(ShaderProgram shaderProgram) {
	shaderPrograms.push_back(shaderProgram);
}

ShaderProgram& Scene::GetShaderProgram(unsigned int index) {
	return &shaderPrograms[index];
}

void Scene::AddActor(Model model, ShaderProgram* shaderProgram, glm::mat4 modelMatrix) {
	Actor actor;
	actor.model = model;
	actor.modelMatrix = modelMatrix;
	actor.shaderProgram = shaderProgram;
	actors.push_back(actor);
}

void Scene::RunScene(GLFWwindow* window, float deltaTime, bool freeCam) {
	this->freeCam = freeCam;
	updateSceneParameters(window);
	handleKeyboardInput(window, deltaTime);
	handleMouseInput(window);
	draw();
}

void Scene::updateSceneParameters(GLFWwindow* window) {
	// update scr_width and scr_height fields
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	scr_width = (float)width;
	scr_height = (float)height;
}

void Scene::handleKeyboardInput(GLFWwindow* window, float deltaFrame){
	// for moving a camera
	if(freeCam) camera.KeyInputProcess(window, deltaFrame);
	else {
		// TODO control over a actor
	}
}

void Scene::handleMouseInput(GLFWwindow* window) {
	// for moving a camera
	if(freeCam) camera.MouseInputProcess(window);
}

void Scene::draw() {
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
