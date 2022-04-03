#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include <iostream>
#include <string>
#include "WindowObject.h"
#include "Renderer.h"
#include "Texture.h"




void processInput(GLFWwindow* window);

int w_Width = 640;
int w_Height = 480;

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(w_Width, w_Height, "Hello World", NULL, NULL);
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

    /* Handling Window Resize Event */
    WindowObject windowObject(w_Width, w_Height);
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        glfwGetWindowSize(window, &w_Width, &w_Height);
    });

    /* Print out the current Version */
    std::cout << glGetString(GL_VERSION) << std::endl;


    /* Enalbe Blending */
    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    {
        /* Vertices Data */
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

        /* Init VertexArray, VertexBuffer, VertexBufferLayout */
        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;

        /* Init Layout & Attach (VertexBuffer, Layout) to VertexArray */
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        /* IndexBuffer, Shader, Texture and Renderer */
        IndexBuffer ib(indices, 6);
        Shader shader("resources/shaders/Basic.shader");
        Texture texture("resources/texture/ramadan.png");
        Renderer renderer;

        /* Unbind Everything, not really required !*/
        shader.unbind(); texture.unbind();
        va.unbind(); vb.unbind(); ib.unbind();

        int i = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* handle Resizing: udpate projection matrix if window been resized */
            if (windowObject.getHeight() != w_Height || windowObject.getWidth() != w_Width)
                windowObject.update(w_Width, w_Height);

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

            shader.setUniformMat4f("u_MVP", windowObject.getProjMat());
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