#ifndef OBJOBJECT_H
#define OBJOBJECT_H

#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <OpenGL/gl.h> // Remove this line in future projects
#else
#include <GL/glew.h>
#endif

#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class OBJObject
{
private:
std::vector<unsigned int> indices;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
glm::mat4 toWorld;
float angle;
float size;


public:
	OBJObject(const char* filepath);

	void parse(const char* filepath);
    glm::mat4 getToWorld();
    std::vector<glm::vec3> getVertices();
    std::vector<glm::vec3> getNormals();
    
	void draw();
    float getSize();
    void setSize(float a);
    void update();
    void spin(float deg);
    void translateX(float v);
    void translateY(float v);
    void translateZ(float v);
    void scale(float v);
    void rotate(float deg);
    void reset();
    
};

#endif
