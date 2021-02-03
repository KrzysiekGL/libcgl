/*
 * Scene class was designed to arrange and manipulate actors (Model objects)
 * - add an actor as a base Model
 * - modify the actor by adding another copies/iterations of it with personalized parameters
 * - Scene object has it's own Camera object, thus It also have to pass to it's Camera any control input
 * - Scene object also holds all of ShaderPrograms that are ought to be used with it's actors
 *
 * To create a simple Scene and run it, do the following:
 * 1. Create a Scene with a Camera as a parameter
 * 2. Add a ShaderProgram with it's name
 * 3. Add an Actor with a Model as parameter and a name of the Actor
 * 4. Add an ActorCopy for an added Actor
 *    Provide model matrix and a name of the previously added ShaderProgram
 * 5. Run cycle of the Scene with RunScene() (normally inside your "Game Loop")
 */

// TODO add more comments inside this header

#ifndef SCENE_H_
#define SCENE_H_

#include "ShaderProgram.h"
#include "Camera.h"
#include "Model.h"

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <iterator>

namespace CGL {

struct ActorCopy  {
	glm::mat4 modelMatrix;
	std::map<std::string, ShaderProgram>::iterator shaderProgram;
};

struct Actor {
	Model model;
	std::vector<ActorCopy> instances;
};

class Scene {
public:
	Scene();
	Scene(Camera camera);
	virtual ~Scene();

	void AddShaderProgram(std::string shaderProgram_name, ShaderProgram shaderProgram);
	void AddShaderProgram(std::string shaderProgram_name, std::string vertex_source, std::string fragmen_source);

	void AddActor(std::string actor_name, Model model);
	void AddActor(std::string actor_name, std::string model_path);

	void AddActorCopy(std::string actor_name, std::string shaderProgram_name, glm::mat4 model_matrix = glm::mat4(1.f));

	void RunScene(GLFWwindow* window, float deltaFrame, bool freeCam);

	std::vector<std::string> GetShaderProgramNames();
	std::vector<std::string> GetActorNames();

private:
	std::map<std::string, Actor> actors;
	std::map<std::string, ShaderProgram> shaderPrograms;
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
