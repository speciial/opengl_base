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

// NOTE: Ignore most of the following code. It's just some sample code, to display 
// the score with. Some interesting bits have comments, which you can read. Skip to
// line 195 for the score example! 

// NOTE: This is the source file for the score library
#include "score.cpp" 

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

    // NOTE: No one ever detaches the compiled shaders, but (according to 
    // the docs) a compiled shader _cannot_ be deleted unless it's detached 
    // from all programs.
    // Refer to: http://docs.gl/gl4/glDeleteShader
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

    glEnable(GL_DEPTH_TEST);

    unsigned int sceneShaderProgram = 
        compileAndLinkShaderProgram(cubeVertexShaderCode, cubeFragmentShaderCode);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // wrapping and filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("container.jpg", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to load texture!\n");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    unsigned int texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // wrapping and filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true);
    data = stbi_load("awesomeface.png", &width, &height, &nrChannels, 0);
    if(data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to load texture!\n");
    }
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0);

    // ===================================================
    // NOTE: Here is what you have to do, to use the code!
    // ===================================================
    int score = 0; 
    int characterWidth = 183; 
    int characterHeight = 256;
    unsigned int scoreShader = 
        compileAndLinkShaderProgram(scoreVertexShaderCode, scoreFragmentShaderCode);
    char *textureFilePath = "numbers.png";
    unsigned int textureSlot = 4;

    score_info scoreInfo = setUpScore(characterWidth, characterHeight, scoreShader,
                                      textureFilePath, textureSlot);
    
    // ===================================================
    // ===================================================

    glUseProgram(sceneShaderProgram);
    // Texture for slot 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(sceneShaderProgram, "outTexture1"), 0);
    // Texture for slot 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glUniform1i(glGetUniformLocation(sceneShaderProgram, "outTexture2"), 1);
    glUseProgram(0);

    // NOTE: Since this is a simple example, no fancy auto-layout things are being
    // done. There are some interesting things to do that could possibly make the 
    // code look a little cleaner and the API a lot simpler.
    // Refer to: https://www.khronos.org/opengl/wiki/Vertex_Specification#Separate_attribute_format
    // or: https://www.youtube.com/watch?v=oD1dvfbyf6A&list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&index=14 
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    
    glBindVertexArray(0);

    double last = glfwGetTime();
    double now = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        now = glfwGetTime();
        double delta = now - last;
        if(delta >= 1)
        {
            ++score;
            last = now;
        } 

        glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

        glUseProgram(sceneShaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(sceneShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, sizeof(cubeIndices), GL_UNSIGNED_INT, 0);

        // ===================================================
        // NOTE: Here is what you have to do, to use the code!
        // ===================================================
        
        // Feel free to change the position 
        glm::vec2 scorePosition = glm::vec2(0.1f, 0.5f);
        float scoreScale = 1.0f;

        drawScore(score, scoreInfo, scorePosition, scoreScale);

        // ===================================================
        // ===================================================
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return(0);
}
