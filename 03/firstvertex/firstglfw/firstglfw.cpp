#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

    // set attribute0 , decides the composition of memory(positions)
    glVertexAttribPointer(
        0, // first attribute
        2, // count of point in a vertex(position)
        GL_FLOAT,
        GL_FALSE, // not normalize
        sizeof(float) * 2, // stride, byte size of 2 vertexes
        0  // offset
    );
       
    glBindBuffer(GL_ARRAY_BUFFER, 0);

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

    glfwTerminate();
    return 0;
}