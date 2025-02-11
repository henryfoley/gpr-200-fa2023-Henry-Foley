#include "shader.h"


namespace HenryLib {
	std::string HenryLib::loadShaderSourceFromFile(const std::string& filePath)
	{
		std::ifstream fstream(filePath);
		if (!fstream.is_open())
		{
			printf("Failed to load file %s", filePath);
			return{};
		}
		std::stringstream buffer;
		buffer << fstream.rdbuf();
		return buffer.str();
	}

	unsigned int HenryLib::createShader(GLenum shaderType, const char* sourceCode)
	{
		//Create a new vertex shader object
		unsigned int shader = glCreateShader(shaderType);
		//Supply the shader object with source code
		glShaderSource(shader, 1, &sourceCode, NULL);
		//Compile the shader object
		glCompileShader(shader);
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success) {
			//512 is an arbitrary length, but should be plenty of characters for our error message.
			char infoLog[512];
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			printf("Failed to compile shader: %s", infoLog);
		}
		return shader;
	}

	unsigned int HenryLib::createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource)
	{
		unsigned int vertexShader = createShader(GL_VERTEX_SHADER, vertexShaderSource);
		unsigned int fragmentShader = createShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

		unsigned int shaderProgram = glCreateProgram();
		//Attach each stage
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		//Link all the stages together
		glLinkProgram(shaderProgram);
		int success;
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			char infoLog[512];
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			printf("Failed to link shader program: %s", infoLog);
		}
		//The linked program now contains our compiled code, so we can delete these intermediate objects
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return shaderProgram;
	}

	unsigned int HenryLib::createVAO(Vertex* vertexData, int numVertices, unsigned int* indicesData, int numIndices)
	{
		unsigned int vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		//Define a new buffer id
		unsigned int vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		//Allocate space for + send vertex data to GPU.
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * numVertices * 3, vertexData, GL_STATIC_DRAW);

		//Define a new buffer id
		unsigned int ebo;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, indicesData, GL_STATIC_DRAW);


		//Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, x));
		glEnableVertexAttribArray(0);

		//UV
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, u));
		glEnableVertexAttribArray(1);

		return vao;
	}

	Shader::Shader(const std::string& vertexShader, const std::string& fragmentShader)
	{
		std::string vertexShaderSource = loadShaderSourceFromFile(vertexShader.c_str());
		std::string fragmentShaderSource = loadShaderSourceFromFile(fragmentShader.c_str());
		mId = createShaderProgram(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
	}
	void Shader::use() 
	{
		glUseProgram(mId);
	}
	void Shader::setInt(const std::string& name, int v) const
	{
		glUniform1i(glGetUniformLocation(mId, name.c_str()), v);
	}
	void Shader::setFloat(const std::string& name, float v) const
	{
		glUniform1f(glGetUniformLocation(mId, name.c_str()), v);
	}
	void Shader::setVec2(const std::string& name, float x, float y) const
	{
		glUniform2i(glGetUniformLocation(mId, name.c_str()), x, y);
	}
	void Shader::setVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(mId, name.c_str()), x, y, z);
	}
	void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(mId, name.c_str()), x, y, z, w);
	}
	void Shader::setMat4(const std::string& name, const ew::Mat4& v) const
	{
		glUniformMatrix4fv(glGetUniformLocation(mId, name.c_str()), 1, GL_FALSE, &v[0][0]);
	}
}

