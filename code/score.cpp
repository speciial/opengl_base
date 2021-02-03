// NOTE: This is the shader code used for drawing the score
char *scoreVertexShaderCode = R"(#version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;

    out vec2 outTexCoord;

    uniform float textureOffset;
    uniform mat4 transform;

    void main()
    {
        gl_Position = transform * vec4(aPos, 1.0f);
        outTexCoord = vec2(aTexCoord.x + textureOffset, aTexCoord.y);
    }
)";

char *scoreFragmentShaderCode = R"(#version 460 core
    in vec2 outTexCoord;    

    out vec4 FragColor;

    uniform sampler2D outTexture;

    void main()
    {
        FragColor = texture(outTexture, outTexCoord);
    }
)";

// NOTE: These could just as well go into the setup function. They were just left 
// out here because of copy-pasting.
float scoreVertices[] = 
{
    // posistions        // tex coords
    -1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
     1.0f, -1.0f, 0.0f,  0.1f, 0.0f, // bottom right
     1.0f,  1.0f, 0.0f,  0.1f, 1.0f, // top right
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f  // top left
};
unsigned int scoreIndices[] = 
{
    0, 1, 3,
    1, 2, 3
};

// NOTE: If the counter should display more than 5 digits, just change this 
// to whatever amount fits.
#define COUNTER_LENGTH 5

struct score_info
{
    int characterWidth;
    int characterHeight;

    unsigned int shaderProgram;
    unsigned int vao;
};

// NOTE: This function sets up all the necessary things for drawing the score, such as 
// the vertex buffer and the textures.
//
// For texture loading the stb_image.h single header file library is used. If you're 
// using some different library, substitue the relavent texture loading code.
static score_info
setUpScore(int characterWidth, int characterHeight, unsigned int shaderProgram,
           char *textureFilePath, unsigned int textureSlot)
{
    score_info result = {};
    result.characterWidth = characterWidth;
    result.characterHeight = characterHeight;
    result.shaderProgram = shaderProgram;

    unsigned int numberTextureAtlas;
    glGenTextures(1, &numberTextureAtlas);
    glBindTexture(GL_TEXTURE_2D, numberTextureAtlas);
    
    // NOTE: Wrapping and filters are the most pointles things. Why don't they just have
    // usable default values? 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // NOTE: This is the texture loading part. In the case that a different library is used, 
    // this code needs to be adjusted.
    int width, height, BPP;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *imageData = stbi_load(textureFilePath, &width, &height, &BPP, 0);
    if(imageData)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else 
    {
        printf("Failed to load texture!\n");
    }
    stbi_image_free(imageData);

    glUseProgram(shaderProgram);
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, numberTextureAtlas);
    glUniform1i(glGetUniformLocation(shaderProgram, "outTexture"), textureSlot);
    glUseProgram(0);

    // setting up the vertex buffer
    unsigned int quadVAO;
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    unsigned int quadVBO;
    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(scoreVertices), scoreVertices, GL_STATIC_DRAW);
    
    // NOTE: Index buffers are somewhat overkill for this simple geometry but this
    // is only done on startup anyway.
    unsigned int quadEBO;
    glGenBuffers(1, &quadEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(scoreIndices), scoreIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    result.vao = quadVAO;
    glBindVertexArray(0);

    return(result);
}

static void 
drawScore(int score, score_info scoreInfo, glm::vec2 position, float scale)
{
    char numbers[COUNTER_LENGTH] = {};
    itoa(score, numbers, 10);
    // NOTE: This calculates the number of leading zeros, which need to be placed 
    // in front of the the score. It's not the nicest thing in the world, but I 
    // couldn't be bothered to find a better solution.
    int initialOffset = 0;
    for(int numberIndex = 0; numberIndex < COUNTER_LENGTH; ++numberIndex)
    {
        if(numbers[numberIndex] == '\0')
        {
            ++initialOffset;
        }
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int leadingZeros = 0;
    for(int numberIndex = 0; numberIndex < COUNTER_LENGTH; ++numberIndex)
    {
        float scoreTextureOffset = 0;
        if(leadingZeros < initialOffset)
        {
            scoreTextureOffset = 0;
            ++leadingZeros;
        }
        else 
        {
            scoreTextureOffset = (float)(numbers[numberIndex - initialOffset] - 48) / 10.0f;
        }
        
        // NOTE: These numbers might need to be adjusted when using different font/
        // texture atlas. 
        // TODO: Use the character size to display the score properly.
        glm::mat4 transform  = glm::mat4(1.0f);
        glm::vec3 translateV = glm::vec3((position.x + (0.183 * numberIndex)),
                                         position.y, 0.0f);
        glm::vec3 scaleV     = glm::vec3((scale * (0.183 / 2)), 
                                         scale * 0.5, 1.0f);
        transform = glm::translate(transform, translateV);
        transform = glm::scale(transform, scaleV);

        // NOTE: The library does not cache the uniform locations to have as little 
        // internal state as possible.
        glUseProgram(scoreInfo.shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(scoreInfo.shaderProgram, "transform"),
                           1, GL_FALSE, glm::value_ptr(transform));
        glUniform1f(glGetUniformLocation(scoreInfo.shaderProgram, "textureOffset"), 
                    scoreTextureOffset);

        glBindVertexArray(scoreInfo.vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    glBindVertexArray(0);
    glDisable(GL_BLEND);
}