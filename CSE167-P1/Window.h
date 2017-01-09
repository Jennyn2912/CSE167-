#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <iostream>

#ifdef __APPLE__
// If modern OpenGL replace gl.h with gl3.h
#include <OpenGL/gl.h>
#include <OpenGL/glext.h>
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include "Cube.h"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Window
{
public:
	static int width;
	static int height;
 
    
	static void initialize_objects();
	static void clean_up();
	static GLFWwindow* create_window(int width, int height);
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void idle_callback();
	static void display_callback(GLFWwindow*);
    
    static void rasterize();
    static void Rasterizer_clearBuffer();
    static glm::mat4 Rasterizer_generateD();
    static void Rasterizer_resizeCallback (GLFWwindow* window, int width, int height);
    static void Rasterizer_drawPoint(int x, int y, float r, float g, float b, int size);
    static void Rasterizer_displayCallback(GLFWwindow* window);
    
    
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void character_callback(GLFWwindow* window, unsigned int codepoint);
    static void charmods_callback( GLFWwindow *window, unsigned int codepoint, int mods );
};

#endif
