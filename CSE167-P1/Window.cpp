#include "Window.h"
#include "OBJObject.h"

const char* window_title = "GLFW Starter Project";
//Cube cube(5.0f);

const glm::vec3 eye = glm::vec3(0,0,20);
const glm::vec3 direction = glm::vec3(0,0,0);
const glm::vec3 up = glm::vec3(0,1,0);

static float x_start = 0.0;
static float y_start = 0.0;

bool modeOpenGL = true;

OBJObject* bunny;
OBJObject* bear;
OBJObject* dragon;
static float* pixels;

OBJObject* all;
bool keyPressed = false;

int Window::width;
int Window::height;

struct Color    // generic color class
{
    float r, g, b;  // red, green, blue
};


void Window::initialize_objects()
{
    bunny = new OBJObject ("bunny.obj");
    bear = new OBJObject ("bear.obj");
    dragon = new OBJObject ("dragon.obj");
    pixels = new float [Window::width * Window::height * 3];
}

void Window::clean_up()
{
    delete bunny;
    delete dragon;
    delete bear;
    delete[] pixels;
}

GLFWwindow* Window::create_window(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return NULL;
	}
    
	// 4x antialiasing
	glfwWindowHint(GLFW_SAMPLES, 4);


	// Create the GLFW window
	GLFWwindow* window = glfwCreateWindow(width, height, window_title, NULL, NULL);

	// Check if the window could not be created
	if (!window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window
	glfwMakeContextCurrent(window);

	// Set swap interval to 1
	glfwSwapInterval(1);

	// Call the resize callback to make sure things get drawn immediately
	//Window::resize_callback(window, width, height);
    int fwidth, fheight;
    
    glfwGetFramebufferSize(window, &fwidth, &fheight);
    
  
    Window::resize_callback(window, fwidth, fheight);

    
	return window;
}

void Window::resize_callback(GLFWwindow* window, int width, int height)
{
    if ( modeOpenGL == true ) {
        
        Window::width = width;
        Window::height = height;
        // Set the viewport size
        glViewport(0, 0, width, height);
        // Set the matrix mode to GL_PROJECTION to determine the proper camera properties
        glMatrixMode(GL_PROJECTION);
        // Load the identity matrix
        glLoadIdentity();
        // Set the perspective of the projection viewing frustum
        //gluPerspective(60.0, double(width) / (double)height, 1.0, 1000.0);
        glm::mat4 m = glm::perspective(60.0 / 180.0 * glm::pi<double>(), double(width) / (double)height, 1.0, 1000.0);
        glLoadMatrixf(&(m[0][0]));
        // Move camera back 20 units so that it looks at the origin (or else it's in the origin)
        glTranslatef(0, 0, -20);
    }
    
    else {
        Window::Rasterizer_resizeCallback(window, width, height);
    }
}


void Window::Rasterizer_resizeCallback (GLFWwindow* window, int width, int height)
{
    delete[] pixels;
    pixels = new float[width * height * 3];
    Window::width = width;
    Window::height = height;
}



void Window::idle_callback()
{
	// Perform any updates as necessary. Here, we will spin the cube slightly.
	
    //cube.update();
    if(keyPressed) {
        all->update();
    }
 
}


void Window::display_callback(GLFWwindow* window)
{
    if ( modeOpenGL == true) {
        // Clear the color and depth buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // Set the matrix mode to GL_MODELVIEW
        glMatrixMode(GL_MODELVIEW);
        // Load the identity matrix
        glLoadIdentity();
        
        // Render objects
        
        if (keyPressed) {
            all->draw();
        }
        
        // Gets events, including input such as keyboard and mouse or window resizing
        glfwPollEvents();
        // Swap buffers
        glfwSwapBuffers(window);
    }
    
    if ( modeOpenGL == false) {
        Rasterizer_displayCallback(window);
    }
}

void Window::rasterize() {
    glm::mat4 M = all->getToWorld();
    glm::mat4 C_inverse = glm::lookAt(eye, direction, up);
    glm::mat4 P = glm::perspective(glm::radians(60.0f),(float) Window::width / (float) Window::height,1.0f, 1000.0f);
    glm::mat4 D = Rasterizer_generateD();
    
    std::vector<glm::vec3> objVertices = all->getVertices();
    std::vector<glm::vec3> objNormals  = all->getNormals();
    
    for (unsigned int i = 0; i < objVertices.size(); ++i) {
        
        glm::vec4 hp = glm::vec4(objVertices[i].x, objVertices[i].y, objVertices[i].z, 1);
        
        glm::vec4 p_prime = D*P*C_inverse*M*hp;
        
        float x_prime = (float) p_prime.x / p_prime.w ;
        float y_prime = (float) p_prime.y / p_prime.w ;
        Rasterizer_drawPoint(x_prime, y_prime,(glm::normalize(objNormals[i].x) + 1.0f) / 2, (1.0f + glm::normalize(objNormals[i].y))/2, (1.0f +glm::normalize(objNormals[i].z) )/2, all->getSize());
    }
    
}

