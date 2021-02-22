#include <stdio.h>
#include <stdlib.h>

#include <glad\glad.h>
#include <GLFW\glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb\stb_image.h>

#include "shaderCode.cpp"
#include "models.cpp"

unsigned int map[13 * 8] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    1, 1, 1, 1, 2, 0, 0, 0, 2, 2, 2, 0, 0,
    1, 1, 1, 1, 1, 2, 2, 2, 1, 1, 1, 2, 2,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
};

glm::vec3 cameraPosition(0.0f, 0.0f, 0.0f);

static unsigned int 
compileAndLinkShaderProgram(char *vertexShaderCode, char *fragmentShaderCode)
{
    unsigned int result = 0; 

    int success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, 0);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        printf("Failed to compile vertext shader:\n%s", infoLog);
        result = -1;   
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        printf("Failed to compile fragment shader:\n%s", infoLog);
        result = -1;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if(!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        printf("Failed to link shader program:\n%s", infoLog);
        result = -1;
    }

    glDetachShader(shaderProgram, vertexShader);
    glDetachShader(shaderProgram, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    result = shaderProgram;
    return(result);
}

static void 
processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    } 
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {  
        cameraPosition.x += 20;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        cameraPosition.x -= 20;
    }
}

int main(void)
{
    GLFWwindow* window;

    if (!glfwInit()){ return(-1); }
    window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return(-1);
    }
    glfwMakeContextCurrent(window);
    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD!\n");
        return(-1);
    }

    unsigned int shaderProgram = 
        compileAndLinkShaderProgram(quadVertexShaderCode, quadFragmentShaderCode);

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), quadIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glBindVertexArray(0);

    // NOTE: bottom and top are flipped so that I can walk through the x-y-for 
    // like I'm used to. This might change in the future.
    glm::mat4 projection = glm::ortho(0.0f, 1280.0f, 720.0f, 0.0f);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.3f, 0.4f, 0.6f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glBindVertexArray(VAO);
        glUseProgram(shaderProgram);

        glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPosition);

        // map            
        for(int y = 0; y < 8; ++y)
        {
            for(int x = 0; x < 13; ++x)
            {
                if(map[x + (y * 13)])
                {
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * 100.0f, y * 100.f, 0.0f));

                    glm::mat4 mvp = projection * view * model;
                    int transformLocation = glGetUniformLocation(shaderProgram, "uMVP");
                    glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
                    
                    if(map[x + (y * 13)] == 1)
                    {
                        glm::vec3 color(0.8f, 0.6f, 0.4f);
                        int fragColorLocation = glGetUniformLocation(shaderProgram, "uColor");
                        glUniform3fv(fragColorLocation, 1, glm::value_ptr(color));
                    } 
                    else if(map[x + (y * 13)] == 2)
                    {
                        glm::vec3 color(0.2f, 0.8f, 0.3f);
                        int fragColorLocation = glGetUniformLocation(shaderProgram, "uColor");
                        glUniform3fv(fragColorLocation, 1, glm::value_ptr(color));
                    }

                    glDrawElements(GL_TRIANGLES, sizeof(quadIndices), GL_UNSIGNED_INT, 0);
                }
            }
        }

        // player
        glm::mat4 model = glm::translate(glm::mat4(1.0f), 
                                         glm::vec3((1280.0f / 2.0f) - 50.0f, (720.f / 2.0f) - 50.0f, 0.0f));
        glm::mat4 mvp = projection * model;
        int transformLocation = glGetUniformLocation(shaderProgram, "uMVP");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(mvp));
                    
        glm::vec3 color(0.5f, 0.8f, 0.3f);
        int fragColorLocation = glGetUniformLocation(shaderProgram, "uColor");
        glUniform3fv(fragColorLocation, 1, glm::value_ptr(color));
        glDrawElements(GL_TRIANGLES, sizeof(quadIndices), GL_UNSIGNED_INT, 0);
                

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return(0);
}
