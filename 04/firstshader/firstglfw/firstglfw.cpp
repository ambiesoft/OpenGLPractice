#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

static int CompileShader(GLenum type, const std::string& source)
{
    GLuint id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id,
        1, // count
        &src,
        nullptr);
    glCompileShader(id);

    // TODO: Error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        std::vector<char> message;
        message.resize(length + 1);
        glGetShaderInfoLog(id, length, &length, &message[0]);
        std::cerr << "Failed to compile" << 
            (type==GL_VERTEX_SHADER ? "vertex":"fragment") <<
            " shader!" << '\n';
        std::cerr << &message[0] << '\n';

        glDeleteShader(id);
        return 0;
    }
    return id;
}

static GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    GLuint program = glCreateProgram();
    GLuint vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // glewInit must be called after Context
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }
    // test glew functions
    unsigned int a;
    glGenBuffers(1, &a);
    std::cout << glGetString(GL_VERSION) << std::endl;

    // create buffer
    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f,
    };
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

    // Enable attribute0
    glEnableVertexAttribArray(0); // first attribute

    // Layout the buffer
    // set attribute0 , decides the composition of memory(positions)
    glVertexAttribPointer(
        0, // first attribute
        2, // count of point in a vertex(position)
        GL_FLOAT,
        GL_FALSE, // not normalize
        sizeof(float) * 2, // stride, byte size of 2 vertexes
        0  // offset
    );
       
    std::string vertexShader =
        "#version 330 core\n" // 'core' meang omit depricated functions
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "void main()\n"
        "{\n"
        "  gl_Position = position;\n"
        "}\n";
    std::string fragmentShader =
        "#version 330 core\n" // 'core' meang omit depricated functions
        "\n"
        "layout(location = 0) out vec4 color;"
        "void main()\n"
        "{\n"
        "  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
        "}\n";
    GLuint shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shader);

    glfwTerminate();
    return 0;
}