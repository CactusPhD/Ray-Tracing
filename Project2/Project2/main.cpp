#include <fstream>
#include <string>
#include <assert.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <unordered_map>

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "vec.h"
#include "mat.h"
#include "quaternion.h"

#include "ShaderLoader.h"

#include "windows.h"
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}

GLuint shaderProagram;
//GLuint wheelShader;
//GLuint perspectiveLocation;
GLuint viewLocation;
//GLuint gColorLocation;
//GLuint gTrimLocation;
//GLuint gWheelColorLocation;
//GLuint pointLightIndex;
//GLuint ssbPoint;
//GLuint ssbDir;
//GLuint dirLightIndex;
GLuint eyeLocation;
GLuint xRotLocation;
GLuint yRotLocation;

float xPos = 0;
float zPos = 0;
float xRot = 0;
float yRot = 0;
//float bunX = 0;
//float bunY = 0;
//thanks sam

//vec4 max;
//vec4 min;
//vec3 mouseRotate = (0, 0, 0);
//float angle = 0;
vec3 eye = vec3(0.0, 0.0, 0.0);

mat4 view;
mat4 perspective;
mat4 model;
float nearZ = 0.1;
float farZ = 100.0;
float ar = 800 / 800;
float fov = DegreesToRadians * 60;

float vertices[]{
	-1.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	-1.0f, -1.0f, 0.0f
};

float perspectiveVertices[]{
	-tan(fov / 2)*ar, tan(fov / 2), -1.0,
	tan(fov / 2)*ar, tan(fov / 2), -1.0,
	tan(fov / 2)*ar, -tan(fov / 2), -1.0,
	-tan(fov / 2)*ar, -tan(fov / 2), -1.0
};

unsigned int indices[]{
	2, 1, 0,
	2, 3, 0
};

//std::vector<float> vertices;
//std::vector<unsigned int> faces;
//std::vector<float> normals;

//same struct in shader file
//i love data alignment
//struct pointLight {
//	vec3 position;
//	float temp1;
//	vec3 ambient;
//	float temp2;
//	vec3 diffuse;
//	float temp3;
//	vec3 specular;
//	float temp4;
//	vec3 attenuation;
//	float temp5;
//};
//
//struct directionalLight {
//	vec3 ambient;
//	float temp1;
//	vec3 diffuse;
//	float temp2;
//	vec3 specular;
//	float temp3;
//	vec3 direction;
//	float temp4;
//};
//
//pointLight light1;
//directionalLight light2;

void display() {
	glClear(GL_COLOR_BUFFER_BIT);
		//| GL_DEPTH_BUFFER_BIT
	
	//mat4 centerBunny = Translate((min + max)*-0.5);
	//std::cout << centerBunny << " center of bunny\n";
	mat4 centerCamera = Translate(0, 0, -10);
	mat4 cameraTranslate = Translate(xPos, 0.0, zPos);
	view = cameraTranslate*centerCamera;
	/*Quaternion quat(sin(angle / 2)*mouseRotate.x, sin(angle / 2)*mouseRotate.y, sin(angle / 2)*mouseRotate.z, cos(angle / 2));
	//mat4 rotation = quat.ConvertToRotationMatrix();*/
	mat4 xRotation = RotateX(xRot);
	mat4 yRotation = RotateY(yRot);
	//mat4 bunTranslate = Translate(bunX, bunY, 0.0);
	//model = bunTranslate*xRotation*yRotation*centerBunny;
	
	glUseProgram(shaderProagram);
	//glUniformMatrix4fv(perspectiveLocation, 1, GL_TRUE, &(perspective)[0][0]);
	//glUniformMatrix4fv(viewLocation, 1, GL_TRUE, &(view)[0][0]);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
	vec3 newEye = vec3(eye.x + xPos, eye.y, eye.z + zPos);
	glUniform3fv(eyeLocation, 1, &newEye[0]);
	glUniform1f(xRotLocation, xRot * DegreesToRadians);
	glUniform1f(yRotLocation, yRot * DegreesToRadians);
	//std::cout << newEye;

	//update ubo contents
	//this is the real light starting position
	/*vec4 lightPos = cameraTranslate*centerCamera*vec4(0.0, 0.5, 0.0, 1.0);
	light1.position.x = lightPos.x;
	light1.position.y = lightPos.y;
	light1.position.z = lightPos.z;
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbPoint);
	GLvoid* p = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(p, &light1, sizeof(pointLight));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/

	/*glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbDir);
	GLvoid* q = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
	memcpy(q, &light2, sizeof(directionalLight));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);*/
	//bonus Bunny
	/*glUseProgram(shaderProagram);
	glUniformMatrix4fv(gModelLocation, 1, GL_TRUE, &(perspective*camera*centerCamera*centerBunny)[0][0]);
	glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, (void*)0);*/

	glutSwapBuffers();
}

