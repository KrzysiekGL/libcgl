/*
 * Scene class was designed to arrange and manipulate actors (Model objects)
 * - add an actor as a base Model
 * - modify the actor by adding another copies/iterations of it with personalized parameters
 * - Scene object has it's own Camera object, thus It also have to pass to it's Camera any control input
 * - Scene object also holds all of ShaderPrograms that are ought to be used with it's actors
 *
 * To create a simple Scene and run it, do the following:
 * 1. Create a Scene with a Camera as a parameter
 * 2. Add a ShaderProgram
 * 3. Add an actor with a Model as a parameter and a reference
 *    to a ShaderProgram from the Scene object (may be that one from the previous step)
 * 4. Run cycle of the Scene with RunScene() (normally inside your "Game Loop")
 */

#ifndef SCENE_H_
#define SCENE_H_

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>


namespace CGL {
// TODO add properties for copies of an actor
struct Actor {
	Model model;
	glm::mat4 modelMatrix;
	ShaderProgram& shaderProgram;
};

class Scene {
public:
	Scene();
	Scene(Camera camera);
	virtual ~Scene();

	void AddShaderProgram(ShaderProgram shaderProgram);
	ShaderProgram& GetShaderProgram(unsigned int index);
	void AddActor(Model model, ShaderProgram* shaderProgram, glm::mat4 modelMatrix = glm::mat4(1.f));

	void RunScene(GLFWwindow* window, float deltaFrame, bool freeCam);

private:
	std::vector<Actor> actors;
	std::vector<ShaderProgram> shaderPrograms;
	Camera camera;
	bool freeCam;
	float scr_width; float scr_height;

	void draw();
	void updateSceneParameters(GLFWwindow* window);
	void handleKeyboardInput(GLFWwindow* window, float deltaTime);
	void handleMouseInput(GLFWwindow* window);
};

} /* namespace CGL */

#endif /* SCENE_H_ */
