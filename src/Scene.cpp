#include "Scene.h"

namespace CGL {

// - Constructors & Destructors
Scene::Scene() {
	// Default settings
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;

	// Add default Camera
	std::string camera_name = "Camera-00";
	AddCamera(camera_name);
	current_camera = cameraCollection[camera_name];
}
// -  END Constructors & Destructors

// - Public Methods
void Scene::AddCamera(std::string camera_name, glm::vec3 camera_position, float pith, float yaw, float camera_sensitivity, float camera_speed){
	cameraCollection[camera_name] = std::make_shared<Camera>(camera_position, pith, yaw, camera_sensitivity, camera_speed);
}

void Scene::AddShaderProgram(std::string shader_name, std::string vertex_path, std::string fragment_path){
	const char* vertex = vertex_path.c_str();
	const char* fragment = fragment_path.c_str();
	shaderProgramCollection[shader_name] = std::make_shared<ShaderProgram>(vertex, fragment);
}

void Scene::AddModel(std::string model_name, std::string model_path){
	const char* model = model_path.c_str();
	modelCollection[model_name] = std::make_shared<Model>(model);
}

bool Scene::AddActor(std::string model_name, std::string shaderProgram_name, glm::mat4 model_matrix, bool isTransparent) {
	/*
	 * Search if there are model_name and shaderProgram_name
	 * present in the collections
	 */

	// Model search
	std::map<std::string, std::shared_ptr<Model>>::iterator mit = modelCollection.find(model_name);
	if(mit == modelCollection.end()){
#ifdef _DEBUG
		std::cout << "ERROR::SCENE No \"" << model_name << "\" Model found in the scene\n";
#endif //_DEBUG
		return false;
	}

	// ShaderProgram search
	std::map<std::string, std::shared_ptr<ShaderProgram>>::iterator spit = shaderProgramCollection.find(shaderProgram_name);
	if(spit == shaderProgramCollection.end()){
#ifdef _DEBUG
		std::cout << "ERROR::SCENE No \"" << shaderProgram_name << "\" ShaderProgram found in the Scene\n";
#endif
		return false;
	}

	/*
	 * If both are present, then create a new shared_ptr to Actor,
	 * generate a name and put it into collection
	 */
	std::string actor_name = mit->first;
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();

	// parameters
	actor->sharedPtrModel = mit->second;
	actor->sharedPtrShaderProgram = spit->second;
	actor->modelMatrix = model_matrix;
	actor->transparent = isTransparent;

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
	for(std::pair<std::string, std::shared_ptr<Actor>> a : actorCollection)
		if(a.second->sharedPtrModel == actor->sharedPtrModel)
			number++;
	actor_name = actor_name+"-"+std::to_string(number);

	// add Actor to the collection
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
	std::map<std::string, std::shared_ptr<ShaderProgram>>::iterator it = shaderProgramCollection.begin();
	while(it != shaderProgramCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetModelCollectionNames() {
	std::vector<std::string> names;
	std::map<std::string, std::shared_ptr<Model>>::iterator it = modelCollection.begin();
	while(it != modelCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetActorCollectionNames() {
	std::vector<std::string> names;
	std::map<std::string, std::shared_ptr<Actor>>::iterator it = actorCollection.begin();
	while(it != actorCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetCameraCollectionNames(){
	std::vector<std::string> names;
	std::map<std::string, std::shared_ptr<Camera>>::iterator it = cameraCollection.begin();
	while(it != cameraCollection.end()){
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
	if(freeCam) current_camera->KeyInputProcess(window, deltaFrame);
	else {
		// TODO control over an actor
	}
}

void Scene::handleMouseInput(GLFWwindow* window) {
	// for moving a camera
	if(freeCam) current_camera->MouseInputProcess(window);
}

void Scene::draw() {
	// Get view and projection matrices for current frame from the Camera
	glm::mat4 viewMatrix = current_camera->GetViewMatrix();
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.f), scr_width/scr_height, .1f, 100.f);

	// Iterate over all actors from collection and render them
	for(std::pair<std::string, std::shared_ptr<Actor>> element : actorCollection){
		// Prepare handles for Model and ShaderProgram
		const std::shared_ptr<Actor> actor = element.second;
		std::shared_ptr<Model> sharedPtrModel = actor->sharedPtrModel;
		std::shared_ptr<ShaderProgram> sharedPtrShaderProgram = actor->sharedPtrShaderProgram;

		// Get Model and ShaderProgram; Set parameters and render model
		sharedPtrShaderProgram->SetUniformMatrix4f("model", actor->modelMatrix);
		sharedPtrShaderProgram->SetUniformMatrix4f("view", viewMatrix);
		sharedPtrShaderProgram->SetUniformMatrix4f("projection", projectionMatrix);
		sharedPtrModel->Draw(sharedPtrShaderProgram);
	}
}
// - END Private Methods

} /* namespace CGL */