void Window::Rasterizer_clearBuffer() {
    Color clearColor = {0.878431, 1.0, 1.0 };   // clear color: black
    std::cout << Window::width << std::endl;
    for (int i = 0; i<Window::width*Window::height; ++i)
    {
        pixels[i * 3] = clearColor.r;
        pixels[i * 3 + 1] = clearColor.g;
        pixels[i * 3 + 2] = clearColor.b;
    }
}

glm::mat4 Window::Rasterizer_generateD(){
    glm::mat4 D = glm::mat4(1.0);
    
    D[0] = glm::vec4( float (Window::width - x_start)/2.0 , 0.0, 0.0 , 0.0 );
    D[1] = glm::vec4( 0.0, float ( (Window::height - y_start) /2.0 ), 0.0 , 0.0);
    D[2] = glm::vec4( 0.0, 0.0, (1.0/2.0), 0.0);
    D[3] = glm::vec4( float ( (Window::width+x_start)/2.0), float( (Window::height + y_start)/2.0 ), (1.0/2.0), 1.0 );
    
    return D;
}


void Window::Rasterizer_drawPoint(int x, int y, float r, float g, float b, int size)
{
   int offset = y * Window::width * 3 + x * 3;
    if ( x >= 0 && x < Window::width && y >= 0 && y < Window::height && offset < Window::width * Window::height * 3  && offset >= 0) {
    
       pixels[offset] = r;
       pixels[offset + 1] = g;
       pixels[offset + 2] = b;
       
       for ( int i = 1; i < size; i++) {
           offset = offset + 3;
           if ( offset < Window::width * Window::height * 3 - 2  && offset >= 0 ) {
               pixels[offset] = r;
               pixels[offset + 1] = g;
               pixels[offset + 2] = b;
           }
       }
   }
}

void Window::Rasterizer_displayCallback(GLFWwindow* window)
{
    Rasterizer_resizeCallback(window, Window::width, Window::height);
    Rasterizer_clearBuffer();
    
    if(keyPressed) {
        rasterize();
    }
    
    // glDrawPixels writes a block of pixels to the framebuffer
    glDrawPixels(Window::width, Window::height, GL_RGB, GL_FLOAT, pixels);
    
    // Gets events, including input such as keyboard and mouse or window resizing
    glfwPollEvents();
    // Swap buffers
    glfwSwapBuffers(window);
}


void Window::character_callback(GLFWwindow* window, unsigned int codepoint)
{
    //std::cout << codepoint << std::endl;
}

void Window::charmods_callback( GLFWwindow *window, unsigned int codepoint, int mods ) {
    
    if ( mods == 1 )
    {

    }
}

void Window::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
    // Check for a key press
	if (action == GLFW_PRESS)
	{
		// Check if escape was pressed
		if (key == GLFW_KEY_ESCAPE)
		{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
        
        else if ( key == GLFW_KEY_F1) {

            all = bunny;
            keyPressed = true;
        }
        
        else if ( key == GLFW_KEY_F2) {
            all = bear;
            keyPressed = true;
        }
        
        else if ( key == GLFW_KEY_F3) {
            all = dragon;
            keyPressed = true;
        }
        
        else if (key == GLFW_KEY_P && mods == 1) {
                all->setSize ( float (all->getSize()) + 1.0);
        }
        
        else if ( key == GLFW_KEY_P && mods == 0 ) {
                all->setSize ( float (all->getSize()) - 1.0 );
        }
        
        else if ( key == GLFW_KEY_X && mods == 1 ) {
            all->translateX(0.5f);
        }
    
        else if ( key == GLFW_KEY_X && mods == 0) {
            all->translateX(-0.5f);
        }
        
        else if ( key == GLFW_KEY_Y && mods == GLFW_MOD_SHIFT ) {
            all->translateY(0.5f);
        }
        
        else if ( key == GLFW_KEY_Y && mods == 0) {
            all->translateY(-0.5f);
        }
        
        else if ( key == GLFW_KEY_Z && mods == 1 ) {
            all->translateZ(0.5f);
        }
        
        else if ( key == GLFW_KEY_Z && mods == 0) {
            all->translateZ(-0.5f);
        }
        
        else if (key == GLFW_KEY_S && mods == GLFW_MOD_SHIFT) {
            all->scale(2.0f);
        }
        
        else if (key == GLFW_KEY_S && mods == 0) {
            all->scale(0.5f);
        }
        
        else if (key == GLFW_KEY_O && mods == GLFW_MOD_SHIFT) {
            all->rotate(-30.0f);
        }
        
        else if (key == GLFW_KEY_O && mods == 0) {
            all->rotate(30.0f);
        }
        
        else if (key == GLFW_KEY_R) {
            all->reset();
        }
        
        else if (key == GLFW_KEY_M) {
            if (modeOpenGL == true) { modeOpenGL = false;}
            else {
                modeOpenGL = true;
            }
        }
        
        else {
            
        }

    }
}
