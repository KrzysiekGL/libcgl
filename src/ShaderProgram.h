/*
 * ShderProgram is designed to handle shaders for OpenGL:
 * - compile shader from a source file (vertex shader and fragment shader as of now)
 * - create a OpenGL shader program from compiled shader
 * - link shaders together
 * - use shader program for rendering
 */

#ifndef SHADERHPP
#define SHADERHPP

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

namespace CGL {
	enum class ShaderType {
		VERTEX,
		FRAGMENT
	};

	class ShaderProgram {
	public:
		// Constructors & Destructors
		/* Empty ShaderProgram object only contain default gl shader program with ID set to 0 */
		ShaderProgram();
		/* Create a ShaderProgram from vertex and fragment shader source code */
		ShaderProgram(const char* vertexFile, const char* fragmentFile);
		/*
		 *  OpenGL is given the information that this shader
		 * with a given ID will no longer be used, thus is being deleted as a GL shader program
		 */
		~ShaderProgram();

		//Methods
		/* Get ShaderProgram ID given by OpenGL  */
		GLuint GetProgram() { return this->ID; }
		/* Use ShaderProgram described by ID given by OpenGL */
		void Use() { glUseProgram(this->ID); }

		/*
		 * Bunch of setters for setting uniforms
		 * inside shader programs (written in GLSL)
		 */
		void SetUniform1i(std::string name, int v);
		void SetUniformMatrix4f(std::string name, glm::mat4 mat);

	private:
		// Fields
		/* ShaderProgram ID given by OpenGL; defaults to 0 */
		GLuint ID;

		// Methods
		/*
		 * Basically whole creation of a shader program pipeline:
		 * Get source file data, create a GL shader program, compile shader,
		 * attache shader to the GL shader program (ID), and
		 * delete no longer required single GL shader object
		 */
		void addShaderToProgram(const char* filePath, ShaderType type);
		/* Read source file and store it as a std::string */
		std::string readFileToSource(const char* filePath);
	};
} // namespace CGL

#endif // !SHADERHPP
