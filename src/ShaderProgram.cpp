#include "ShaderProgram.h"
namespace CGL {

// - Ctors & Dtors
	ShaderProgram::ShaderProgram() {
		ID = 0;
	#ifdef _DEBUG
		std::cout << "CGL::INFO::SHADERPROGRAM ShaderProgram instantiated, but shader program itself not compiled and created.\n";
	#endif // _DEBUG
	}

	ShaderProgram::ShaderProgram(const char* vertexFile, const char* fragmentFile) {
		vertex_path = std::string(vertexFile);
		fragment_path = std::string(fragmentFile);
		this->ID = glCreateProgram();

		addShaderToProgram(vertexFile, ShaderType::VERTEX);
		addShaderToProgram(fragmentFile, ShaderType::FRAGMENT);

		glLinkProgram(this->ID);
#ifdef _DEBUG
		int success;
		glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "CGL::ERROR::GL::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
			std::cout << "Program created, but could not be linked\n";
		}
#endif // _DEBUG
	}

	ShaderProgram::~ShaderProgram() {
		glDeleteProgram(this->ID);
	}
// - END Ctors & Dtors

// - Public Methods
	void ShaderProgram::SetUniform1i(std::string name, int value) {
		int	location = glGetUniformLocation(ID, name.c_str());
#ifdef _DEBUG
		if(-1==location)
			std::cout << "CGL::ERROR::SHADERPROGRAM::Returned value of location is " << location <<
				" Name \"" << name << "\" doesn't correspond to an active unifrom variable in program or" <<
				" name starts with the reserved prefix \"gl_\".\n";
#endif //_DEBUG
		if(-1!=location) glUniform1i(location, value);
	}

	void ShaderProgram::SetUniformMatrix4f(std::string name, glm::mat4 mat) { // @suppress("Type cannot be resolved") // @suppress("Member declaration not found")
		int location = glGetUniformLocation(this->ID, name.c_str());
#ifdef _DEBUG
		if(-1==location)
			std::cout << "CGL::ERROR::SHADERPROGRAM::Returned value of location is " << location <<
				" Name \"" << name << "\" doesn't correspond to an active unifrom variable in program or" <<
				" name starts with the reserved prefix \"gl_\".\n";
#endif //_DEBUG
		if(-1!=location) glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)); // @suppress("Invalid arguments")
	}

	std::string ShaderProgram::GetVertexPath() const {
		return vertex_path;
	}

	std::string ShaderProgram::GetFragmentPaht() const {
		return fragment_path;
	}
// - END Public Methods

// Private Methods
	std::string ShaderProgram::readFileToSource(const char* filePath) {
		std::ifstream file(filePath);
		if (file) {
			std::stringstream text;
			text << file.rdbuf();
			file.close();
			return text.str();
		}
		else {
	#ifdef _DEBUG
			std::cout << "ERRORR::IFSTREAM::Could not open file " << filePath << std::endl;
	#endif // _DEBUG
			file.close();
			return 0;
		}
	}

	void ShaderProgram::addShaderToProgram(const char* filePath, ShaderType type) {
		GLuint shader;
		std::string string_source = readFileToSource(filePath);
		const char* source = string_source.c_str();

		if (type == ShaderType::VERTEX)
			shader = glCreateShader(GL_VERTEX_SHADER);
		else // ShaderType::FRAGMENT
			shader = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(shader, 1, &source, NULL);
		glCompileShader(shader);

		int success;
			char infoLog[512];
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success) {
				glGetShaderInfoLog(shader, 512, NULL, infoLog);
	#ifdef _DEBUG
				std::cout << "ERROR::GL::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
	#endif // _DEBUG
				glDeleteShader(shader);
			}
			else {
				glAttachShader(this->ID, shader);
				glDeleteShader(shader);
			}
	}
// - END Private Methods

} // namespace CGL
