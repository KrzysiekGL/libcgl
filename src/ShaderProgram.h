/*
 * ShderProgram is designed to handle shaders for OpenGL:
 * - compile shader from a source file (vertex shader and fragment shader as of now)
 * - create a OpenGL shader program from compiled shader
 * - link shaders together
 * - use shader program for rendering
 */

#ifndef SHADERHPP
#define SHADERHPP

#include "Resource.h"

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

	class ShaderProgram : public Resource {
	public:
		// Ctor & Dtor

		/*
		 * Create a ShaderProgram from vertex and fragment shader source code
		 */
		ShaderProgram(std::string name, const char* vertexFile, const char* fragmentFile);

		/*
		 * OpenGL is given the information that this shader
		 * with a given ID will no longer be used, thus is being deleted as a GL shader program
		 */
		~ShaderProgram();

		/*
		 * Delete Copy Constructor and operator=
		 * to prevent from memory leaks with pointers
		 */
		ShaderProgram(const ShaderProgram & other) = delete;
		ShaderProgram & operator=(const ShaderProgram & other) = delete;

		//Methods

		/*
		 * Get ShaderProgram ID given by OpenGL
		 */
		GLuint GetProgram() { return this->ID; }

		/*
		 * Use ShaderProgram described by ID given by OpenGL
		 */
		void Use() { glUseProgram(this->ID); }

		/*
		 * Bunch of setters for setting uniforms
		 * inside shader programs (written in GLSL)
		 */
		void SetUniform1i(const char * name, int v);
		void SetUniformMatrix4f(const char * name, glm::mat4 mat);

		/*
		 * Get vertex or fragment shader source file path
		 */
		std::string GetVertexPath() const;
		std::string GetFragmentPaht() const;

	private:
		// Fields

		/*
		 * ShaderProgram ID given by OpenGL; defaults to 0
		 */
		GLuint ID;

		std::string vertex_path;
		std::string fragment_path;

		// Methods

		/*
		 * Basically whole creation of a shader program pipeline:
		 * Get source file data, create a GL shader program, compile shader,
		 * attache shader to the GL shader program (ID), and
		 * delete no longer required single GL shader object
		 */
		void addShaderToProgram(const char* filePath, ShaderType type);

		/*
		 * Read source file and store it as a std::string
		 */
		std::string readFileToSource(const char* filePath);
	};
} // namespace CGL

#endif // !SHADERHPP
