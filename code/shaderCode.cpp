/*
    NOTE: These are super simple shader programs written for demonstration
    purposes. Since these are not getting modified before loading and 
    compilation, they are not loaded from external files. 
 */

char *cubeVertexShaderCode = R"(#version 460 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aColor;
    layout (location = 2) in vec2 aTexCoord;

    out vec3 outColor;
    out vec2 outTexCoord;

    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;

    void main()
    {
        gl_Position = projection * view * model * vec4(aPos, 1.0f);
        outColor = aColor;
        outTexCoord = aTexCoord;
    } 
)";

char *cubeFragmentShaderCode = R"(#version 460 core
    in vec3 outColor;
    in vec2 outTexCoord;    

    out vec4 FragColor;

    uniform sampler2D outTexture1;
    uniform sampler2D outTexture2;

    void main()
    {
        FragColor = vec4(outColor, 1.0f) * mix(texture(outTexture1, outTexCoord), texture(outTexture2, outTexCoord), 0.2f);
    }
)";