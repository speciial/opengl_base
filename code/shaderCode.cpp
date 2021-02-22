char *quadVertexShaderCode = R"(#version 460 core
    layout (location = 0) in vec3 aPosition;
    layout (location = 1) in vec2 aTexCoords;

    uniform vec3 uColor;
    uniform mat4 uMVP;

    out vec3 fragColor;

    void main()
    {
        gl_Position = uMVP * vec4(aPosition, 1.0f);
        fragColor = uColor;
    }

)";

char *quadFragmentShaderCode = R"(#version 460 core

    in vec3 fragColor;

    out vec4 color;

    void main()
    {
        color = vec4(fragColor, 1.0f);
    }

)";