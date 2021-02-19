#include "Actor.h"

namespace CGL {

// - Ctors & Dtors
Actor::Actor() {}

Actor::~Actor(){
	delete shape;
}
// - END Ctros & Dtors

// - Public Methods
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