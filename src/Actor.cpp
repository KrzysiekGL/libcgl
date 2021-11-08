#include "Actor.hpp"
#include "Scene.hpp"

namespace CGL {

/* Ctor & Dtor */
Actor::Actor(
		std::string name,
		std::shared_ptr<ShaderProgram> shaderProgram,
		std::shared_ptr<Model> model,
		std::shared_ptr<PrimitiveShape> shape,
		bool isTransparent)
{
	// Resource configuration
	setName(name); setType(Type::ACTOR);

	// Actor configuration
	this->shaderProgram = shaderProgram;
	this->model = model;
	this->shape = shape;
	this->isTransparent = isTransparent;
}
/* Ctor & Dtor */
/* Public Methods */
void Actor::SetLinearVelocity(glm::vec3 vec, float value) {
	btVector3 btDirection(vec.x, vec.y, vec.z);
	shape->SetLinearVelocity(btDirection, value);
} /* Actor::SetLinearVelocity(...) */

void Actor::Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix) {
	glm::mat4 modelMatrix = shape->GetModelMatrix();

	// Render Actor
	shaderProgram->Use();
	shaderProgram->SetUniformMatrix4f("model", modelMatrix);
	shaderProgram->SetUniformMatrix4f("view", viewMatrix);
	shaderProgram->SetUniformMatrix4f("projection", projectionMatrix);
	model->Draw(shaderProgram.get());
}

std::shared_ptr<Model> Actor::GetModelPtr() const {
	return model;
}

glm::mat4 Actor::GetModelMatrix() const {
	return shape->GetModelMatrix();
}

//void Actor::SetModelMatrix(glm::mat4 modelMatrix) {
//	this->modelMatrix = modelMatrix;
//}
/* Public Methods */
} /* namespace CGL */
