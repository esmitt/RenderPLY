#define GLFW_DLL
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GLSLProgram.h"
#include "3DModel.h"
#include "ArcBall.h"
#include <stdlib.h>
#include <string>
#include <iostream>

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#pragma comment(lib, "glfw3dll.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")

using namespace std;

///< Only wrapping the glfw functions
namespace glfwFunc
{
	GLFWwindow* glfwWindow;
	const unsigned int WINDOW_WIDTH = 1024;
	const unsigned int WINDOW_HEIGHT = 650;
	const float NCP = 0.01f;
	const float FCP = 52.f;
	const float fAngle = 45.f;
	string strNameWindow = "Hello GLFW";
	glm::vec4 m_vec4ColorAB;
	glm::vec4 BLACK = glm::vec4(0, 0, 0, 1);
	glm::vec4 m_vec4ColorC;
	C3DModel m_model, m_cone;
	CArcBall m_arcball;
	bool m_bLeftButton;
	bool m_bFlag;
	glm::ivec2 m_MousePoint(0);

	CGLSLProgram m_program;
	glm::mat4x4 mProjMatrix, mModelViewMatrix;

	///
	/// Init all data and variables.
	/// @return true if everything is ok, false otherwise
	///
	bool initialize()
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glPolygonOffset(1, 1);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glewExperimental = GL_TRUE;
		m_bLeftButton = m_bFlag = false;
		if (glewInit() != GLEW_OK)
		{
			cout << "- glew Init failed :(" << endl;
			return false;
		}
		std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
		//load the shaders
		m_program.loadShader("shaders/basic.vert", CGLSLProgram::VERTEX);
		m_program.loadShader("shaders/basic.frag", CGLSLProgram::FRAGMENT);
		m_program.create_link();
		m_program.enable();
		m_program.addAttribute("vVertex");
		m_program.addUniform("mProjection");
		m_program.addUniform("mModelView");
		m_program.addUniform("vec4Color");
		m_program.disable();
		m_model.load("geometry/surfaceAB.ply");
		m_cone.load("geometry/surfaceC.ply");
		m_vec4ColorAB = glm::vec4(1, 1, 1 , 0.3);
		m_vec4ColorC = glm::vec4(0.1, 0.01, 0.6, 1.0);
		return true;
	}

	///< Callback function used by GLFW to capture some possible error.
	void errorCB(int error, const char* description)
	{
		cout << description << endl;
	}

	void onMouseMove(GLFWwindow *window, double xpos, double ypos)
	{
		if (m_bLeftButton)
		{
			if (!m_bFlag)
			{
				m_bFlag = true;
				m_arcball.OnMouseDown(glm::ivec2(xpos, ypos));
			}
			else
			{
				m_MousePoint.x = xpos;
				m_MousePoint.y = ypos;
				m_arcball.OnMouseMove(m_MousePoint, ROTATE);
			}
		}
	}

	void onMouseDown(GLFWwindow* window, int button, int action, int mods)
	{
		if (action == GLFW_PRESS)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_bLeftButton = true;
			}
		}
		else if (action == GLFW_RELEASE)
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
			{
				m_bLeftButton = false;
				m_bFlag = false;
				m_arcball.OnMouseUp(m_MousePoint);
			}
		}
	}

	///
	/// The keyboard function call back
	/// @param window id of the window that received the event
	/// @param iKey the key pressed or released
	/// @param iScancode the system-specific scancode of the key.
	/// @param iAction can be GLFW_PRESS, GLFW_RELEASE or GLFW_REPEAT
	/// @param iMods Bit field describing which modifier keys were held down (Shift, Alt, & so on)
	///
	void keyboardCB(GLFWwindow* window, int iKey, int iScancode, int iAction, int iMods)
	{
		if (iAction == GLFW_PRESS)
		{
			switch (iKey)
			{
			case GLFW_KEY_ESCAPE:
			case GLFW_KEY_Q:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;
			}
		}
	}

	///< The resizing function
	void resizeCB(GLFWwindow* window, int iWidth, int iHeight)
	{
		if (iHeight == 0) iHeight = 1;
		float ratio = iWidth / float(iHeight);
		glViewport(0, 0, iWidth, iHeight);
		mProjMatrix = glm::perspective(fAngle, ratio, NCP, FCP);
		m_arcball.Resize(iWidth, iHeight);
	}

	///< The main rendering function.
	void draw()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClearColor(0.94f, 0.94f, 0.94f, 0.f);
		glClearColor(0.54f, 0.54f, 0.54f, 0.f);
		mModelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, -6.f)) * m_arcball.GetTransformation();
		m_program.enable();
			glUniformMatrix4fv(m_program.getLocation("mModelView"), 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));
			glUniformMatrix4fv(m_program.getLocation("mProjection"), 1, GL_FALSE, glm::value_ptr(mProjMatrix));
			mModelViewMatrix = glm::translate(glm::mat4(), glm::vec3(0, 0, -6.f)) * glm::scale(glm::mat4(), glm::vec3(0.6, 0.6, 0.6)) * m_arcball.GetTransformation();
			glUniformMatrix4fv(m_program.getLocation("mModelView"), 1, GL_FALSE, glm::value_ptr(mModelViewMatrix));

			glUniform4fv(m_program.getLocation("vec4Color"), 1, glm::value_ptr(m_vec4ColorC));
			m_cone.drawObject();
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glPolygonOffset(1, 1);
			glLineWidth(2.0f);
			glUniform4fv(m_program.getLocation("vec4Color"), 1, glm::value_ptr(BLACK));
			m_cone.drawObject();
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glLineWidth(1.0);
			glUniform4fv(m_program.getLocation("vec4Color"), 1, glm::value_ptr(m_vec4ColorAB));
			m_model.drawObject();
			
		m_program.disable();

		glfwSwapBuffers(glfwFunc::glfwWindow);
	}

	/// Here all data must be destroyed + glfwTerminate
	void destroy()
	{
		m_model.deleteBuffers();
		m_cone.deleteBuffers();
		glfwDestroyWindow(glfwFunc::glfwWindow);
		glfwTerminate();
	}
};

int main(int argc, char** argv)
{
	glfwSetErrorCallback(glfwFunc::errorCB);
	if (!glfwInit())	exit(EXIT_FAILURE);
	glfwFunc::glfwWindow = glfwCreateWindow(glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT, glfwFunc::strNameWindow.c_str(), NULL, NULL);
	if (!glfwFunc::glfwWindow)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(glfwFunc::glfwWindow);
	if (!glfwFunc::initialize()) exit(EXIT_FAILURE);
	glfwFunc::resizeCB(glfwFunc::glfwWindow, glfwFunc::WINDOW_WIDTH, glfwFunc::WINDOW_HEIGHT);	//just the 1st time
	glfwSetKeyCallback(glfwFunc::glfwWindow, glfwFunc::keyboardCB);
	glfwSetWindowSizeCallback(glfwFunc::glfwWindow, glfwFunc::resizeCB);
	glfwSetMouseButtonCallback(glfwFunc::glfwWindow, glfwFunc::onMouseDown);
	glfwSetCursorPosCallback(glfwFunc::glfwWindow, glfwFunc::onMouseMove);
	// main loop!
	while (!glfwWindowShouldClose(glfwFunc::glfwWindow))
	{
		glfwFunc::draw();
		glfwPollEvents();	//or glfwWaitEvents()
	}
	glfwFunc::destroy();
	return EXIT_SUCCESS;
}
