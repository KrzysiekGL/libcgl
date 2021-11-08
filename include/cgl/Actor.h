#ifndef ACTOR_H_
#define ACTOR_H_

#include "Resource.h"
#include "ShaderProgram.h"
#include "Model.h"
#include "PrimitiveShape.h"

#include <glm/glm.hpp>

#include <btBulletDynamicsCommon.h>

#include <memory>

namespace CGL {

class Actor : public Resource {
public:
	Actor(
			std::string name,
			std::shared_ptr<ShaderProgram> shaderProgram,
			std::shared_ptr<Model> model,
			std::shared_ptr<PrimitiveShape> shape,
			bool isTransparent);

	/*
	 * Delete Copy Constructor and operator=
	 */
	Actor(const Actor & other) = delete;
	Actor & operator=(const Actor & other) = delete;

	/*
	 * Draw an actor
	 */
	void Draw(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);

	/*
	 *  Set linear velocity of this actor
	 */
	void SetLinearVelocity(glm::vec3 direction, float value);

	/*
	 * Getters
	 */
	std::shared_ptr<Model> GetModelPtr() const;
	glm::mat4 GetModelMatrix() const;

	/*
	 * Setters
	 */
	//void SetModelMatrix(glm::mat4 modelMatrix);

private:
	std::shared_ptr<ShaderProgram> shaderProgram;
	std::shared_ptr<Model> model;
	std::shared_ptr<PrimitiveShape> shape;
	bool isTransparent;
};

}

#endif //ACTOR_H_
