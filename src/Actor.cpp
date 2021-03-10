#include "Actor.h"

namespace CGL {

// - Ctors & Dtors
Actor::Actor() {}

Actor::~Actor(){
	delete shape;
}
// - END Ctros & Dtors

// - Public Methods
void Actor::SetLinearVelocity(glm::vec3 vec, float value) {
	vec *= value;
	btVector3 btDirection(vec.x, vec.y, vec.z);
	body->setLinearVelocity(btDirection);
} /* Actor::SetLinearVelocity(...) */

void Actor::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	// Get Bullet motion state
	btTransform transform;
	if(body && body->getMotionState())
		body->getMotionState()->getWorldTransform(transform);
	else
		transform = body->getWorldTransform();

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
	return modelMatrix;
}

void Actor::SetModelMatrix(glm::mat4 modelMatrix) {
	this->modelMatrix = modelMatrix;
}

void Actor::SetParameters(
			std::shared_ptr<Model> sharedModel,
			std::shared_ptr<ShaderProgram> sharedShaderProgram,
			btCollisionShape * shape,
			btRigidBody * body,
			glm::mat4 modelMatrix,
			bool isTransparent)
{
	this->sharedModel = sharedModel;
	this->sharedShaderProgram = sharedShaderProgram;
	this->shape = shape;
	this->body = body;
	this->modelMatrix = modelMatrix;
	this->isTransparent = isTransparent;
}
// - END Public Methods

}