int timeNow;
int timePrev;
void idle() {
	timeNow = glutGet(GLUT_ELAPSED_TIME);
	int timeElapsed = timeNow - timePrev;
	if (timeElapsed > 16) {
		/*if (xP > 1 - 10*scale1)
			forwards = -1;
		else if (xP < -1)
			forwards = 1;

		float speed = SPEED_ROT_RATIO*scale1*scale2*rotSpeed;
		xP += forwards*speed;
		rot -= forwards*rotSpeed;*/

		//std::cout << "FPS: " << 1000.0f / timeElapsed << "\r";
		glutPostRedisplay();
		timePrev = timeNow;
	}
}

vec3 color(1, 1, 1);
float trim = 3;
float downX = 0;
float downY = 0;
float downZ = 1;
bool leftDown = false;
void keyboard(unsigned char key, int x, int y) {
	if (key == 'a' || key == 'w' || key == 's' || key == 'd') {
		switch (key) {
		case 'a':
			xPos -= 0.05;
			break;
		case 'w':
			zPos -= 0.1;
			break;
		case 's':
			zPos += 0.1;
			break;
		case 'd':
			xPos += 0.05;
			break;
		}
	}
	/*if (key == 'j' || key == 'i' || key == 'l' || key == 'k') {
			switch (key) {
			case 'j':
				bunX -= 0.01;
				break;
			case 'l':
				bunX += 0.01;
				break;
			case 'k':
				bunY -= 0.01;
				break;
			case 'i':
				bunY += 0.01;
				break;
			}
	}*/
		/*glUseProgram(shaderProagram);
		glUniform3fv(gColorLocation, 1, &color[0]);
		glUseProgram(wheelShader);
		glUniform3fv(gWheelColorLocation, 1, &color[0]);*/

	
	
	//glUseProgram(wheelShader);
	//glUniform1f(gTrimLocation, trim);
}

static void arrows(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT || key == GLUT_KEY_UP || key == GLUT_KEY_DOWN) {
		switch (key) {
		case GLUT_KEY_LEFT:
			yRot -= 1;
			break;
		case GLUT_KEY_RIGHT:
			yRot += 1;
			break;
		case GLUT_KEY_UP:
			xRot -= 1;
			break;
		case GLUT_KEY_DOWN:
			xRot += 1;
			break;
		}
	}
}

//static void mouse(int button, int state, int x, int y) {
//	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
//		float upX = (float)x / GLUT_SCREEN_WIDTH;
//		float upY = (float)y / GLUT_SCREEN_HEIGHT;
//		float upZ = 1 - pow(upX, 2) - pow(upY, 2);
//		if (upZ < 0) {
//			upZ = 0;
//		}
//		else {
//			upZ = sqrt(upZ);
//		}
//
//		vec3 mouseDown(downX, downY, downZ);
//		mouseDown = normalize(mouseDown);
//		std::cout << mouseDown << " down vector";
//		vec3 mouseUp(upX, upY, upZ);
//		mouseUp = normalize(mouseUp);
//		std::cout << mouseUp << " up vector\n";
//		angle = acos(dot(mouseDown, mouseUp));
//		mouseRotate = normalize(cross(mouseDown, mouseUp));
//		std::cout << angle << " angle " << mouseRotate << " axis of rotation\n";
//
//		downX = (float)x / GLUT_SCREEN_WIDTH;
//		downY = (float)y / GLUT_SCREEN_HEIGHT;
//		downZ = 1 - pow(downX, 2) - pow(downY, 2);
//		if (downZ < 0) {
//			downZ = 0;
//		}
//		else {
//			downZ = sqrt(downZ);
//		}
//	}
//}

static std::vector<std::string> split(std::string str, std::string sep) {
	/*char* cstr = const_cast<char*>(str.c_str());
	char* current;
	std::vector<std::string> arr;
	current = strtok(cstr, sep.c_str());
	while (current != NULL) {
		arr.push_back(current);
		current = strtok(NULL, sep.c_str());
	}
	return arr;*/
}

static void checkMinMax(vec4 v) {
	/*if (v.x > max.x) {
		max.x = v.x;
	}
	if (v.y > max.y) {
		max.y = v.y;
	}
	if (v.z > max.z) {
		max.z = v.z;
	}
	if (v.x < min.x) {
		min.x = v.x;
	}
	if (v.y < min.y) {
		min.y = v.y;
	}
	if (v.z < min.z) {
		min.z = v.z;
	}*/
}

