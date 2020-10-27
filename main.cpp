#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "VertexManager.h"
#include "ShaderManager.h"
#include "VariableManager.h"
#include "flock.h"

VertexManager vm = VertexManager();
ShaderManager sm = ShaderManager();
VariableManager varm = VariableManager();
Flock flock = Flock();

int main(void)
{

	// GL 3.0 + GLSL 130
	const char* glsl_version = "#version 130";
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Error loading GLFW library" << std::endl;
		return -1;
	}
	
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 480, "Flock Simulation", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Initialize imgui*/
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	/*Init GLEW lib*/
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error loading GLEW library" << std::endl;
		return -1;
	}

	/* Temporary Data Filler */
	float pos[12] =
	{
		-0.5, -0.5, 0.0, 1.0,
		 0.5, -0.5, 0.0, 1.0,
		 0.0,  1.0, 0.0, 1.0
		 
	};
	// Load data into the vertex manager
	VGroup triangle = newVGroup(3, pos);
	defineAttribute(triangle, POS, 3);
	vm.loadVertGroup(triangle);

	/* Temporary Index Data */
	unsigned int indicies[3] =
	{
		2, 1, 0
	};
	// Load index data into the vertex manager
	VGroup indexBuf = newIGroup(3, indicies);
	defineAttribute(indexBuf, INDEX, NULL);
	vm.loadIndexGroup(indexBuf);

	// Initialize and load the shader into the shader manager
	// TODO: Introduce compute shader to handle larger groups of foids
	SSet defaultShaders = newShaderSet("src/default.shader");
	sm.loadShaders(defaultShaders);

	// Initialize flock attributes
	bool speedVari = flock.speedVari;
	float speedFactor = flock.speedFactor;
	glm::vec2 awareVec = flock.awareVec;
	glm::vec3 behavInfluence = flock.behavInfluence;

	// Generate the flock
	flock.populateFlock();
	// Resize matrix for each flock member
	glm::mat4 shrink = glm::scale(glm::mat4(1.0f), glm::vec3(0.03, 0.03, 0.03));
	//glm::mat4 matA = flock.getModelMat(0) * shrink;
	//glm::mat4 matB = flock.getModelMat(1) * shrink;
	// Add model matrix and color uniforms to variable manager
	int MVP = varm.addGlobalMat(defaultShaders->programID, "MVP", glm::mat4(1.0f));
	int colorUniform = varm.addGlobalVec(defaultShaders->programID, "u_color", &(glm::vec4(1.0f)[0]));

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		/* Start ImGui Frame */
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Render flock control GUI
		ImGui::Begin("Flock Controls");

		ImGui::Text("Speed Controls");
		ImGui::SliderFloat("Speed", &flock.speedFactor, 0.0f, 2.0f);
		ImGui::Checkbox("Speed Variation", &flock.speedVari);

		ImGui::Text("\nFlock Member Eyesight");
		ImGui::SliderFloat("Distance", &flock.awareVec[RADIUS], 0.0f, 1.0f);
		ImGui::SliderFloat("Cone of Vision", &flock.awareVec[DEGREE], 0.0f, 1.0f);
			
		ImGui::Text("\nBehavior Controls");
		ImGui::SliderFloat("Avoidance", &flock.behavInfluence[AVOID], -1.0f, 1.0f );
		ImGui::SliderFloat("Alignment", &flock.behavInfluence[ALIGN], -1.0f, 1.0f);
		ImGui::SliderFloat("Attraction", &flock.behavInfluence[ATTRACT], -1.0f, 1.0f);

		if (ImGui::Button("Randomize"))
		{
			flock.awareVec = glm::vec2(((float)rand() / RAND_MAX),
				((float)rand() / RAND_MAX));
			flock.behavInfluence = glm::vec3((((float)rand() / RAND_MAX) * 2.0) - 1.0,
				(((float)rand() / RAND_MAX) * 2.0) - 1.0,
				(((float)rand() / RAND_MAX) * 2.0) - 1.0);
		}

		ImGui::Text("\nApplication average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		/* Render ImGui Window */
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		/* Draw faces */
		glm::mat4 M, V, P;
		for (int i = 0; i < FLOCK_SIZE; i++)
		{
			// Get model matrix
			M = flock.getModelMat(i) * shrink;
			// Get projection matrix
			P = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 1.0, 1.0));
			// Get view matrix
			V = glm::translate(glm::mat4(1.0f), glm::vec3(-1.0, 0.0, 0.0));
			// Calculate overall matrix and update it in the variable manager
			varm.updateGlobalMat(MVP, P * V * M);
			varm.updateGlobalVec(colorUniform, &(flock.population[i].color[0]));
			// Draw the foid
			glDrawElements(GL_TRIANGLES, indexBuf->elements, GL_UNSIGNED_INT, nullptr);
		}
		// Update all flock behavior
		flock.update();
		
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	vm.close();
	sm.close();
	flock.freeFlock();

	glfwTerminate();
	return 0;
}