#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <iostream>
#include <string>
#include "WindowObject.h"
#include "Camera.h"
#include "Renderer.h"
#include "Texture.h"




void processInput(GLFWwindow* window, Camera* camera);

int w_Width = 1280;
int w_Height = 720;

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

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

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

    /* Enable depth testing */
    GlCall(glEnable(GL_DEPTH_TEST));
    /// TODO: move to renderer class

    {
        /* Vertices Data */
        float positions[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.5f, 0.0f, 1.0f,

            0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f,

            0.5f, -0.5f,  0.5f, 0.0f, 0.0f, // 9
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, // 10
            -0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, 1.0f,

            0.5f,  0.5f,  0.5f, 0.0f, 0.0f, // 9
            0.5f,  0.5f, -0.5f, 1.0f, 0.0f, // 10
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f,  0.5f, 0.0f, 1.0f,

        };
        unsigned int indices[] = {
            0, 1, 2, 2, 3, 0,
            4, 5, 6, 6, 7, 4,
            8, 9, 10, 10, 11, 8,
            12, 13, 14, 14, 15, 12,
            16, 17, 18, 18, 19, 16,
            20, 21, 22, 22, 23, 20,
        };

        /* Init VertexArray, VertexBuffer, VertexBufferLayout */
        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;

        /* Init Layout & Attach (VertexBuffer, Layout) to VertexArray */
        layout.Push<float>(3);
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        /* IndexBuffer, Shader, Texture and Renderer */
        IndexBuffer ib(indices, sizeof(indices)/ sizeof(unsigned int));
        Shader shader("resources/shaders/Basic.shader");
        Texture texture("resources/texture/1.png");
        Renderer renderer;

        /* Unbind Everything, not really required !*/
        shader.unbind(); texture.unbind();
        va.unbind(); vb.unbind(); ib.unbind();

        /* Model Matrix */
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        /* View Matrix (Camera) */
        glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, -1.0f);
        Camera camera(window, cameraPos, cameraTarget);

        /* Projection Matrix */
        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), windowObject.getAspectRatio(), 0.1f, 100.0f);

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f); // to be deleted

        glm::vec3 cubePositions[] = {
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(2.0f, 5.0f, -15.0f),
            glm::vec3(-1.5f, -2.2f, -2.5f),
            glm::vec3(-3.8f, -2.0f, -12.3f),
            glm::vec3(2.4f, -0.4f, -3.5f),
            glm::vec3(-1.7f, 3.0f, -7.5f),
            glm::vec3(1.3f, -2.0f, -2.5f),
            glm::vec3(1.5f, 2.0f, -2.5f),
            glm::vec3(1.5f, 0.2f, -1.5f),
            glm::vec3(-1.3f, 1.0f, -1.5f)
        };

        float angle0 = 0;
        float angle1 = 0;
        float angle2 = 0;
        int i = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.clear(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();


            /* handle Resizing: udpate projection matrix if window been resized */
            if (windowObject.getHeight() != w_Height || windowObject.getWidth() != w_Width) {
                //windowObject.update(w_Width, w_Height);
            }
                

            if (i > 180)
                i = 0;
            i += 3;
            /* Handel Esc Key Down */
            processInput(window, &camera);
            camera.processInput();
            /*
            * Render here
            */
            
            texture.bind();
            shader.bind();     

                     
            shader.setUniformMat4f("view", camera.getViewMat());
            shader.setUniformMat4f("projection", projection);
            
            //float color = sin(i * 3.14 / 180);
            float color = 0.5f;
            shader.setUniform4f("u_Color", color, color, color, 1.0f);
            shader.setUniform1i("u_Texture", 0);

            for (unsigned int i = 0; i < 10; i++) {
                model = glm::mat4(1.0);
                model = glm::translate(model, cubePositions[i]);
                model = glm::rotate(model, glm::radians(angle0), glm::vec3(1.0f, 0.0f, 0.0f));
                model = glm::rotate(model, glm::radians(angle1), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::rotate(model, glm::radians(angle2), glm::vec3(0.0f, 0.0f, 1.0f));


                shader.setUniformMat4f("model", model);
                /* Triangle :) */
                renderer.draw(va, ib, shader);
            }
            

            {
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
                ImGui::SliderFloat("Rotate X", &angle0, 0.0f, 360.0f);
                ImGui::SliderFloat("Rotate Y", &angle1, 0.0f, 360.0f);
                ImGui::SliderFloat("Rotate Z", &angle2, 0.0f, 360.0f);
                //ImGui::SliderFloat("Translate Y", &translation.y, -1.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }


           

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }        
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate(); // it returns error, !
    return 0;
}

void processInput(GLFWwindow *window, Camera *camera)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        camera->toggleMouseInput();

}