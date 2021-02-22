float quadVertices[] = 
{
      0.0f,   0.0f, 0.0f,  0.0f, 0.0f, // bottom left
    100.0f,   0.0f, 0.0f,  0.1f, 0.0f, // bottom right
    100.0f, 100.0f, 0.0f,  0.1f, 1.0f, // top right
      0.0f, 100.0f, 0.0f,  0.0f, 1.0f  // top left
};

unsigned int quadIndices[] = 
{
    0, 1, 3,
    1, 2, 3
};