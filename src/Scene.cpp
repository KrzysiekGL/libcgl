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
}

void Scene::AddShaderProgram(std::string name, ShaderProgram shaderProgram) {
	shaderPrograms[name] = shaderProgram;
}

void Scene::AddShaderProgram(std::string name, std::string vert, std::string frag) {
	AddShaderProgram(name, ShaderProgram(vert.c_str(), frag.c_str()));
}

void Scene::AddActor(std::string actor_name, Model model) {
	Actor actor;
	actor.model = model;

	actors[actor_name] = actor;
}

void Scene::AddActor(std::string actor_name, std::string model_path) {
	AddActor(actor_name, Model(model_path));
}


void Scene::AddActorCopy(std::string actorName, std::string shaderProgName, glm::mat4 modelMatrix) {
	// create a copy of the Actor
	ActorCopy actorCopy;

	std::map<std::string, ShaderProgram>::iterator it = shaderPrograms.find(shaderProgName);
	if(it != shaderPrograms.end())
		actorCopy.shaderProgram = it;
	else {
#ifdef _DEBUG
		printf("ERROR::SCENE::No %s ShaderProgram exist", shaderProgName);
#endif //_DEBUG
		return;
	}

	actorCopy.modelMatrix = modelMatrix;

	actors[actorName].instances.push_back(actorCopy);
}

void Scene::RunScene(GLFWwindow* window, float deltaTime, bool freeCam) {
	// freeCam for the Camera
	this->freeCam = freeCam;
	// check for size of a frame buffer
	updateSceneParameters(window);
	// handle inputs by the Camera
	handleKeyboardInput(window, deltaTime);
	handleMouseInput(window);
	// render everything
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
		// TODO control over an actor
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

	std::map<std::string, Actor>::iterator it = actors.begin();
	// Iterate over all actors
	while(it != actors.end()){
		// Iterate over all instances of an actor and render each one of them
		Actor actor = it->second;
		for (ActorCopy &AC : actor.instances) {
			shaderProg = AC.shaderProgram->second;

			shaderProg.SetUniformMatrix4f("model", AC.modelMatrix);
			shaderProg.SetUniformMatrix4f("view", viewMatrix);
			shaderProg.SetUniformMatrix4f("projection", projectionMatrix);

			actor.model.Draw(shaderProg);
		}
		it++;
	}
}

/* Getters */
std::vector<std::string> Scene::GetShaderProgramNames() {
	std::vector<std::string> shaderProgNames;

	std::map<std::string, ShaderProgram>::iterator it = shaderPrograms.begin();
	while(it != shaderPrograms.end()) {
		shaderProgNames.push_back(it->first);
		it++;
	}

	return shaderProgNames;
}

std::vector<std::string> Scene::GetActorNames() {
	std::vector<std::string> actorNames;

	std::map<std::string, Actor>::iterator it = actors.begin();
	while(it != actors.end()) {
		actorNames.push_back(it->first);
		it++;
	}

	return actorNames;
}

} /* namespace CGL */
