#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "Renderer.h"
#include "Texture.h"


void processInput(GLFWwindow* window);


int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        GlCall(glfwTerminate());
        return -1;
    }

    /* Make the window's context current */
    // some how do not do GlCall here !
    glfwMakeContextCurrent(window);
    // do sync rendering loop to the refresh rate of monitor
    // Intervel param: Is the minimum number of screen updates 
    // to wait for until the buffers are swapped
    glfwSwapInterval(1); 

    /* Initialize GLEW Library */
    if (glewInit() != GLEW_OK) {
        std::cout << "Error: Initializing GLEW" << std::endl;
        return -1;
    }
    /* Print out the current Version */
    std::cout << glGetString(GL_VERSION) << std::endl;

    {
        float positions[24] = {
            -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 1.0f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        GlCall(glEnable(GL_BLEND));
        GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        // Index Buffer Object
        IndexBuffer ib(indices, 6);

        Shader shader("resources/shaders/Basic.shader");

        Texture texture("resources/texture/ramadan.png");

        Renderer renderer;
        // Unbind EveryThing
        shader.unbind();
        texture.unbind();
        va.unbind();
        vb.unbind();
        ib.unbind();

        int i = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            if (i > 180)
                i = 0;
            i += 3;
            /* Handel Esc Key Down */
            processInput(window);
            /*
            * Render here
            */
            renderer.clear(0.2f, 0.3f, 0.3f, 1.0f);

            texture.bind();
            shader.bind();
            float color = sin(i * 3.14 / 180);
            shader.setUniform4f("u_Color", color, color, color, 1.0f);
            shader.setUniform1i("u_Texture", 0);

            /* Triangle :) */
            renderer.draw(va, ib, shader);
           

            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }        
    }
    glfwTerminate(); // it returns error, !
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}