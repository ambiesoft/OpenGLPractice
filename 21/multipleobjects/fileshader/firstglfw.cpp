#include <cassert>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw_gl3.h>

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
             -50.0f,   -50.0f, 0.0f, 0.0f,
              50.0f,   -50.0f, 1.0f, 0.0f,
              50.0f,    50.0f, 1.0f, 1.0f,
             -50.0f,    50.0f, 0.0f, 1.0f,
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

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));


        // Shader
        Shader shader("res/shaders/Basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);


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

        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        bool show_demo_window = true;
        bool show_another_window = false;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        glm::vec3 translationA(200.0f, 200.0f, 0.0f);
        glm::vec3 translationB(400.0f, 200.0f, 0.0f);

        // Loop until the user closes the window
        float r = 0.0f;
        float increment = 0.5f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();
            
            ImGui_ImplGlfwGL3_NewFrame();


            {
                // 200 left, 200 up
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }
            {
                // 400 left, 200 up
                glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
                glm::mat4 mvp = proj * view * model;
                shader.Bind();
                shader.SetUniformMat4f("u_MVP", mvp);
                renderer.Draw(va, ib, shader);
            }

            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0.0f)
                increment = 0.05f;

            r += increment;

            {
                static float f = 0.0f;
                static int counter = 0;
                ImGui::Text("Hello, world!");                           // Display some text (you can use a format string too)
                ImGui::SliderFloat3("Translation A", &translationA.x, 0.0, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
                ImGui::SliderFloat3("Translation B", &translationB.x, 0.0, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f    
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our windows open/close state
                ImGui::Checkbox("Another Window", &show_another_window);

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (NB: most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());


            /* Swap front and back buffers */
            CALLGL(glfwSwapBuffers(window));

            /* Poll for and process events */
            CALLGL(glfwPollEvents());
        }
    } 
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}
