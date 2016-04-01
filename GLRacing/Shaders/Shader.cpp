#include "Shader.h"



Shader::Shader(string vertexFileName, string fragmentFileName)
{
	GLuint vShaderId = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShaderId = glCreateShader(GL_FRAGMENT_SHADER);

	string vShaderCode = getCode(vertexFileName);
	string fShaderCode = getCode(fragmentFileName);

	//Compile Vertex Shader
	char const * sourcePointer = vShaderCode.c_str();
	glShaderSource(vShaderId, 1, &sourcePointer, nullptr);
	glCompileShader(vShaderId);

	checkStatus(vShaderId);

	//Compile Fragment Shader
	sourcePointer = fShaderCode.c_str();
	glShaderSource(fShaderId, 1, &sourcePointer, nullptr);
	glCompileShader(fShaderId);

	checkStatus(fShaderId);

	//Create Program
	makeProgram(vShaderId, fShaderId);

	//Delete Shaders
	glDeleteShader(vShaderId);
	glDeleteShader(fShaderId);

}

string Shader::getCode(string fileName) {
	// Read the Vertex Shader code from the file
	string shaderCode;
	std::ifstream shaderStream("Shaders/"+fileName, std::ios::in);
	if (shaderStream.is_open()) {
		std::string Line = "";
		while (getline(shaderStream, Line))
			shaderCode += "\n" + Line;
		shaderStream.close();
	}
	else {
		printf("Impossible to open %s. Are you in the right directory ?"
			" Don't forget to read the FAQ !\n", fileName.c_str());
		getchar();
		exit(-1);
	}
	return shaderCode;
}

void Shader::checkStatus(GLuint shaderID) {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Check Vertex Shader
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> shaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderID, InfoLogLength, nullptr, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
	}
}

GLuint Shader::getUniform(const GLchar* name) {
	return glGetUniformLocation(programID, name);

}

void Shader::use() {
	glUseProgram(programID);
}
void Shader::unuse() {
	glUseProgram(0);
}



Shader::~Shader()
{
}

void Shader::makeProgram(GLuint vShader, GLuint fShader) {
	GLint Result;
	GLint InfoLogLength;


	// Link the program
	printf("Linking program\n");
	programID = glCreateProgram();
	glAttachShader(programID, vShader);
	glAttachShader(programID, fShader);

	glBindAttribLocation(programID, 0, "in_Position");
	glBindAttribLocation(programID, 1, "normal");
	glBindAttribLocation(programID, 2, "texCoord");

	glBindAttribLocation(programID, 4, "fragmentdepth");


	glLinkProgram(programID);

	checkStatus(programID);
}
