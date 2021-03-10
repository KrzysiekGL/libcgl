#include "Actor.h"

namespace CGL {

// - Ctors & Dtors
Actor::Actor() {}

Actor::Actor(
			std::shared_ptr<Model> sharedModel,
			std::shared_ptr<ShaderProgram> sharedShaderProgram,
			btRigidBody * body,
			bool isTransparent
			)
{
	this->sharedModel = sharedModel;
	this->sharedShaderProgram = sharedShaderProgram;
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

	// Render Actor
	sharedShaderProgram->SetUniformMatrix4f("model", modelMatrix);
	sharedShaderProgram->SetUniformMatrix4f("view", viewMatrix);
	sharedShaderProgram->SetUniformMatrix4f("projection", projectionMatrix);
	sharedModel->Draw(sharedShaderProgram);
}

std::shared_ptr<Model> Actor::GetSharedModel() const {
	return sharedModel;
}

std::shared_ptr<ShaderProgram> Actor::GetSharedShaderProgram() const {
	return sharedShaderProgram;
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
