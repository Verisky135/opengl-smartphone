// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/texture.hpp"

const float E  = 4.0f / 5.0f;
const float S  = 9.0f / 5.0f;
const float A  = 0.5f / 5.0f;

int main( void )
{
	// Initialise GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow( 1024, 768, "Tutorial 05 - Textured Cube", NULL, NULL);
	if( window == NULL ){
		fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 

	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "TextureFragmentShader.fragmentshader" );

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	glm::mat4 View       = glm::lookAt(
								glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
								glm::vec3(0,0,0), // and looks at the origin
								glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
						   );
	// Model matrix : an identity matrix (model will be at the origin)
	glm::mat4 Model      = glm::mat4(1.0f);

	// Load the texture using any two methods
	//GLuint Texture = loadBMP_custom("uvtemplate.bmp");
	GLuint Texture = loadDDS("last.dds");
	
	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

	// Our vertices. Tree consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
	// A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices

    // ini posisi titik titiknya di ruang 3D
    static GLfloat g_vertex_buffer_data[] = {
        +E, +A, -S, // A 1 I
        +E, -A, -S, // B 2
        +E, -A, +S, // C 3

        +E, +A, -S, // A 4
        +E, +A, +S, // D 5
        +E, -A, +S, // C 6

        +E, +A, -S, // A 7 II
        +E, -A, -S, // B 8
        -E, -A, -S, // F 9

        +E, +A, -S, // A 10
        -E, +A, -S, // E 11
        -E, -A, -S, // F 12

        +E, +A, -S, // A 13 III
        +E, +A, +S, // D 14
        -E, +A, +S, // H 15

        +E, +A, -S, // A 16
        -E, +A, -S, // E 17
        -E, +A, +S, // H 18

        -E, -A, +S, // G 19 IV
        +E, -A, +S, // C 20
        +E, +A, +S, // D 21

        -E, -A, +S, // G 22
        -E, +A, +S, // H 23
        +E, +A, +S, // D 24

        -E, -A, +S, // G 25 V
        -E, -A, -S, // F 26
        -E, +A, -S, // E 27

        -E, -A, +S, // G 28
        -E, +A, +S, // H 29
        -E, +A, -S, // E 30

        -E, -A, +S, // G 31 VI
        +E, -A, +S, // C 32
        +E, -A, -S, // B 33

        -E, -A, +S, // G 34
        -E, -A, -S, // F 35
        +E, -A, -S, // B 36
    };


	// Two UV coordinatesfor each vertex. They were created withe Blender.
	static const GLfloat g_uv_buffer_data[] = { 


		0.95f, 1.0f, // I
		1.0f, 1.0f,
		1.0f, 0.0f, 

		0.95f, 1.0f,
		0.96f, 0.0f,
		1.0f, 0.0f,

		0.0f, 0.0f, // II
		0.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 0.0f, 
		0.0f, 0.0f,
		0.0f, 0.0f,

		0.95f, 1.0f, // III
		0.95f, 0.0f,
		0.475f, 0.0f, 

		0.95f, 1.0f,
		0.475f, 1.0f,
		0.475f, 0.0f,

		0.0f, 0.0f, // IV
		0.0f, 0.0f,
		0.0f, 0.0f,

		0.0f, 0.0f,
		0.0f, 0.0f,
		0.0f, 0.0f,

		0.95f, 0.0f, 
		0.95f, 1.0f, // V
		1.0f, 1.0f,

		0.95f, 0.0f, 
		1.0f, 0.0f, // V
		1.0f, 1.0f,
		
		0.475f, 0.0f, // VI
		0.0f, 0.0f,
		0.0f, 1.0f,

		0.475f, 0.0f,
		0.475f, 1.0f,
		0.0f, 1.0f,
	};


	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

    float moveCubeX = 0.0f;
    float moveCubeY = 0.25f;
    float scaledelta = 0.0f;
    float scale = 1.55f;
    float scale2 = 0.0f;

	do{


        float zRotationMatrix[16] = {cos(-moveCubeX), -sin(-moveCubeX), 0.0f, 0.0f,
                                     sin(-moveCubeX), cos(-moveCubeX), 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};
        float yRotationMatrix[16] = {cos(moveCubeY), 0.0f, sin(moveCubeY), 0.0f,
                                     0.0f, 1.0f, 0.0f, 0.0f,
                                     -sin(moveCubeY), 0.0f, cos(moveCubeY), 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};
        float scaleme[16] = {scale2, 0.0f, 0.0f, 0.0f,
                                     0.0f, scale2, 0.0f, 0.0f,
                                     0.0f, 0.0f, scale2, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};
        glm::mat4 roty = glm::make_mat4(yRotationMatrix);
        glm::mat4 rotz = glm::make_mat4(zRotationMatrix);
        glm::mat4 scc = glm::make_mat4(scaleme);

		// Our ModelViewProjection : multiplication of our 3 matrices
		glm::mat4 MVP        = Projection * View * Model * rotz * roty * scc; // Remember, matrix multiplication is the other way around

		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Use our shader
		glUseProgram(programID);

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Draw the triangle !
		glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

        moveCubeX += 0.01f;
        moveCubeY += 0.005f;
        if(scale < 2.0f && scale > 1.5f)
        	scaledelta = -0.01f;

        if(scale > -2.0f && scale < -1.5f)
        	scaledelta = +0.01f;

        if(scale2 < 0.5f && scale2 > -0.5f)
        	scale2 = 0.5;
        else
        	scale2 = scale;

        
        scale += scaledelta;

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &TextureID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

