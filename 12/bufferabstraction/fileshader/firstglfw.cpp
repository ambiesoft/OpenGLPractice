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

struct ShaderProgramSources
{
    std::string VertexSource;
    std::string FragmentSource;
};

static ShaderProgramSources ParseShader(const std::string& filepath)
{
    std::ifstream stream(filepath);

    enum class ShaderType
    {
        NONE = -1,
        VERTEX,
        FRAGMENT,
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;
    while (getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
            {
                type = ShaderType::VERTEX;
            }
            else if (line.find("fragment") != std::string::npos)
            {
                type = ShaderType::FRAGMENT;
            }
        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

static int CompileShader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    CALLGL(glShaderSource(id,
        1, // count
        &src,
        nullptr));
    CALLGL(glCompileShader(id));

    // TODO: Error handling
    int result;
    CALLGL(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result != GL_TRUE)
    {
        int length;
        CALLGL(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        std::vector<char> message;
        message.resize(length + 1);
        CALLGL(glGetShaderInfoLog(id, length, &length, &message[0]));
        std::cerr << "Failed to compile" << 
            (type==GL_VERTEX_SHADER ? "vertex":"fragment") <<
            " shader!" << '\n';
        std::cerr << &message[0] << '\n';

        CALLGL(glDeleteShader(id));
        return 0;
    }
    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    CALLGL(glAttachShader(program, vs));
    CALLGL(glAttachShader(program, fs));

    CALLGL(glLinkProgram(program));
    CALLGL(glValidateProgram(program));

    CALLGL(glDeleteShader(vs));
    CALLGL(glDeleteShader(fs));

    return program;
}

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

        ShaderProgramSources source = ParseShader("res/shaders/Basic.shader");
        //std::cout << "VERTEX >>>>" << '\n';
        //std::cout << source.VertexSource << '\n';
        //std::cout << "<<<< VERTEX" << '\n';

        //std::cout << "FRAGMENT >>>>" << '\n';
        //std::cout << source.FragmentSource << '\n';
        //std::cout << "<<<< FRAGMENT" << '\n';

        GLuint shader = CreateShader(source.VertexSource, source.FragmentSource);
        CALLGL(glUseProgram(shader));

        // Uniform
        int location = glGetUniformLocation(shader, "u_Color");
        ASSERT_GL(location != -1);
        CALLGL(glUniform4f(location, 0.8f, 0.3f, 0.8f, 1.0f));


        // Unbind all
        CALLGL(glBindVertexArray(0));
        CALLGL(glUseProgram(0));
        CALLGL(glBindBuffer(GL_ARRAY_BUFFER, 0));
        CALLGL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));


        // Loop until the user closes the window
        float r = 0.0f;
        float increment = 0.5f;
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            CALLGL(glClear(GL_COLOR_BUFFER_BIT));

            // Bind
            CALLGL(glUseProgram(shader));
            CALLGL(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

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

        CALLGL(glDeleteProgram(shader));
    }
    glfwTerminate();
    return 0;
}
