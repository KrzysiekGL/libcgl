#include "Scene.h"

namespace CGL {

/* Ctor & Dtor */
Scene::Scene() {
	// Default settings
	freeCam = false;
	scr_width = 0.f;
	scr_height = 0.f;

	// Initialize resource manager
	rman = std::make_shared<ResourceManager>();

	// Add default Camera
	std::string camera_name = "Camera-00";
	AddCamera(camera_name, glm::vec3(0.f, 7.f, 15.f), -45.f);
	current_camera = std::dynamic_pointer_cast<Camera>(rman->GetResourceByName(camera_name));
	current_camera->SetCameraSpeed(20.f);

	// Create Bullet Dynamic World and it's configuration dependencies
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphaseInterface = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0.f, -9.81f, 0.f));
}

Scene::~Scene(){
	// Delete all Bullet members
	delete dynamicWorld;
	delete solver;
	delete broadphaseInterface;
	delete dispatcher;
	delete collisionConfiguration;
}
/* Ctor & Dtor */
/* Public Methods */
std::string Scene::AddCamera(std::string camera_name, glm::vec3 camera_position, float pith, float yaw, float camera_sensitivity, float camera_speed){
	if(! rman->AddResource(std::make_shared<Camera>(camera_name, camera_position, pith, yaw, camera_sensitivity, camera_speed))) {
		std::cout << "CGL::WARNING::SCENE::ADDCAMERA() Camera with name " << camera_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	return camera_name;
}

std::string Scene::AddShaderProgram(std::string shader_name, std::string vertex_path, std::string fragment_path){
	if(! rman->AddResource(std::make_shared<ShaderProgram>(shader_name, vertex_path.c_str(), fragment_path.c_str()))) {
		std::cout << "CGL::WARNING::SCENE::ADDCSHADERPROGRAM() ShaderProgram with name " << shader_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	return shader_name;
}

std::string Scene::AddModel(std::string model_name, std::string model_path){
	if(! rman->AddResource(std::make_shared<Model>(model_name, model_path.c_str()))) {
		std::cout << "CGL::WARNING::SCENE::ADDMODEL() Model with name " << model_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	return model_name;
}

std::string Scene::AddPrimitivePlane(std::string body_name, glm::mat4 modelMatrix, btVector3 planeNormal, btScalar planeConstatnt) {
	if(! rman->AddResource(std::make_shared<PrimitiveShape>(body_name, Shape::PLANE))) {
		std::cout << "CGL::WARNING::SCENE::ADDPRIMITIVEPLANE() Primitive with name " << body_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	// Setup PrimitiveShape Plane
	std::shared_ptr<PrimitiveShape> shape = getPrimitiveShape(body_name); if(shape == NULL) return std::string();
	shape->SetupPlane(dynamicWorld, modelMatrix, planeNormal, planeConstatnt);
	return body_name;
}

std::string Scene::AddPrimitiveBox(std::string body_name, glm::mat4 modelMatrix, btScalar mass, btVector3 boxDimensions) {
	if(! rman->AddResource(std::make_shared<PrimitiveShape>(body_name, Shape::BOX))) {
		std::cout << "CGL::WARNING::SCENE::ADDPRIMITIVEBOX() Primitive with name " << body_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	// Setup PrimitiveShape Box
	std::shared_ptr<PrimitiveShape> shape = getPrimitiveShape(body_name); if(shape == NULL) return std::string();
	shape->SetupBox(dynamicWorld, modelMatrix, mass, boxDimensions);
	return body_name;
}

std::string Scene::AddPrimitiveSphere(std::string body_name, glm::mat4 modelMatrix, btScalar mass, btScalar sphereRadius) {
	if(! rman->AddResource(std::make_shared<PrimitiveShape>(body_name, Shape::SPHERE))) {
		std::cout << "CGL::WARNING::SCENE::ADDPRIMITVESPHERE() Primitive with name " << body_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	// Setup PrimitiveShape Sphere
	std::shared_ptr<PrimitiveShape> shape = getPrimitiveShape(body_name); if(shape == NULL) return std::string();
	shape->SetupSpeher(dynamicWorld, modelMatrix, mass, sphereRadius);
	return body_name;
}

std::string Scene::AddActor(std::string actor_name, std::string model_name, std::string shaderProgram_name, std::string primitiveShape_name, bool isTransparent) {
	/*
	 * Search if there are model_name, shaderProgram_name
	 * and primitiveShape_name present in the ResourceManager
	 */
	// Model search
	std::shared_ptr<Model> model = getModel(model_name); if(model == NULL) return std::string();

	// ShaderProgram search
	std::shared_ptr<ShaderProgram> shader = getShaderProgram(shaderProgram_name); if(shader == NULL) return std::string();

	// PrimitiveShape search
	std::shared_ptr<PrimitiveShape> shape = getPrimitiveShape(primitiveShape_name); if(shape == NULL) return std::string();

	// Add Actor to the ResourceManager
	if(! rman->AddResource(std::make_shared<Actor>(actor_name, shader, model, shape, isTransparent))) {
		std::cout << "CGL::WARNING::SCENE::ADDACTOR() Actor with name " << actor_name << " is already present in the ResourceManager\n";
		return std::string();
	}
	return actor_name;
} /* Scene::AddActor(...) */

void Scene::DelActor(std::string actorName) {
	std::vector<std::string> names; names.push_back(actorName);
	rman->DeleteResourcesByNames(names);
} /* Scene::DelActor(actorName) */

void Scene::RunScene(GLFWwindow* window, float deltaTime, bool freeze, bool freeCam) {
	// freeCam for the Camera
	this->freeCam = freeCam;
	// check for size of a frame buffer
	updateSceneParameters(window);
	// handle inputs by the Camera
	handleKeyboardInput(window, deltaTime);
	handleMouseInput(window);
	// Run physics if not freeze
	if(!freeze) dynamicWorld->stepSimulation(1.f/60.f, 10.f);
	// render everything
	draw();
}

void Scene::SetActorLinearVelocity(std::string actor_name, glm::vec3 direction, float value) {
	auto actor = getActor(actor_name);
	if(actor != NULL) actor->SetLinearVelocity(direction, value);
}

std::vector<std::string> Scene::GetCollectionNames(Type type) const {
	std::vector<std::shared_ptr<Resource>> resources = rman->GetAllResourcesByType(type);
	std::vector<std::string> names;
	for(auto & r : resources)
		names.push_back(r->GetName());
	return names;
}

glm::vec3 Scene::GetCameraPosition() const {
	return current_camera->GetPosition();
}

glm::vec3 Scene::GetCameraFront() const {
	return current_camera->GetFront();
}

/* Public Methods */
/* Private Methods */
std::shared_ptr<ShaderProgram> Scene::getShaderProgram(std::string shaderProgram_name) {
	std::shared_ptr<ShaderProgram> shader = std::dynamic_pointer_cast<ShaderProgram>(rman->GetResourceByName(shaderProgram_name));
	if(shader == nullptr){
		std::cout << "CGL::ERROR::SCENE::GETSHADER() No " << shaderProgram_name << " ShaderProgram found in the ResourceManager\n";
		return NULL;
	}
	return shader;
}
std::shared_ptr<PrimitiveShape> Scene::getPrimitiveShape(std::string primitiveShape_name) {
	std::shared_ptr<PrimitiveShape> shape = std::dynamic_pointer_cast<PrimitiveShape>(rman->GetResourceByName(primitiveShape_name));
	if(shape == nullptr){
		std::cout << "CGL::ERROR::SCENE::ADDACTOR() No " << primitiveShape_name << " PrimitiveShape found in the ResourceManager\n";
		return NULL;
	}
	return shape;
}
std::shared_ptr<Model> Scene::getModel(std::string model_name) {
	std::shared_ptr<Model> model = std::dynamic_pointer_cast<Model>(rman->GetResourceByName(model_name));
	if(model == nullptr){
		std::cout << "CGL::ERROR::SCENE::GETMODEL() No " << model_name << " Model found in the ResourceManager\n";
		return NULL;
	}
	return model;
}
std::shared_ptr<Camera> Scene::getCamera(std::string camera_name) {
	std::shared_ptr<Camera> camera = std::dynamic_pointer_cast<Camera>(rman->GetResourceByName(camera_name));
	if(camera == nullptr){
		std::cout << "CGL::ERROR::SCENE::GETCAMERA() No " << camera_name << " Model found in the ResourceManager\n";
		return NULL;
	}
	return camera;
}
std::shared_ptr<Actor> Scene::getActor(std::string actor_name) {
	std::shared_ptr<Actor> actor = std::dynamic_pointer_cast<Actor>(rman->GetResourceByName(actor_name));
	if(actor == nullptr){
		std::cout << "CGL::ERROR::SCENE::GETACTOR() No " << actor_name << " Actor found in the ResourceManager\n";
		return NULL;
	}
	return actor;
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

	// Iterator over all Actor type Resources and render them
	std::vector<std::shared_ptr<Resource>> resources = rman->GetAllResourcesByType(Type::ACTOR);
	for(auto & r : resources)
		std::dynamic_pointer_cast<Actor>(r)->Draw(viewMatrix, projectionMatrix);
}
/* Private Methods */
} /* namespace CGL */
