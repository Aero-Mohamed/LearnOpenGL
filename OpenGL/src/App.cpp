#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


void processInput(GLFWwindow* window);


struct ShaderProgramSource 
{
    std::string VertexSource;
    std::string FragmentSource;
    std::string GeometrySource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
    enum class ShaderType {
        NONE = -1, VERTEX=0, FRAGMENT=1, GEOMETRY=2
    };

    std::ifstream stream(filePath); // open file
    ShaderType type = ShaderType::NONE; // initial non
    std::stringstream ss[3];
    std::string line;
    while (getline(stream, line)) {
        if (line.find("#shader") != std::string::npos){
            // if shader found update Type variable
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
            else if (line.find("geometry") != std::string::npos)
                type = ShaderType::GEOMETRY;
        }else{
            // otherwise write to stringStream unless type is -1
            if(type != ShaderType::NONE)
                ss[(int)type] << line << '\n';
        }
    }
    return { 
        ss[(int)ShaderType::VERTEX].str(),
        ss[(int)ShaderType::FRAGMENT].str(),
        ss[(int)ShaderType::GEOMETRY].str()
    };
}

inline const char* shaderType2Str(GLenum shader)
{
    switch (shader) {
        case GL_VERTEX_SHADER: return "Vertex Shader";
        case GL_FRAGMENT_SHADER: return "Fragment Shader";
        case GL_GEOMETRY_SHADER: return "Geometry Shader";
        default: return "";
    }
}

static unsigned int CompileShader(GLenum type, const std::string& source)
{
    
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    GlCall(glShaderSource(id, 1, &src, nullptr));
    GlCall(glCompileShader(id));

    int results;
    GlCall(glGetShaderiv(id, GL_COMPILE_STATUS, &results));
    if (results == GL_FALSE) {

        int len;
        GlCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len));
        char* message = (char*)malloc(len * sizeof(char));
        GlCall(glGetShaderInfoLog(id, len, &len, message));

        std::cout << "Failed to compile " <<(shaderType2Str(type)) << std::endl;
        std::cout << message << std::endl;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    GlCall(glAttachShader(program, vs));
    GlCall(glAttachShader(program, fs));
    GlCall(glLinkProgram(program));
    GlCall(glValidateProgram(program));

    GlCall(glDetachShader(program, vs));
    GlCall(glDetachShader(program, fs));
    GlCall(glDeleteShader(vs));
    GlCall(glDeleteShader(fs));

    return program;
}

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
        float positions[12] = {
            -0.5f, -0.5f,
            0.5f, -0.5f,
            0.5f, 0.5f,
            -0.5f, 0.5f,
        };

        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0,
        };

        VertexArray va;
        VertexBuffer vb(positions, sizeof(positions));
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.addBuffer(vb, layout);

        // Index Buffer Object
        IndexBuffer ib(indices, 6);

        ShaderProgramSource shaderSource = ParseShader("resources/shaders/Basic.shader");

        unsigned int shader = CreateShader(shaderSource.VertexSource, shaderSource.FragmentSource);
        GlCall(glUseProgram(shader));

        int ColorLocation = glGetUniformLocation(shader, "u_Color");
        ASSERT(ColorLocation != -1);


        // Unbind EveryThing
        GlCall(glUseProgram(0));
        GlCall(glBindVertexArray(0));
        GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
        GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

        int i = 0;
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            i++;
            /* Handel Esc Key Down */
            processInput(window);
            /*
            * Render here
            */
            // Set Screen Color
            GlCall(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));
            GlCall(glClear(GL_COLOR_BUFFER_BIT));

            /* Binds whatever required to Draw */
            GlCall(glUseProgram(shader)); //bind shader program 
            va.bind(); // bind vertex array
            ib.bind(); // bind index buffer

            // Set Color
            GlCall(glUniform4f(ColorLocation, sin((long)i * 3.14 / 180), 1.0f, 0.0f, 1.0f));
            /* Triangle :) */
            GlCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));


            /* Swap front and back buffers */
            glfwSwapBuffers(window);
            /* Poll for and process events */
            glfwPollEvents();
        }

        GlCall(glDeleteProgram(shader));
    }
    
    glfwTerminate(); // it returns error, !
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}