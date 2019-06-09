
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include<iostream>
#include "Shader.h"
#include "stb_image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include <assimp/scene.h>
#include "Model.h"
#include<filesystem>
//#include<string>

float mixPercent = 0.2f;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//float cameraSpeed = 0.05f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

// mouse
float lastX = 400;
float lastY = 300;

//float pitch = 0.0f;
//float yaw = -90.0f;
bool firstMouse= true;

float fov = 60.0f;

glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	//cameraSpeed = 2.5f * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		mixPercent += 0.1f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		mixPercent -= 0.1f;
	}
	if (mixPercent >= 1)
		mixPercent = 1;
	if (mixPercent <= 0)
		mixPercent = 0;

	// camera strafe
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
	/*float sensitivity = 0.05f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
	{
		pitch = 89.0f;
	}
	if (pitch < -89.0f)
	{
		pitch = -89.0f;
	}

	glm::vec3 front;
	front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
	cameraFront = glm::normalize(front);*/
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

int main()
{
	glfwInit();
	//all enum see http://www.glfw.org/docs/latest/window.html#window_hints
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// core-profile
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);// for mac os x
	

	// 宽，高，窗口名
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	//设置为当前上下文
	glfwMakeContextCurrent(window);

	// 隐藏鼠标光标
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 注册指针回调
	glfwSetCursorPosCallback(window, mouse_callback);
	// 鼠标滚轮回调
	glfwSetScrollCallback(window, scroll_callback);

	// GLAD管理OpenGL的函数指针，调用任何OpenGL函数之前需要初始化GLAD
	// glfwGetProcAddress 加载系统相关的OpenGL函数指针地址的函数
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	float screenWidth = 800;
	float screenHeight = 600;
	// viewport
	glViewport(0, 0, screenWidth, screenHeight);
	// 注册回调
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// enable depth testing
	glEnable(GL_DEPTH_TEST);
	// set comparison operator or depth function, GL_LESS default
	//glDepthFunc(GL_ALWAYS);
	// value in depth buffer is between 0 and 1


	// stencil test 模板测试
	//glEnable(GL_STENCIL_TEST);
	// AND operate with all spencil value 与操作
	//glStencilMask(0xFF);

	// p1 stencil test function
	// p2 compare/ref value
	// p3 mask, AND with both ref value and stored stencil value before compare 与操作
	//glStencilFunc(GL_EQUAL, 1, 0xFF);

	// stencil test before depth test
	// p1 when stencil test fail
	// p2 when stencil test pass success, but depth test fail
	// p3 when depth test pass
	//glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

	//https://learnopengl-cn.github.io/04%20Advanced%20OpenGL/02%20Stencil%20testing/
	//TODO: Draw outlining

	Shader ourShader("shader.vs", "shader.fs");
	Shader lampShader("lamp.vs", "lamp.fs");

	Model ourModel("Model/nanosuit.obj");
	
	// cube and normal and texture coords
	//float vertices[] = {
	//	// positions          // normals           // texture coords
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	//	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	//	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	//	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
	//	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
	//	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	//	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
	//	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
	//	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
	//	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	//	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
	//	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
	//	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	//};

	//unsigned int indices[] = {
	//	0, 1, 3,
	//	1, 2, 3
	//};


	/* light */
	//unsigned int VBO;
	//unsigned int lightVAO;
	//glGenVertexArrays(1, &lightVAO);
	//glBindVertexArray(lightVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	//glEnableVertexAttribArray(0);



	// wireframe mode
	// p1 前后两面，p2 用线绘制
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 输出顶点属性的个数上限，一般至少为16个，且为四个成员
	/*int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;*/

	/* old fashion maps, and we prefer using pbr now*/
	// diffuse maps
	//unsigned int diffuseMap = loadTexture("D:/Tools/opengltest/learnopengl/learnopengl/container2.png");
	// specular maps
	//unsigned int specularMap = loadTexture("D:/Tools/opengltest/learnopengl/learnopengl/container2_specular.png");
	// shader configuration
	// --------------------
	//ourShader.use();
	//ourShader.setInt("material.diffuse", 0);
	//ourShader.setInt("material.specular", 1);


	/* 矩阵变换 */
	// suggested init way
	/*glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));*/


	/* 坐标系变换矩阵 */
	// model matrix | local->world
	//glm::mat4 model = glm::mat4(1.0f);
	//model = glm::rotate(model, glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

	// view matrix
	/*glm::mat4 view = glm::mat4(1.0f);
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));*/
	

	// projection matrix | view->clip 投影矩阵
	glm::mat4 projection;
	// p1 FOV
	// p2 aspect ratio 宽高比
	// p3 p4 near and far plane
	/*projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);*/


	/* camera | Gram-Schmidt Process*/
	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	//glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
	//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
	//glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	//glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
	//glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


	//glm::vec3 cubePositions[] = {
	//	glm::vec3(0.0f,  0.0f,  0.0f),
	//	glm::vec3(2.0f,  5.0f, -15.0f),
	//	glm::vec3(-1.5f, -2.2f, -2.5f),
	//	glm::vec3(-3.8f, -2.0f, -12.3f),
	//	glm::vec3(2.4f, -0.4f, -3.5f),
	//	glm::vec3(-1.7f,  3.0f, -7.5f),
	//	glm::vec3(1.3f, -2.0f, -2.5f),
	//	glm::vec3(1.5f,  2.0f, -2.5f),
	//	glm::vec3(1.5f,  0.2f, -1.5f),
	//	glm::vec3(-1.3f,  1.0f, -1.5f)
	//};
	// positions of the point lights
	//glm::vec3 pointLightPositions[] = {
	//	glm::vec3(0.7f,  0.2f,  2.0f),
	//	glm::vec3(2.3f, -3.3f, -4.0f),
	//	glm::vec3(-4.0f,  2.0f, -12.0f),
	//	glm::vec3(0.0f,  0.0f, -3.0f)
	//};

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// counting DeltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		// render
		//...
		//设置clear时设置的颜色
		//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//指定清空的buffer，此处为颜色
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// activate program object
		//glUseProgram(shaderProgram);
		ourShader.use();	
		// 设置shader里struct的方法
		//ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		//ourShader.setInt("material.diffuse", 0);
		//ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ourShader.setFloat("material.shininess", 64.0f);
		ourShader.setVec3("viewPos", camera.Position.x, camera.Position.y, camera.Position.z);

		//glm::vec3 lightColor;
		//lightColor.x = sin(glfwGetTime() * 2.0f);
		//lightColor.y = sin(glfwGetTime() * 0.7f);
		//lightColor.z = sin(glfwGetTime() * 1.3f);
		//glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		//ourShader.setVec3("light.ambient", ambientColor.x, ambientColor.y, ambientColor.z);
		//ourShader.setVec3("light.diffuse", diffuseColor.x, diffuseColor.y, diffuseColor.z);

		// directional light
		//ourShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		//ourShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		//ourShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		//ourShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);


		// point light 1
	/*	ourShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		ourShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[0].constant", 1.0f);
		ourShader.setFloat("pointLights[0].linear", 0.09);
		ourShader.setFloat("pointLights[0].quadratic", 0.032);*/
		// point light 2
		/*ourShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		ourShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[1].constant", 1.0f);
		ourShader.setFloat("pointLights[1].linear", 0.09);
		ourShader.setFloat("pointLights[1].quadratic", 0.032);*/
		// point light 3
		/*ourShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		ourShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[2].constant", 1.0f);
		ourShader.setFloat("pointLights[2].linear", 0.09);
		ourShader.setFloat("pointLights[2].quadratic", 0.032);*/
		// point light 4
		/*ourShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		ourShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		ourShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		ourShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		ourShader.setFloat("pointLights[3].constant", 1.0f);
		ourShader.setFloat("pointLights[3].linear", 0.09);
		ourShader.setFloat("pointLights[3].quadratic", 0.032);*/


		// spotLight
		//ourShader.setVec3("spotLight.position", camera.Position);
		//ourShader.setVec3("spotLight.direction", camera.Front);
		//ourShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		//ourShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		//ourShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		//ourShader.setFloat("spotLight.constant", 1.0f);
		//ourShader.setFloat("spotLight.linear", 0.09);
		//ourShader.setFloat("spotLight.quadratic", 0.032);
		//ourShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		//ourShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));


		// point light Attenuation
		//http://wiki.ogre3d.org/tiki-index.php?page=-Point+Light+Attenuation
		//ourShader.setFloat("light.constant", 1.0f);
		//ourShader.setFloat("light.linear", 0.09f);
		//ourShader.setFloat("light.quadratic", 0.09f);

		
		//float timeValue = glfwGetTime();
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		//// uniform是全局唯一的
		//int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");


		/* 矩阵变换 */
		// suggested init way
		//glm::mat4 trans = glm::mat4(1.0f);
		// 因为是矩阵,实际执行顺序与代码顺序相反，此处为先旋转后位移
		//trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		//trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
		/*float scaleAmount = sin(glfwGetTime());
		if (scaleAmount < 0)
			scaleAmount = -scaleAmount;
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));*/

		//glm::mat4 model = glm::mat4(1.0f);
		//model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
		glm::mat4 model = glm::mat4(1.0f);
		/*float radius = 10.0f;
		float camX = sin(glfwGetTime()) * radius;
		float camZ = cos(glfwGetTime()) * radius;*/
		//glm::mat4 view = glm::mat4(1.0f);
		// 位置，目标，上向量
		glm::mat4 view = camera.GetViewMatrix();//glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		projection = glm::perspective(glm::radians(camera.Zoom), screenWidth / screenHeight, 0.1f, 100.0f);

		/* 坐标系变换矩阵 */
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(ourShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		ourModel.Draw(ourShader);

		//0为默认纹理单元，至少有16个纹理单元
		//可以通过GL_TEXTURE0 + 8的方式获得GL_TEXTURE8
		//glActiveTexture(GL_TEXTURE0);
		//// 自动把纹理赋值给fragment shader的采样器sampler
		//glBindTexture(GL_TEXTURE_2D, texture1);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_2D, texture2);

		//glBindVertexArray(cubeVAO);
		//p1绘制类型，p2顶点数组的起始索引，p3绘制多少个顶点
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//p2:绘制顶点数,p3索引类型,p4偏移量，也可以传索引数组(没使用VBO时)
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		// for light
 	//	lampShader.use();
		//lampShader.setMat4("projection", projection);
		//lampShader.setMat4("view", view);

		//glBindVertexArray(lightVAO);
		//for (unsigned int i = 0; i < 4; i++)
		//{
		//	model = glm::mat4(1.0f);
		//	model = glm::translate(model, pointLightPositions[i]);
		//	model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		//	lampShader.setMat4("model", model);
		//	glDrawArrays(GL_TRIANGLES, 0, 36);
		//}

		// swap 前后 buffer
		glfwSwapBuffers(window);
		// 调用回调
		glfwPollEvents();
	}

	//glDeleteVertexArrays(1, &cubeVAO);
	//glDeleteVertexArrays(1, &lightVAO);
	//glDeleteBuffers(1, &VBO);

	// 释放资源
	glfwTerminate();
	return 0;
}