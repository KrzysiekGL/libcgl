#include "Actor.h"

namespace CGL {

// - Ctors & Dtors

Actor::Actor(
			std::shared_ptr<Scene> rootScene,
			std::string shaderProgramName,
			std::string modelName,
			btRigidBody * body,
			bool isTransparent
			)
{
	this->rootScene = rootScene;
	this->shaderProgramName = shaderProgramName;
	this->modelName = modelName;
	this->body = body;
	this->isTransparent = isTransparent;
}

Actor::~Actor(){
}
// - END Ctros & Dtors

// - Public Methods
void Actor::SetLinearVelocity(glm::vec3 vec, float value) {
	vec *= value;
	btVector3 btDirection(vec.x, vec.y, vec.z);
	body->setLinearVelocity(btDirection);
} /* Actor::SetLinearVelocity(...) */

void Actor::SetActorName(std::string actorName) {
	this->actorName = actorName;
} /* Actor::SetActorName(...) */

void Actor::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	// Get Bullet motion state
	btTransform transform;
	if(body && body->getMotionState())
		body->getMotionState()->getWorldTransform(transform);
	else
		transform = body->getWorldTransform();
	glm::mat4 modelMatrix;
	transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));

	// Get handles for ShaderProgram and Model
	std::shared_ptr<ShaderProgram> shader; rootScene->GetShaderProgramPtr(shaderProgramName, shader);
	std::shared_ptr<Model> model; rootScene->GetModelPtr(modelName, model);

	// Check if shader and model are set
	if(shader == nullptr || model == nullptr) {
		std::cout << "CGL::ERRORR::ACTOR Shader ptr or Model ptr not set for rendering\n";
		return;
	}

	// Render Actor
	shader->SetUniformMatrix4f("model", modelMatrix);
	shader->SetUniformMatrix4f("view", viewMatrix);
	shader->SetUniformMatrix4f("projection", projectionMatrix);
	model->Draw(shader);
}

std::string Actor::GetModelName() const {
	return modelName;
}

std::string Actor::GetShaderProgramName() const {
	return shaderProgramName;
}

btRigidBody * const Actor::GetRigidBody() const {
	return body;
}

glm::mat4 Actor::GetModelMatrix() const {
	btTransform transform;
	if(body && body->getMotionState())
		body->getMotionState()->getWorldTransform(transform);
	else
		transform = body->getWorldTransform();
	glm::mat4 modelMatrix;
	transform.getOpenGLMatrix(glm::value_ptr(modelMatrix));
	return modelMatrix;
}

//void Actor::SetModelMatrix(glm::mat4 modelMatrix) {
//	this->modelMatrix = modelMatrix;
//}
// - END Public Methods

}
