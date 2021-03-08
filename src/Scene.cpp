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

	// Create Bullet Dynamic World and it's configuration dependencies
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	broadphaseInterface = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver();
	dynamicWorld = new btDiscreteDynamicsWorld(dispatcher, broadphaseInterface, solver, collisionConfiguration);
	dynamicWorld->setGravity(btVector3(0.f, -9.81f, 0.f));
}

Scene::~Scene(){
	delete dynamicWorld;
	delete solver;
	delete broadphaseInterface;
	delete dispatcher;
	delete collisionConfiguration;
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

std::string Scene::AddActor(std::string model_name, std::string shaderProgram_name, Shape shape, btScalar mass, glm::mat4 model_matrix, bool isTransparent) {
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
		return std::string("");
	}

	// ShaderProgram search
	std::map<std::string, std::shared_ptr<ShaderProgram>>::iterator spit = shaderProgramCollection.find(shaderProgram_name);
	if(spit == shaderProgramCollection.end()){
#ifdef _DEBUG
		std::cout << "ERROR::SCENE No \"" << shaderProgram_name << "\" ShaderProgram found in the Scene\n";
#endif
		return std::string("");
	}

	/*
	 * If both are present, then create a new shared_ptr to Actor,
	 * generate a name and put it into collection
	 */
	std::string actor_name = mit->first;
	std::shared_ptr<Actor> actor = std::make_shared<Actor>();

	// create a rigid body and add it to the Dynamic World; also store collision shape in the actor

	// Bullet shape
	// TODO Make it possible to customize shapes
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

	// get pointers to model and shader collection to store them in the actor

	// store everything what is needed in the actor
	std::shared_ptr<Model> sharedModel = mit->second;
	std::shared_ptr<ShaderProgram> sharedShaderProgram = spit->second;
	actor->SetParameters(sharedModel, sharedShaderProgram, bulletShape, body, model_matrix, isTransparent);

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
		if(a.second->GetSharedModel() == actor->GetSharedModel())
			number++;
	actor_name = actor_name+"-"+std::to_string(number);

	// add Actor to the collection
	actorCollection[actor_name] = actor;

	return actor_name;
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

	// Step Bullet in simulation
	dynamicWorld->stepSimulation(1.f/60.f, 10.f);

	// Iterate over all world collision objects
	for (int i = dynamicWorld->getNumCollisionObjects()-1; i>=0; i--){
		btCollisionObject * obj = dynamicWorld->getCollisionObjectArray()[i];
		btRigidBody * body = btRigidBody::upcast(obj);

		// find to which Actor this body/object corresponds to
		// Iterate over all actors from collection and render them
			for(std::pair<std::string, std::shared_ptr<Actor>> element : actorCollection){
				const std::shared_ptr<Actor> actor = element.second;

				// if this is not the Actor - skip
				if(actor->GetRigidBody() != body)
					continue;

				// Get transformation matrix (model matrix)
				btTransform transform;
				if(body && body->getMotionState())
					body->getMotionState()->getWorldTransform(transform);
				else
					transform = obj->getWorldTransform();

				glm::mat4 new_modelMatrix;
				transform.getOpenGLMatrix(glm::value_ptr(new_modelMatrix));
				actor->SetModelMatrix(new_modelMatrix);

				// Prepare handles for Model and ShaderProgram
				std::shared_ptr<Model> sharedPtrModel = actor->GetSharedModel();
				std::shared_ptr<ShaderProgram> sharedPtrShaderProgram = actor->GetSharedShaderProgram();

				// Get Model and ShaderProgram; Set parameters and render model
				sharedPtrShaderProgram->SetUniformMatrix4f("model", actor->GetModelMatrix());
				sharedPtrShaderProgram->SetUniformMatrix4f("view", viewMatrix);
				sharedPtrShaderProgram->SetUniformMatrix4f("projection", projectionMatrix);
				sharedPtrModel->Draw(sharedPtrShaderProgram);
			}
	}
}
// - END Private Methods

} /* namespace CGL */
