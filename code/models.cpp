/*
    NOTE: These "models" were just used for testing purposes and include
    some unnecessary information such as colors (which I used for making
    sure the texture coords were correct). Even the texture coords could 
    be removed, since they can easily be calculated inside the shader.

    To render a GUI, things like GL_TRIANGLE_STRIPS might be interesting,
    but to keep things simple standard indexed quads were used.
 */
float quadVertices[] = 
{
    -1.0f, -1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // bottom left
     1.0f, -1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.1f, 0.0f, // bottom right
     1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.1f, 1.0f, // top right
    -1.0f,  1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  0.0f, 1.0f  // top left
};
unsigned int quadIndices[] = 
{
    0, 1, 3,
    1, 2, 3
};

float cubeVertices[] = 
{
    // front
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f,  // bottom left
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // top right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // top left

    // back
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  1.0f, 0.0f,  // bottom right
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  0.0f, 1.0f,  // top left

    // left 
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  // bottom left
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  1.0f, 0.0f,  // bottom right
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // top right
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top left

    // right
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,  // bottom left
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  // top right
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f,  // top left

    // top
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,  // bottom right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,  1.0f, 1.0f,  // top right
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top left

    // bottom
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,  0.0f, 0.0f,  // bottom left
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f,  // top right
    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f,  // top left
};

unsigned int cubeIndices[] = 
{
     0,  1,  2,
     0,  2,  3,

     4,  6,  7,
     4,  5,  6,

     8,  9, 10,
     8, 10, 11,

    12, 13, 14,
    12, 14, 15,

    16, 17, 18,
    16, 18, 19,

    20, 21, 22,
    20, 22, 23,
};