static void readFile() {

	//std::ifstream fin("bunny.m");
	//if (fin.is_open()) {
	//	std::cout << "Bunny is Open \n";
	//	std::string s;
	//	std::getline(fin, s);
	//	//std::cout << s+"\n";
	//	std::vector<std::string> triangle = split(s, " ");
	//	std::cout << triangle[0]+"\n";

	//	//eat comments
	//	while (triangle[0] == "#") {
	//		std::getline(fin, s);
	//	    triangle = split(s, " ");
	//	}

	//	std::cout << "Reading "<< triangle[0] << "s\n";
	//	min = vec4(std::stof(triangle[2]), std::stof(triangle[3]), std::stof(triangle[4]),0.0);
	//	max = vec4(std::stof(triangle[2]), std::stof(triangle[3]), std::stof(triangle[4]),0.0);
	//	while (triangle[0] == "Vertex") {
	//		//std::cout << std::size(triangle);
	//	    vec4 v = vec4(std::stof(triangle[2]), std::stof(triangle[3]), std::stof(triangle[4]), 1.0);
	//		checkMinMax(v);
	//		//for normal
	//		triangle[5] = split(triangle[5], "(")[1];
	//		//std::cout << triangle[5] + "\n";
	//		triangle[7] = split(triangle[7], ")")[0];
	//		//std::cout << triangle[7] + "\n";
	//		vec4 n = vec4(std::stof(triangle[5]), std::stof(triangle[6]), std::stof(triangle[7]), 0.0);
	//		vertices.push_back(v.x);
	//		vertices.push_back(v.y);
	//		vertices.push_back(v.z);
	//		normals.push_back(n.x);
	//		normals.push_back(n.y);
	//		normals.push_back(n.z);
	//		std::getline(fin, s);
	//		triangle = split(s, " ");
	//	}

	//	std::cout << "Reading " << triangle[0] << "s\n";
	//	while (triangle[0] == "Face") {
	//		vec3 f = vec3(std::stoi(triangle[2])-1, std::stoi(triangle[3])-1, std::stoi(triangle[4])-1);
	//		faces.push_back(f.x);
	//		faces.push_back(f.y);
	//		faces.push_back(f.z);
	//		std::getline(fin, s);
	//		triangle = split(s, " ");
	//	}
	//}
	//fin.close();
	//std::cout << "Finished Reading Bunny\n\n";
	//std::cout << vertices.size() << " elements in vertices vector\n";
	//std::cout << normals.size() << " elements in normals vector\n";
	//std::cout << faces.size() << " elements in faces vector\n";
	//std::cout << vertices.size() / 3  << " vertices\n";
	//std::cout << normals.size() / 3 << " normals\n";
	//std::cout << faces.size() / 3 << " faces\n\n";
}

static void CreateVertexBuffer()
{

	// Create a vertex array object
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint positionVBO;
	glGenBuffers(1, &positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	GLuint rayVBO;
	glGenBuffers(1, &rayVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rayVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(perspectiveVertices), perspectiveVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);

	// Create and initialize a buffer object
	GLuint elementBuffer;
	glGenBuffers(1, &elementBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//create ssb
	// ssb index is bound in shader
	/*glGenBuffers(1, &ssbPoint);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbPoint);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(pointLight), &light1, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbPoint);*/

	/*glGenBuffers(1, &ssbDir);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbDir);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(directionalLight), &light2, GL_DYNAMIC_COPY);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, ssbDir);*/
	
}

void init() {
	//give light values
	//light1.ambient = vec3(1.0, 1.0, 1.0);
	//light1.attenuation = vec3(0.1, 0.0, 0.3);
	//light1.diffuse = vec3(1.0, 1.0, 1.0);
	////dummy position value, gets updated in display
	//light1.position = vec3(0.0, 0.0, 0.0);
	//light1.specular = vec3(1.0, 1.0, 1.0);

	//light2.ambient = vec3(0.5, 0.0, 0.0);
	//light2.diffuse = vec3(0.5, 0.0, 0.0);
	//light2.direction = vec3(-1.0, -1.0, 1.0);
	//light2.specular = vec3(0.5, 0.0, 0.0);

	CreateVertexBuffer();

	shaderProagram = loadShaders("Shaders/Vertex_Shader.vert", "Shaders/Fragment_Shader.frag");
	glUseProgram(shaderProagram);

	//perspectiveLocation = glGetUniformLocation(shaderProagram, "perspective");
	viewLocation = glGetUniformLocation(shaderProagram, "view");
	eyeLocation = glGetUniformLocation(shaderProagram, "eye");
	xRotLocation = glGetUniformLocation(shaderProagram, "xRot");
	yRotLocation = glGetUniformLocation(shaderProagram, "yRot");


	perspective = Perspective(60, ar, nearZ, farZ);
}

int main(int argc, char** argv)
{
	//readFile();
	//std::cout << min << " min \n";
	//std::cout << max << " max \n\n";

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("QUAD");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);
	//glutMouseFunc(mouse);
	glutSpecialFunc(arrows);
	//glEnable(GL_DEPTH_TEST);

	// Must be done after glut is initialized!
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	//make sure your GL version is at least 3
	printf("GL version: %s\n", glGetString(GL_VERSION));
	printf("Glut version: %i\n", (glutGet(GLUT_VERSION)));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	init();

	glutMainLoop();

	return 0;
}