float quadVertices[] = 
{
      0.0f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // top left
    100.0f,   0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.1f, 0.0f, // top right
    100.0f, 100.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.1f, 1.0f, // bottom right
      0.0f, 100.0f, 0.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f  // bottom left
};

unsigned int quadIndices[] = 
{
    0, 1, 3,
    1, 2, 3
};