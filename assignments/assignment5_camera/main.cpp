#include <stdio.h>
#include <math.h>

#include <ew/external/glad.h>
#include <ew/ewMath/ewMath.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ew/shader.h>
#include <ew/procGen.h>
#include <ew/transform.h>
#include "HenLib/camera.h"

void framebufferSizeCallback(GLFWwindow* window, int width, int height);

//Projection will account for aspect ratio!
const int SCREEN_WIDTH = 1080;
const int SCREEN_HEIGHT = 720;

const int NUM_CUBES = 4;
ew::Transform cubeTransforms[NUM_CUBES];

void moveCamera(GLFWwindow* window, HenLib::Camera* camera, HenLib::CameraControls* controls) {
	//If right mouse is not held, release cursor and return early
	if (!glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_2)) {
		//Release cursor
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		controls->firstMouse = true;
		return;
	}
	//GLFW_CURSOR_DISABLED hides the cursor, but the position will still be changed as we move your mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Get screen mouse position this frame
	double mouseX, mouseY;
	glfwGetCursorPos(window, &mouseX, &mouseY);

	//If we just start right clicking, set prevMouse values to current position
	//This prevents a bug where the camera moves as soon as we click
	if (controls->firstMouse) {
		controls->firstMouse = false;
		controls->prevMouseX = mouseX;
		controls->prevMouseY = mouseY;
	}

	//Get mouse position delta for this frame
	float xOffset = mouseX - controls->prevMouseX;
	float yOffset = mouseY - controls->prevMouseY;

	//Add to yaw and pitch
	controls->yaw += xOffset;
	controls->pitch -= yOffset;

	//Clamp pitch between -89 and 89 degrees
	if (controls->pitch > 89.0f) {
		controls->pitch = 89.0f;
	}
	if (controls->pitch < -89.0f) {
		controls->pitch = -89.0f;
	}

	//Remember previous mouse position
	controls->prevMouseX = mouseX;
	controls->prevMouseY = mouseY;

	//Contstruct forward vector using yaw and pitch
	float yawRadian = ew::Radians(controls->yaw);
	float pitchRadian = ew::Radians(controls->pitch);
	ew::Vec3 forward;
	forward.x = cos(yawRadian) * cos(pitchRadian);
	forward.y = sin(pitchRadian);
	forward.z = sin(yawRadian) * cos(pitchRadian);
	forward = ew::Normalize(forward);

	camera->target = camera->position + forward;
}

int main() {
	printf("Initializing...");
	if (!glfwInit()) {
		printf("GLFW failed to init!");
		return 1;
	}

	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Camera", NULL, NULL);
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

	//Enable back face culling
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//Depth testing - required for depth sorting!
	glEnable(GL_DEPTH_TEST);

	ew::Shader shader("assets/vertexShader.vert", "assets/fragmentShader.frag");
	
	//Camera
	HenLib::Camera camera;
	HenLib::CameraControls cameraControls;
	camera.orthographic = false;
	camera.position = ew::Vec3(0, 0, 5);
	camera.target = (0, 0, 0);
	camera.fov = 60.0;
	camera.orthoSize = 6.0;
	camera.nearPlane = 0.1;
	camera.farPlane = 100;
	camera.aspectRatio = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);

	//Cube mesh
	ew::Mesh cubeMesh(ew::createCube(0.5f));

	//Cube positions
	for (size_t i = 0; i < NUM_CUBES; i++)
	{
		cubeTransforms[i].position.x = i % (NUM_CUBES / 2) - 0.5;
		cubeTransforms[i].position.y = i / (NUM_CUBES / 2) - 0.5;
	}

	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();
		moveCamera(window, &camera, &cameraControls);
		glClearColor(0.3f, 0.4f, 0.9f, 1.0f);
		//Clear both color buffer AND depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Set uniforms
		shader.use();

		//Set model matrix uniform
		for (size_t i = 0; i < NUM_CUBES; i++)
		{
			//View Projection
			ew::Mat4 viewProjection = camera.ProjectionMatrix() * camera.ViewMatrix() * cubeTransforms[i].getModelMatrix();
			shader.setMat4("_WVC", viewProjection);
			cubeMesh.draw();
		}

		//Render UI
		{
			ImGui_ImplGlfw_NewFrame();
			ImGui_ImplOpenGL3_NewFrame();
			ImGui::NewFrame();

			ImGui::Begin("Settings");
			ImGui::Text("Camera");
			ImGui::Checkbox("Orthographic", &camera.orthographic);
			if (camera.orthographic){
				ImGui::SliderFloat("Ortho Height", &camera.orthoSize, 0.0f, 10.0f);
			}
			else {
				ImGui::SliderFloat("FOV", &camera.fov, 0.0f, 180.0f);
			}

			ImGui::DragFloat3("Position", &camera.position.x, 0.05f);
			ImGui::DragFloat3("Target", &camera.target.x, 0.05f);
			ImGui::SliderFloat("Near Plane", &camera.nearPlane, 0.1f, 180.0f);
			ImGui::SliderFloat("Far Plane", &camera.farPlane, 0.0f, 100.0f);
			ImGui::End();
			
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

