#include "Scene.h"

namespace CGL {

// - Constructors & Destructors
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
// -  END Constructors & Destructors

// - Public Methods
void Scene::AddShaderProgram(std::string name, ShaderProgram shaderProgram) {
	shaderProgramCollection[name] = shaderProgram;
}

void Scene::AddModel(std::string name, Model model) {
	modelCollection[name] = model;
}

bool Scene::AddActor(std::string model_name, std::string shaderProgram_name) {
	// Search if there are model_name and shaderProgram_name present
	std::map<std::string, Model>::iterator mit = modelCollection.find(model_name);
	if(mit == modelCollection.end()){
#ifdef _DEBUG
		std::cout << "ERROR::SCENE No " << model_name << " Model found in the scene\n";
#endif //_DEBUG
		return false;
	}
	std::map<std::string, ShaderProgram>::iterator spit = shaderProgramCollection.find(shaderProgram_name);
	if(spit == shaderProgramCollection.end()){
#ifdef _DEBUG
		std::cout << "ERROR::SCENE No " << shaderProgram_name << " ShaderProgram found in the Scene\n";
#endif
		return false;
	}

	/*
	 * If both are present, then create a new Actor,
	 * generate a name and put it into collection
	 */
	Actor actor; std::string actor_name = model_name;

	// parameters
	actor.modelIterator = mit;
	actor.shaderProgramIterator = spit;
	actor.modelMatrix = glm::mat4(1.f);
	actor.transparent = false;

	// name
	/*
 	 * Iterate over actor collection, and search for how
 	 * many actors has the same model. Then modify actor_name
 	 * so it has another number with regard to how many actors
 	 * with the same model there are.
 	 * For example: if there are already 3 actors with model "box"
 	 * then change actor_name from "box" to "box-3" (counting from 0)
	 */
	unsigned int number = 0;
	for(std::pair<std::string, Actor> a : actorCollection)
		if(a.second.modelIterator == mit)
			number++;
	actor_name = actor_name+"-"+std::to_string(number);

	// add Actor to collection
	actorCollection[actor_name] = actor;

	return true;
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

// -- Getters
std::vector<std::string> Scene::GetShaderProgramCollectionNames() {
	std::vector<std::string> names;
	std::map<std::string, ShaderProgram>::iterator it = shaderProgramCollection.begin();
	while(it != shaderProgramCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetModelCollectionNames() {
	std::vector<std::string> names;
	std::map<std::string, Model>::iterator it = modelCollection.begin();
	while(it != modelCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetActorCollectionNames() {
	std::vector<std::string> names;
	std::map<std::string, Actor>::iterator it = actorCollection.begin();
	while(it != actorCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}
// -- END Getters
// - END Public Methods

// - Private Methods
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
	// Get view and projection matrices for current frame from the Camera
	glm::mat4 viewMatrix = camera.GetViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), scr_width/scr_height, .1f, 100.f);

	// Iterate over all actors from collection and render them
	for(std::pair<std::string, Actor> element : actorCollection){
		// Prepare handles for Model and ShaderProgram
		const Actor actor = element.second;
		std::map<std::string, Model>::iterator mit = actor.modelIterator;
		std::map<std::string, ShaderProgram>::iterator spit = actor.shaderProgramIterator;

		// Get Model and ShaderProgram; Set parameters and render model
		Model* model = &mit->second;
		ShaderProgram* shader = &spit->second;

		shader->SetUniformMatrix4f("model", actor.modelMatrix);
		shader->SetUniformMatrix4f("view", viewMatrix);
		shader->SetUniformMatrix4f("projection", projectionMatrix);

		model->Draw(*shader);
	}
}
// - END Private Methods

} /* namespace CGL */
