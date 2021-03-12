#include "Actor.h"
#include "Scene.h"

namespace CGL {

/* Ctor & Dtor */
Actor::Actor(
		std::string name,
		Scene * rootScene,
		ShaderProgram * shaderProgram,
		Model * model,
		btRigidBody * body,
		bool isTransparent)
{
	// Resource configuration
	setName(name); setType(Type::ACTOR);

	// Actor configuration
	this->rootScene = rootScene;
	this->shaderProgram = shaderProgram;
	this->model = model;
	this->body = body;
	this->isTransparent = isTransparent;
}

Actor::~Actor(){ }
/* Ctor & Dtor */
/* Public Methods */
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
	shaderProgram->SetUniformMatrix4f("model", modelMatrix);
	shaderProgram->SetUniformMatrix4f("view", viewMatrix);
	shaderProgram->SetUniformMatrix4f("projection", projectionMatrix);
	model->Draw(shaderProgram);
}

Resource * Actor::GetModelPtr() const {
	return model;
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
/* Public Methods */
} /* namespace CGL */
