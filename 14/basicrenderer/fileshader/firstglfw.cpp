#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    // glewInit must be called after Context
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    
    // test glew functions
    {
        std::cout << glGetString(GL_VERSION) << std::endl;
    }

    {
        // create buffer
        float positions[] = {
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,

             // 0.5f,  0.5f, (duplicated)
            -0.5f,  0.5f,
            // -0.5f, -0.5f, (duplicated)
        };
        unsigned int indices[] = {
            0,1,2,
            2,3,0,
        };
        assert(sizeof(positions) == (_countof(positions) * sizeof(float)));

        // Vertex Array Object
        VertexArray va;
        VertexBuffer vb(positions, _countof(positions) * sizeof(float));
        

        VertexBufferLayout layout;
        layout.Push<float>(2); // a vertex has 2 points
        va.AddBuffer(vb, layout);



        // index buffer object
        IndexBuffer ib(indices, _countof(indices));


        // Shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

 

        // Unbind all
        va.Unbind();
        vb.Unbind();
        va.Unbind();
        shader.Unbind();



        // Loop until the user closes the window
        float r = 0.0f;
        float increment = 0.5f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            CALLGL(glClear(GL_COLOR_BUFFER_BIT));

            // Bind
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            va.Bind();
            ib.Bind(); // CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

            CALLGL(glDrawElements(GL_TRIANGLES,
                6, // count of index
                GL_UNSIGNED_INT,
                nullptr));

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            CALLGL(glfwSwapBuffers(window));

            /* Poll for and process events */
            CALLGL(glfwPollEvents());
        }
    }
    glfwTerminate();
    return 0;
}
