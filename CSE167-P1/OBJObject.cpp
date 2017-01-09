#include "OBJObject.h"
using namespace std;


OBJObject::OBJObject(const char *filepath) 
{
	toWorld = glm::mat4(1.0f);
    this->size = 1.0;
    this->angle = 0.0f;
	parse(filepath);
}

float OBJObject::getSize() {
    return this->size;
}

void OBJObject::setSize(float a) {
    this->size = a;
}

glm::mat4 OBJObject::getToWorld() {
    return this->toWorld;
}

std::vector<glm::vec3> OBJObject::getVertices(){
    return this->vertices;
}

std::vector<glm::vec3> OBJObject::getNormals() {
    return this->normals;
}

void OBJObject::parse(const char *filepath) 
{
	//TODO parse the OBJ file
	// Populate the face indices, vertices, and normals vectors with the OBJ Object data

    FILE* fp;     // file pointer
    float x,y,z;  // vertex coordinates
    float r,g,b;  // vertex color
    float xn, yn, zn; // vertex normal coordinates
    int c1,c2;    // characters read from file
    
    unsigned int f1,f2,f3;
    
    float minX,minY,minZ = INFINITY;
    float maxX, maxY, maxZ = -INFINITY;
    
    float longestAxis;
    
    string line;
    bool loop = true;

    fp = fopen(filepath, "rb");  // make the file name configurable so you can load other files

    
    if (fp==NULL) { cerr << "error loading file" << endl; exit(-1); }  // just in case the file can't be found or is corrupt
    else {
      
        while ( loop ) {
            
            c1 = fgetc(fp);
            c2 = fgetc(fp);

            
            if( c1 == EOF ) {
                break;
            }
        
            if ( (c1 == 'v') && (c2==' ') ) {
                fscanf(fp, "%f %f %f %f %f %f", &x, &y, &z, &r, &g, &b);
                vertices.push_back(glm::vec3(x,y,z));
                
                if ( x < minX) {
                    minX = x;
                }
                
                if ( y < minY) {
                    minY= y;
                }
                
                if ( z < minZ) {
                    minZ = z;
                }
                
                if ( x > maxX) {
                    maxX = x;
                }
                
                if ( y > maxY) {
                    maxY = y;
                }
                
                if ( z > maxZ) {
                    maxZ = z;
                }
            }
            
            else if ( (c1 == 'v') && (c2=='n') ) {
                fgetc(fp); //space "vn" ""
                fscanf(fp, "%f %f %f", &xn, &yn, &zn);
                normals.push_back(glm::vec3(xn,yn,zn));
            }
            
            else if ( ( c1 == 'f') && (c2 == ' ')) {
    
                fscanf(fp, "%d//%d %d//%d %d//%d", &f1, &f1, &f2, &f2, &f3, &f3);
                indices.push_back(f1-1);
                indices.push_back(f2-1);
                indices.push_back(f3-1);
            }
            
            else {
                continue;
            }
        
        }
        
        float avgX = (maxX + minX)/2;
        float avgY = (maxY + minY)/2;
        float avgZ = (maxZ + minZ)/2;
        
        float xAxis = maxX - minX;
        float yAxis = maxY - minY;
        float zAxis = maxZ - minZ;
        
        glm::vec3 centerVector = glm::vec3(avgX, avgY, avgZ);
        
        if ( xAxis >= yAxis && xAxis >= zAxis ) {
            longestAxis = xAxis;
        }
        
        if ( yAxis >= xAxis && yAxis >= zAxis ) {
            longestAxis = yAxis;
        }
        
        if ( zAxis >= xAxis && zAxis >= yAxis ) {
            longestAxis = zAxis;
        }
        
        
        for ( unsigned int i = 0; i < vertices.size(); i++) {
            vertices[i] = (vertices[i] - centerVector );
            vertices[i] = vertices[i]/ longestAxis;
          
        }
        

    }
    

    fclose(fp);   // make sure you don't forget to close the file when done
}

void OBJObject::draw()
{
	glMatrixMode(GL_MODELVIEW);

	// Push a save state onto the matrix stack, and multiply in the toWorld matrix
	glPushMatrix();
	glMultMatrixf(&(toWorld[0][0]));

    glPointSize(size);
    
	glBegin(GL_POINTS);
    // Set the color of the cube (R, G, B). Change it to your favorite color!
    

	// Loop through all the vertices of this OBJ Object and render them
	for (unsigned int i = 0; i < vertices.size(); ++i) 
	{
        glColor3f( (glm::normalize(normals[i].x) + 1.0f) / 2, (1.0f + glm::normalize(normals[i].y))/2, (1.0f +glm::normalize(normals[i].z) )/2 );
		glVertex3f(vertices[i].x, vertices[i].y, vertices[i].z);
       
	}
    
	glEnd();

	// Pop the save state off the matrix stack
	// This will undo the multiply we did earlier
	glPopMatrix();
}

void OBJObject::update()
{
    spin(340.0f);
}

void OBJObject::spin(float deg)
{
    this->angle += deg;
    if (this->angle > 360.0f || this->angle < -360.0f) this->angle = 0.0f;
    // This creates the matrix to rotate the cube
    this->toWorld = this->toWorld * glm::rotate(glm::mat4(1.0f), this->angle / 180.0f * glm::pi<float>(), glm::vec3(0.0f, 1.0f, 0.0f));
}

void OBJObject::translateX(float v) {
     this->toWorld = ( glm::translate ( glm::mat4(1.0f), glm::vec3(v,0.0f, 0.0f) ) ) * this->toWorld;
}

void OBJObject::translateY(float v) {
    this->toWorld = ( glm::translate (glm::mat4(1.0f), glm::vec3(0.0f,v,0.0f) ) ) * this->toWorld;
}

void OBJObject::translateZ(float v) {
    this->toWorld = ( glm::translate ( glm::mat4(1.0f), glm::vec3(0.0f,0.0f,v) ) ) * this->toWorld;
}

void OBJObject::scale(float v) {
    this->toWorld = this->toWorld * glm::scale (glm::mat4(1.0f), glm::vec3(v) );
}

void OBJObject::rotate(float deg) {
    this->toWorld =  glm::rotate(glm::mat4(1.0f), deg/180.0f * glm::pi<float>(), glm::vec3(0.0f, 0.0f, 1.0f)) * toWorld;
}

void OBJObject::reset() {
    toWorld = glm::mat4(1.0f);
    this->size = 1.0;
}







