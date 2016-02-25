#include "Shader.h"



Shader::Shader(string fileName, int type)
{
	switch (type) {
	case 0:
		shaderId = glCreateShader(GL_VERTEX_SHADER);
		break;
	case 1:
		shaderId = glCreateShader(GL_FRAGMENT_SHADER);
	}

	getCode(fileName);
	compileShader();

}

void Shader::getCode(string fileName) {
	// Read the Vertex Shader code from the file
	std::ifstream shaderStream(fileName, std::ios::in);
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
}

void Shader::compileShader() {
	GLint Result = GL_FALSE;
	int InfoLogLength;

	char const * sourcePointer = shaderCode.c_str();
	glShaderSource(shaderId, 1, &sourcePointer, nullptr);
	glCompileShader(shaderId);

	// Check Vertex Shader
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> shaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(shaderId, InfoLogLength, nullptr, &shaderErrorMessage[0]);
		printf("%s\n", &shaderErrorMessage[0]);
	}
}


Shader::~Shader()
{
}

GLuint Shader::getShaderId() {
	return shaderId;
}

GLuint Shader::makeProgram(GLuint vShader, GLuint fShader) {
	GLint Result;
	GLint InfoLogLength;


	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, vShader);
	glAttachShader(ProgramID, fShader);

	glBindAttribLocation(ProgramID, 0, "in_Position");

	//appearing in the vertex shader.
	glBindAttribLocation(ProgramID, 1, "in_Color");

	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, nullptr, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	return ProgramID;
}
