#pragma once
#include <GL/glew.h>
#include <GL/glfw3.h>
#include <vector>
#include <FreeImage.h>

class DisplayManager
{
private:

	void ReadShaderSource(const char *, std::vector<char> &);
	GLuint LoadAndCompileShader(const char *, GLenum );
	GLuint CreateShaderProgram(const char *, const char *);

	GLuint vao;										// vertex array object
	
	std::vector <std::string> m_imageFiles;			// List of images to display

public:
	DisplayManager();
	~DisplayManager();

	void Initialize();
	void Draw(GLuint &, GLFWwindow *);
	void LoadImage(const char *);

	GLuint & GetVertexArrayObject();

	GLuint AddNewImage(const char *);

	GLuint getImageListSize();
	void UpdateImage(GLuint);
};

