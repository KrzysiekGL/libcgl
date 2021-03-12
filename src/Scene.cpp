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
	AddCamera(camera_name, glm::vec3(0.f, 7.f, 15.f), -45.f);
	current_camera = cameraCollection[camera_name];
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

	// Delete rest of the private members
	for(auto & actor : actorCollection)
		delete(actor.second);

	for(auto & shader : shaderProgramCollection)
		delete(shader.second);

	for(auto & model : modelCollection)
		delete(model.second);

	for(auto & cam : cameraCollection)
		delete(cam.second);
}
// -  END Constructors & Destructors

// - Public Methods
void Scene::AddCamera(std::string camera_name, glm::vec3 camera_position, float pith, float yaw, float camera_sensitivity, float camera_speed){
	cameraCollection[camera_name] = new Camera(camera_position, pith, yaw, camera_sensitivity, camera_speed);
}

void Scene::AddShaderProgram(std::string shader_name, std::string vertex_path, std::string fragment_path){
	// Check first if given ShaderProgram doesn't exist yet in the collection

	// search for the same name
	auto it = shaderProgramCollection.find(shader_name);
	if(it != shaderProgramCollection.end()) return;

	ShaderProgram * shader = new ShaderProgram(vertex_path.c_str(), fragment_path.c_str());

	// search for the same ShaderProgram by shader's source paths
	it=shaderProgramCollection.begin();
	while(it!=shaderProgramCollection.end()) {
		// If there is already ShaderProgram with existing Vertex and Fragment shaders, save it under the new name
		if(		(shader->GetVertexPath() == it->second->GetVertexPath()) &&
				(shader->GetFragmentPaht() == it->second->GetFragmentPaht())) {
			shaderProgramCollection[shader_name] = it->second;
			return;
		}
		it++;
	}

	// add to the collection
	shaderProgramCollection[shader_name] = shader;
}

void Scene::AddModel(std::string model_name, std::string model_path){
	// Check first if given Model doesn't exist yet in the collection

	// Search for the same name
	auto it = modelCollection.find(model_name);
	if(it != modelCollection.end()) return;

	Model * model = new Model(model_path.c_str());

	// Search for the same Model by directory
	it=modelCollection.begin();
	while(it!=modelCollection.end()){
		// If Model already exist, make this Model accessible under the new name
		if(model->GetDirectory() == it->second->GetDirectory()) {
			modelCollection[model_name] = it->second;
			return;
		}
		it++;
	}

	// add to the collection
	modelCollection[model_name] = model;
}

