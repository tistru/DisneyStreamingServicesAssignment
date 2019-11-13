#include "DisplayManager.h"
#include <iostream>
#include <fstream>


DisplayManager::DisplayManager()
{
	m_imageFiles.clear();
}


DisplayManager::~DisplayManager()
{
	m_imageFiles.clear();
}

// Public function to get the manager's vertex array object
GLuint &DisplayManager::GetVertexArrayObject() {
	return vao;
}

// Function to add a new image to the internal image list. Return the new size of the list
GLuint DisplayManager::AddNewImage(const char *imagePath) {
	m_imageFiles.push_back(imagePath);

	return m_imageFiles.size();
}

// Function to get the size of the internal image list
GLuint DisplayManager::getImageListSize() {
	return m_imageFiles.size();
}

// Initialize the data to be rendered
void DisplayManager::Initialize() {
	// Use a Vertex Array Object
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 1 square (made by 2 triangles) to be rendered
	GLfloat vertices_position[8] = {
		-0.5, -0.5,
		0.5, -0.5,
		0.5, 0.5,
		-0.5, 0.5,
	};

	GLfloat texture_coord[8] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0,
	};

	GLuint indices[6] = {
		0, 1, 2,
		2, 3, 0
	};

	// Create a Vector Buffer Object that will store the vertices on video memory
	GLuint vbo;
	glGenBuffers(1, &vbo);

	// Allocate space for vertex positions and texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_position) + sizeof(texture_coord), NULL, GL_STATIC_DRAW);

	// Transfer the vertex positions:
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices_position), vertices_position);

	// Transfer the texture coordinates:
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices_position), sizeof(texture_coord), texture_coord);

	// Create an Element Array Buffer that will store the indices array:
	GLuint eab;
	glGenBuffers(1, &eab);

	// Transfer the data from indices to eab
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eab);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create a texture
	GLuint texture;
	glGenTextures(1, &texture);

	// Specify that we work with a 2D texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Initialize the images array
	m_imageFiles.push_back("images/MLBStats_20190610_Game1.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game2.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game3.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game4.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game5.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game6.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game7.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game8.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game9.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game10.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game11.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game12.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game13.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game14.jpg");
	m_imageFiles.push_back("images/MLBStats_20190610_Game15.jpg");

	LoadImage(m_imageFiles[0].c_str());

	GLuint shaderProgram = CreateShaderProgram("shaders/vert.shader", "shaders/frag.shader");

	// Get the location of the attributes that enters in the vertex shader
	GLint position_attribute = glGetAttribLocation(shaderProgram, "position");

	// Specify how the data for position can be accessed
	glVertexAttribPointer(position_attribute, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Enable the attribute
	glEnableVertexAttribArray(position_attribute);

	// Texture coord attribute
	GLint texture_coord_attribute = glGetAttribLocation(shaderProgram, "texture_coord");
	glVertexAttribPointer(texture_coord_attribute, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(vertices_position));
	glEnableVertexAttribArray(texture_coord_attribute);
}


// Load an image from the disk with FreeImage
void DisplayManager::LoadImage(const char *fileName) {
	// active only for static linking
#ifdef FREEIMAGE_LIB
	FreeImage_Initialise();
#endif

	FIBITMAP *bitmap;
	// Get the format of the image file
	FREE_IMAGE_FORMAT fFormat = FreeImage_GetFileType(fileName, 0);

	// If the format can't be determined, try to guess the format from the file name
	if (fFormat == FIF_UNKNOWN) {
		fFormat = FreeImage_GetFIFFromFilename(fileName);
	}

	// Load the data in bitmap if possible
	if (fFormat != FIF_UNKNOWN && FreeImage_FIFSupportsReading(fFormat)) {
		bitmap = FreeImage_Load(fFormat, fileName);
	}
	else {
		bitmap = NULL;
	}

	// PROCESS IMAGE if bitmap was successfully initialized
	if (bitmap) {
		unsigned int w = FreeImage_GetWidth(bitmap);
		unsigned int h = FreeImage_GetHeight(bitmap);
		unsigned pixel_size = FreeImage_GetBPP(bitmap);

		// Get a pointer to the pixel data
		BYTE *data = (BYTE*)FreeImage_GetBits(bitmap);

		// Process only RGB and RGBA images
		if (pixel_size == 24) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else if (pixel_size == 32) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_BGRA, GL_UNSIGNED_BYTE, (GLvoid*)data);
		}
		else {
			std::cerr << "pixel size = " << pixel_size << " don't know how to process this case" << std::endl;
			exit(-1);
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	}
	else {
		std::cerr << "Unable to load the image file " << fileName << std::endl;
		exit(-1);
	}

	// Clean bitmap;
	FreeImage_Unload(bitmap);

	// active only for static linking
#ifdef FREEIMAGE_LIB
	FreeImage_DeInitialise();
#endif	
}

// Read a shader source from a file. store the shader source in a std::vector<char>
void DisplayManager::ReadShaderSource(const char *fileName, std::vector<char> &buffer) {
	std::ifstream in;
	in.open(fileName, std::ios::binary);

	if (in.is_open()) {
		// Get the number of bytes stored in this file
		in.seekg(0, std::ios::end);
		size_t length = (size_t)in.tellg();

		// Go to start of the file
		in.seekg(0, std::ios::beg);

		// Read the content of the file in a buffer
		buffer.resize(length + 1);
		in.read(&buffer[0], length);
		in.close();
		// Add a valid C - string end
		buffer[length] = '\0';
	}
	else {
		std::cerr << "Unable to open " << fileName << std::endl;
		exit(-1);
	}
}

// Compile a shader
GLuint DisplayManager::LoadAndCompileShader(const char *fileName, GLenum shaderType) {
	// Load a shader from an external file
	std::vector<char> buffer;
	ReadShaderSource(fileName, buffer);
	const char *src = &buffer[0];

	// Compile the shader
	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	// Check the result of the compilation
	GLint test;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &test);
	if (!test) {
		std::cerr << "Shader compilation failed with this message:" << std::endl;
		std::vector<char> compilation_log(512);
		glGetShaderInfoLog(shader, compilation_log.size(), NULL, &compilation_log[0]);
		std::cerr << &compilation_log[0] << std::endl;
		glfwTerminate();
		exit(-1);
	}
	return shader;
}

// Create a program from two shaders
GLuint DisplayManager::CreateShaderProgram(const char *vsPath, const char *psPath) {
	// Load and compile the vertex and fragment shaders
	GLuint vertexShader = LoadAndCompileShader(vsPath, GL_VERTEX_SHADER);
	GLuint fragmentShader = LoadAndCompileShader(psPath, GL_FRAGMENT_SHADER);

	// Attach the above shader to a program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// Flag the shaders for deletion
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Link and use the program
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	return shaderProgram;
}

// Draw scene
void DisplayManager::Draw(GLuint &vao, GLFWwindow *window) {
	glClear(GL_COLOR_BUFFER_BIT);

	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	// Swap front and back buffers
	glfwSwapBuffers(window);
}

void DisplayManager::UpdateImage(GLuint newIndex) {

	LoadImage(m_imageFiles[newIndex].c_str());
}
