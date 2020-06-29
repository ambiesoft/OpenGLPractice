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
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

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
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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
        //float positions[] = {
        //    -0.5f, -0.5f, 0.0f, 0.0f,
        //     0.5f, -0.5f, 1.0f, 0.0f,
        //     0.5f,  0.5f, 1.0f, 1.0f,

        //     // 0.5f,  0.5f, (duplicated)
        //    -0.5f,  0.5f, 0.0f, 1.0f,
        //    // -0.5f, -0.5f, (duplicated)
        //};
        float positions[] = {
             100.0f,   100.0f, 0.0f, 0.0f,
             200.0f,   100.0f, 1.0f, 0.0f,
             200.0f,   200.0f, 1.0f, 1.0f,
             100.0f,   200.0f,0.0f, 1.0f,
        };
        unsigned int indices[] = {
            0,1,2,
            2,3,0,
        };
        assert(sizeof(positions) == (_countof(positions) * sizeof(float)));

        CALLGL(glDisable(GL_BLEND));
        CALLGL(glBlendFunc(GL_ONE, GL_ZERO));

        // CALLGL(glEnable(GL_BLEND));
        // CALLGL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


        // Vertex Array Object
        VertexArray va;
        VertexBuffer vb(positions, _countof(positions) * sizeof(float));
        

        VertexBufferLayout layout;
        layout.Push<float>(2); // a vertex has 2 points
        layout.Push<float>(2); // a vertex has 2 points
        va.AddBuffer(vb, layout);



        // index buffer object
        IndexBuffer ib(indices, _countof(indices));

        // create something called an orthographic matrix
        // need to specify something that adheres to that four by three aspect ratio
        // glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));

        // 200 left, 200 up
        glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

        glm::mat4 mvp = proj * view * model;



        // Shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/ChernoLogo.png");
        int slot = 0;
        texture.Bind(slot);
        shader.SetUniform1i("u_Texture", slot);

        // Unbind all
        va.Unbind();
        vb.Unbind();
        va.Unbind();
        shader.Unbind();

        Renderer renderer;

        // Loop until the user closes the window
        float r = 0.0f;
        float increment = 0.5f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            

            // Bind
            shader.Bind();
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            renderer.Draw(va, ib, shader);

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