std::string Scene::AddActor(
		std::string model_name,
		std::string shaderProgram_name,
		Shape shape,
		btScalar mass,
		glm::mat4 model_matrix,
		bool isTransparent)
{
	/*
	 * Search if there are model_name and shaderProgram_name
	 * present in the collections
	 */
	// Model search
	auto mit = modelCollection.find(model_name);
	if(mit == modelCollection.end()){
#ifdef _DEBUG
		std::cout << "CGL::ERROR::SCENE No \"" << model_name << "\" Model found in the scene\n";
#endif //_DEBUG
		return std::string("");
	}

	// ShaderProgram search
	auto spit = shaderProgramCollection.find(shaderProgram_name);
	if(spit == shaderProgramCollection.end()){
#ifdef _DEBUG
		std::cout << "CGL::ERROR::SCENE No \"" << shaderProgram_name << "\" ShaderProgram found in the Scene\n";
#endif
		return std::string("");
	}

	/*
	 * If both are present, then create a new ptr to Actor,
	 * generate a name and put it into collection
	 */
	// create a rigid body and add it to the Dynamic World; also store collision shape in the actor
	// Bullet shape
	// TODO Make it possible to choose shape easily
	btCollisionShape * bulletShape;
	switch (shape) {
		case Shape::BOX:
			bulletShape = new btBoxShape(btVector3(1.f, 1.f, 1.f));
			break;
		case Shape::PLANE:
			bulletShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 0.f);
			break;
		case Shape::SPHERE:
			bulletShape = new btSphereShape(1.f);
			break;
	}

	// Bullet initial model transformation
	btTransform transform;
	transform.setFromOpenGLMatrix(glm::value_ptr(model_matrix));

	// Bullet rigid body dynamics
	btVector3 localInertia(0.f, 0.f, 0.f);
	if(mass!=0.f)
		bulletShape->calculateLocalInertia(mass, localInertia);

	// Bullet motionstate
	btDefaultMotionState * motionState = new btDefaultMotionState(transform);

	// Bullet rigid body
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, bulletShape, localInertia);
	btRigidBody * body = new btRigidBody(rbInfo);

	// Add teh body to teh dynamic world
	dynamicWorld->addRigidBody(body);

	/*
	 * store everything what is needed in the actor
	 * especially set it's ShaderProgram, Model and the rootScene
	 */
	Actor * actor = new Actor(this, spit->second, mit->second, body, isTransparent);

	/*
	 * Name:
 	 * Iterate over actor collection, and search for how
 	 * many actors has the same model. Then modify actor_name
 	 * so it has another number with regard to how many actors
 	 * with the same model there are.
 	 * For example: if there are already 3 actors with model "box"
 	 * then change actor_name from "box" to "box-3" (counting from 0)
	 */
	std::string actor_name = mit->first;
	unsigned int number = 0;
	for(auto actorElement : actorCollection)
		if(actorElement.second->GetModelPtr() == actor->GetModelPtr())
			number++;
	actor_name = actor_name+"-"+std::to_string(number);
	actor->SetActorName(actor_name);

	// add Actor to the collection
	actorCollection[actor_name] = actor;

	return actor_name;
} /* Scene::AddActio(...) */

void Scene::DelActor(std::string actorName) {
	auto it = actorCollection.find(actorName);
	if(it != actorCollection.end()) {
		delete(it->second);
		actorCollection.erase(it);
	}
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
	auto it = actorCollection.find(actor_name);
	if(it != actorCollection.end())
		it->second->SetLinearVelocity(direction, value);
}

// -- Getters
std::vector<std::string> Scene::GetShaderProgramCollectionNames() const {
	std::vector<std::string> names;
	auto it = shaderProgramCollection.begin();
	while(it != shaderProgramCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetModelCollectionNames() const {
	std::vector<std::string> names;
	auto it = modelCollection.begin();
	while(it != modelCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetActorCollectionNames() const {
	std::vector<std::string> names;
	auto it = actorCollection.begin();
	while(it != actorCollection.end()) {
		names.push_back(it->first);
		it++;
	}
	return names;
}

std::vector<std::string> Scene::GetCameraCollectionNames() const {
	std::vector<std::string> names;
	auto it = cameraCollection.begin();
	while(it != cameraCollection.end()){
		names.push_back(it->first);
		it++;
	}
	return names;
}

ShaderProgram * Scene::GetShaderProgramPtr(std::string shaderProgramName) const {
	auto it = shaderProgramCollection.find(shaderProgramName);
	if(it == shaderProgramCollection.end()) return nullptr;
	return it->second;
}

Model * Scene::GetModelPtr(std::string modelName) const {
	auto it = modelCollection.find(modelName);
	if(it == modelCollection.end()) return nullptr;
	return it->second;
}

glm::vec3 Scene::GetCameraPosition(std::string camera_name) const {
	return cameraCollection.find(camera_name)->second->GetPosition();
}

glm::vec3 Scene::GetCameraFront(std::string camera_name) const {
	return cameraCollection.find(camera_name)->second->GetFront();
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

	// Iterator over all objects and render them  (if freeze then with physics)
	for(auto & actor : actorCollection)
		actor.second->Draw(viewMatrix, projectionMatrix);
}
// - END Private Methods

} /* namespace CGL */
