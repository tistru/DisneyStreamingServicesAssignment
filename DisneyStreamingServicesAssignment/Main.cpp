#include <GL/glew.h>
#include <GL/glfw3.h>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <include/json/json.h>
#include "JSONParser.h"
#include "DisplayManager.h"

// Called when the window is resized
void ResizeCallback(GLFWwindow* window, int width, int height);

// Called for keyboard events
void InputCallback(GLFWwindow *window);

// Global variable to store image index
GLint imageIndex = 0;

// Create DisplayManager Object
DisplayManager *DM = new DisplayManager();


int main(void) {
	
	std::string inputJsonFile;

	int MLBFileID = 0;

	while (MLBFileID == 0) {
		std::cout << "Please specify the MLB JSON file to parse[1-3]: 1=[2018-06-10], 2=[2018-08-10], 3=[2019-06-10]\n";
		std::cin >> MLBFileID;

		if (MLBFileID < 1 || MLBFileID > 3) {
			std::cout << "Invalid file. Please re-enter a value between 1 and 3 \n";
			MLBFileID = 0;
		}
	}

	switch (MLBFileID)
	{
	case 1: inputJsonFile = "jsonfiles/mlbstats.json";
		break;
	case 2: inputJsonFile = "jsonfiles/mlbstats2.json";
		break;
	case 3: inputJsonFile = "jsonfiles/mlbstats3.json";
		break;
	default:
		break;
	}

	JSONParser *MLBParser = new JSONParser(inputJsonFile.c_str());

	MLBParser->ParseJsonFile();

	// Print out the data in the lists for each game
	std::cout << "Size of Dates array = " << MLBParser->GetDatesList().size() << std::endl;
	std::cout << "Size of Headlines array = " << MLBParser->GetHeadlinesList().size() << std::endl;
	std::cout << "Size of Blurbs array = " << MLBParser->GetBlurbsList().size() << std::endl;
	std::cout << "Size of Thumbnails array = " << MLBParser->GetThumbnailsList().size() << std::endl;
	
	std::cout << "\n";
	
	MLBParser->PrintArrayData(MLBParser->GetDatesList(), "Game Dates");
	MLBParser->PrintArrayData(MLBParser->GetHeadlinesList(), "Headlines");
	MLBParser->PrintArrayData(MLBParser->GetBlurbsList(), "Blurbs");
	MLBParser->PrintArrayData(MLBParser->GetThumbnailsList(), "Thumbnails");

	delete MLBParser;

	// Initialize GLFW
	if (!glfwInit()) {
		std::cerr << "Failed to initialize GLFW! I'm out!" << std::endl;
		exit(-1);
	}

	// Use OpenGL 3.2 core profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

	// Open a window and attach an OpenGL rendering context to the window surface
	GLFWwindow *window = glfwCreateWindow(800, 600, "MLB Game Viewer", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Print the OpenGL version
	int major, minor, rev;
	glfwGetVersion(&major, &minor, &rev);
	std::cout << "OpenGL - " << major << "." << minor << "." << rev << std::endl;

	// Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		std::cerr << "Failed to initialize GLEW! I'm out!" << std::endl;
		glfwTerminate();
		exit(-1);
	}

	// Initialize the data to be rendered
	DM->Initialize();

	// Create a rendering loop
	//int running = GL_TRUE;

	while (!glfwWindowShouldClose(window))
	{
		// check for input events
		InputCallback(window);

		// Display scene
		DM->Draw(DM->GetVertexArrayObject(), window);

		// Pool for events
		glfwPollEvents();

		// Check if the window was rezized
		glfwSetFramebufferSizeCallback(window, ResizeCallback);
	}

	// Terminate GLFW
	glfwTerminate();

	delete DM;

	return 0;
}

/***************** CALLBACKS ********************/
// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void InputCallback(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		imageIndex++;

		if (imageIndex >= DM->getImageListSize()) {
			imageIndex = 0;
		}

		DM->UpdateImage(imageIndex);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		imageIndex--;

		if (imageIndex < 0) {
			imageIndex = DM->getImageListSize() - 1;
		}

		DM->UpdateImage(imageIndex);
	}
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void ResizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}
/*********************************************/