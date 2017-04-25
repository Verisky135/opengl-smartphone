// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI (3.141592653589793)

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

#include "common/shader.hpp"
#include "common/controls.hpp"


const float E  = 4.0f / 5.0f;
const float S  = 9.0f / 5.0f;
const float A  = 1.0f / 5.0f;
const float SATU  = 1.0f;
const float NOL   = 0.0f;

const float AKAR13 = sqrt(0.333333333);
const float AKAR2 = sqrt(2);
const float AKAR23 = sqrt(0.666666666);
const float AKAR3 = sqrt(3);

int main(){
//    printf("%f\n", sqrt(0.33333));
    // Initialise GLFW
    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Get mouse position

    // Open a window and create its OpenGL context
    window = glfwCreateWindow( 1024, 768, "3D Box Color Pallete", NULL, NULL);
    if( window == NULL ){
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

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
    GLuint programID = LoadShaders( "TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader" );

    // Get a handle for our "MVP" uniform
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

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

    // ini kode warna gan, jangan bingung RGB
    // 1,1,1 means R: 255, G: 255, B: 255
    GLfloat g_color_buffer_data[] = {
        SATU, SATU, SATU, // A I
        SATU, NOL, SATU, // B
        SATU, NOL, NOL, // C

        SATU, SATU, SATU, // A
        SATU, SATU, NOL, // D
        SATU, NOL, NOL, // C

        SATU, SATU, SATU, // A II
        SATU, NOL, SATU, // B
        NOL, NOL, SATU, // F

        SATU, SATU, SATU, // A
        NOL, SATU, SATU, // E
        NOL, NOL, SATU, // F

        SATU, SATU, SATU, // A III
        SATU, SATU, NOL, // D
        NOL, SATU, NOL, // H

        SATU, SATU, SATU, // A
        NOL, SATU, SATU, // E
        NOL, SATU, NOL, // H

        NOL, NOL, NOL, // G IV
        SATU, NOL, NOL, // C
        SATU, SATU, NOL, // D

        NOL, NOL, NOL, // G
        NOL, SATU, NOL, // H
        SATU, SATU, NOL, // D

        NOL, NOL, NOL, // G V
        NOL, NOL, SATU, // F
        NOL, SATU, SATU, // E

        NOL, NOL, NOL, // G
        NOL, SATU, NOL, // H
        NOL, SATU, SATU, // E

        NOL, NOL, NOL, // G VI
        SATU, NOL, NOL, // C
        SATU, NOL, SATU, // B

        NOL, NOL, NOL, // G
        NOL, NOL, SATU, // F
        SATU, NOL, SATU, // B

    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    // Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);
    // Camera matrix
    glm::mat4 View       = glm::lookAt(
                                glm::vec3(7,0,0), // Camera is at (4,3,-3), in World Space
                                glm::vec3(0,0,0), // and looks at the origin
                                glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
                           );

    float moveCubeX = 0.0f;

    float sudut = PI / 2;

    float xRotationMatrix[16] = {cos(sudut), 0.0f, sin(sudut), 0.0f,
                                 0.0f, 1.0f, 0.0f, 0.0f,
                                 -sin(sudut), 0.0f, cos(sudut), 0.0f,
                                 0.0f, 0.0f, 0.0f, 1.0f};

//    float xRotationMatrix[16] = {1.0f, 0.0f, 0.0f, 0.0f,
//                                 0.0f, cos(PI/2), sin(PI/2), 0.0f,
//                                 0.0f, -sin(PI/2), cos(PI/2), 0.0f,
//                                 0.0f, 0.0f, 0.0f, 1.0f};

    glm::mat4 rotx = glm::make_mat4(xRotationMatrix);

    do{


        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use our shader
        glUseProgram(programID);

        float zRotationMatrix[16] = {cos(-moveCubeX), -sin(-moveCubeX), 0.0f, 0.0f,
                                     sin(-moveCubeX), cos(-moveCubeX), 0.0f, 0.0f,
                                     0.0f, 0.0f, 1.0f, 0.0f,
                                     0.0f, 0.0f, 0.0f, 1.0f};

        glm::mat4 rotz = glm::make_mat4(zRotationMatrix);


        // Model matrix : an identity matrix (model will be at the origin)
        glm::mat4 Model      = glm::mat4(1.0f);
        // Our ModelViewProjection : multiplication of our 3 matrices
        glm::mat4 MVP        = Projection * View * Model * rotz; // Remember, matrix multiplication is the other way around

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

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

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 indices starting at 0 -> 12 triangles

        glm::mat4 base = Projection * View * Model;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &base[0][0]);
        glDrawArrays(GL_TRIANGLES, 12 * 3, (28 - 12) * 3);

        base = Projection * View * Model * glm::translate(glm::mat4(1.f), glm::vec3(0.0f, 0.0f, -4 * AKAR3 + 2 * AKAR3));;
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &base[0][0]);
        glDrawArrays(GL_TRIANGLES, 28 * 3, (30 - 28) * 3);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

        moveCubeX += 0.07f;

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0 );

    // Cleanup VBO and shader
    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}

