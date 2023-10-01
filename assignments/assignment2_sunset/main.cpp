#include <stdio.h>
#include <math.h>


#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <HenLib/shader.h>
#include <HenLib/VertexData.h>

unsigned int createShader(GLenum shaderType, const char* sourceCode);
unsigned int createShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource);
unsigned int createVAO(float* vertexData, int numVertices);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);

const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;


unsigned int indices[6] =
{
	0,1,2,		 //Triangle 1
	2,3,0		 //Triangle 2
};

Vertex vertices[4] = {
	{-1, -1, 0.0, 0.0, 0.0}, //Bottom Left
	{ 1, -1, 0.0, 1.0, 0.0}, //Bottom Right
	{ 1,  1, 0.0, 1.0, 1.0}, //Top Right
	{-1,  1, 0.0, 0.0, 1.0}	 //Top Left
};

//Shader Parameter

float daySkyColor[3] = { 1.0f, 0.5f, 0.0f };
float dayGroundColor[3] = { 1.0f, 0.31f, 0.0f };
float nightSkyColor[3] = { 0.45f, 0.0f, 0.32f };
float nightGroundColor[3] = { 0.195f, 0.25f, 0.32f };
float sunTopColor[3] = { 1.0f, 0.545f, 0.0f };
float sunBottomColor[3] = { 1.0f, 0.2f, 0.0f };
float skylineColor[4] = { 0.0f, 0.0f, 0.0f, 0.8f};
float sunSpeed = 1.0f;
float sunRadius = 0.25f;
float hillAmount = 15.0f;
float hillSteepness = 0.1f;
bool showImGUIDemoWindow = true;

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Hello Triangle", NULL, NULL);
	if (window == NULL) {
		printf("GLFW failed to create window");
		return 1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	if (!gladLoadGL(glfwGetProcAddress)) {
		printf("GLAD Failed to load GL headers");
		return 1;
	}

	//Initialize ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();

	std::string vertexShaderSource = HenryLib::loadShaderSourceFromFile("assets/vertexShader.vert");
	std::string fragmentShaderSource = HenryLib::loadShaderSourceFromFile("assets/fragmentShader.frag");

	HenryLib::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");

	unsigned int vao = HenryLib::createVAO(vertices, 12, indices, 6);

	shader.use();
	glBindVertexArray(vao);

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//Set time
		float currentTime = glfwGetTime();


		//Set uniforms
		shader.setFloat("iTime", currentTime);
		shader.setVec2("iResolution", SCREEN_WIDTH, SCREEN_HEIGHT);
		shader.setVec3("_daySkyColor", daySkyColor[0], daySkyColor[1], daySkyColor[2]);
		shader.setVec3("_dayGroundColor", dayGroundColor[0], dayGroundColor[1], dayGroundColor[2]);
		shader.setVec3("_nightSkyColor", nightSkyColor[0], nightSkyColor[1], daySkyColor[2]);
		shader.setVec3("_nightGroundColor", nightGroundColor[0], nightGroundColor[1], nightGroundColor[2]);
		shader.setVec3("_sunTopColor", sunTopColor[0], sunTopColor[1], sunTopColor[2]);
		shader.setVec3("_sunBottomColor", sunBottomColor[0], sunBottomColor[1], sunBottomColor[2]);
		shader.setVec4("_skylineColor", skylineColor[0], skylineColor[1], skylineColor[2], skylineColor[3]);
		shader.setFloat("_sunSpeed", sunSpeed);
		shader.setFloat("_sunRadius", sunRadius);
		shader.setFloat("_hillAmount", hillAmount);
		shader.setFloat("_hillSteepness", hillSteepness);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);


		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Checkbox("Show Demo Window", &showImGUIDemoWindow);

			//Colors
			ImGui::ColorEdit3("Day Sky Color", daySkyColor);
			ImGui::ColorEdit3("Day Ground Color", dayGroundColor);
			ImGui::ColorEdit3("Night Sky Color", nightGroundColor);
			ImGui::ColorEdit3("Night Groun Color", nightSkyColor);
			ImGui::ColorEdit3("Sun Top Color", sunTopColor);
			ImGui::ColorEdit3("Sun Bottom Color", sunBottomColor);
			ImGui::ColorEdit3("Skyline Color", skylineColor);

			//Speed & Radius
			ImGui::SliderFloat("Speed", &sunSpeed, 0.1f, 2.0f);
			ImGui::SliderFloat("Radius", &sunRadius, 0.1f, 0.5f);

			//Hill Controls
			ImGui::SliderFloat("Hill Amount", &hillAmount, 5.0f, 30.0f);
			ImGui::SliderFloat("Hill Steepnes", &hillSteepness, 0.01f, 0.3f);

			ImGui::End();
			if (showImGUIDemoWindow) {
				ImGui::ShowDemoWindow(&showImGUIDemoWindow);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		}

		glfwSwapBuffers(window);
	}
	printf("Shutting down...");
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

