#include "GameController.h"
#include <stdio.h>
#include <ctime>
#include <ft2build.h>
#include FT_FREETYPE_H  

#include "Character.h"
#include <glm\gtc\matrix_transform.hpp>



GameController::GameController(Shader *s, Model *character, Model *start, int laps)
{
	this->character = character;
	this->lapPosition = start->getPosition();
	this->laps = laps;
	gameStarted = false;
	seconds = 0;
	minutes = 0;
	currentLap = 0;
	finaltime = 0.0;

	FT_Library ft;
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	FT_Face face;
	if (FT_New_Face(ft, "Assets/fonts/audimb.TTF", 0, &face))
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

	FT_Set_Pixel_Sizes(face, 0, 52);

	if (FT_Load_Char(face, 'X', FT_LOAD_RENDER))
		std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction

	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
			);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	shader = s;

	setupRender();
}

void GameController::RenderText(std::string text, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color)
{
	// Activate corresponding render state	
	glDisable(GL_DEPTH_TEST);
	shader->use();
	glUniform3f(shader->getUniform("textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glEnable(GL_DEPTH_TEST);
}


GameController::~GameController()
{
}

void GameController::update(int width, int height) {
	//glm::mat4 projection = glm::perspective(0.0f, (float)width / (float)height, 0.0f, 500.0f);
	glm::mat4 projection = glm::ortho(0.0f, (float) width, 0.0f, (float) height);
	shader->use();
	glUniformMatrix4fv(shader->getUniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));


	
	glm::vec3 currentPos = character->getPosition();
	float zthresholdUp = lapPosition.z + threshold;
	float zthresholdDown = lapPosition.z - threshold;
	float xthresholdUp = lapPosition.x + threshold;
	float xthresholdDown = lapPosition.x - threshold;
	if (currentPos.z < zthresholdUp && currentPos.z > zthresholdDown) {
		if (currentPos.x < xthresholdUp && currentPos.x > xthresholdDown) {
			if (finaltime == 0.0) {
				if (!gameStarted) {
					glfwSetTime(0);
					startTime = glfwGetTime();
					currentLap = 0;

					gameStarted = true;
					cout << "Game Started!!" << endl;
				}
				else {

					double currentTime = glfwGetTime();
					if ((currentLap == 0 && currentTime - startTime > 10) ||
						currentLap > 0 && currentTime - lapTime > 10) {
						lapTime = currentTime;
						currentLap++;
						cout << "Lap " << currentLap - 1 << ": " << lapTime - startTime << endl;
						if (currentLap == laps) {
							gameStarted = false;
							finaltime = currentTime - startTime;
							cout << "Finished!! -- " << finaltime << endl;
							//Save result in file
						}
					}

				}
			}
		}
	}
	double time = glfwGetTime() - startTime;

	if (gameStarted) {
		seconds = (int)time;
		if (seconds % 60 == 0 && seconds != 0) {
			minutes++;
			glfwSetTime(0);
		}
		char min[5], secs[6];
		sprintf_s(min, "%02d",minutes);
		sprintf_s(secs, "%04.2f", time);
		string formatedTime = string(min)+ ":" + string(secs);
		RenderText(formatedTime,
			width - 300.0f, height - 100, 1.0f,
			glm::vec3(6.0, 0.0f, 0.0f));
	}
	else {
		if (finaltime == 0) {
			RenderText("00:00.00",
				width - 300.0f, height - 100, 1.0f,
				glm::vec3(6.0, 0.0f, 0.0f));
		}
		else {
			char min[5], secs[6];
			sprintf_s(min, "%02d", minutes);
			sprintf_s(secs, "%04.2f", time);
			string formatedTime = string(min) + ":" + string(secs);
			RenderText(formatedTime,
				width - 300.0f, height - 100, 1.0f,
				glm::vec3(6.0, 0.0f, 0.0f));
		}
	}
}

void GameController::setupRender() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}