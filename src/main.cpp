#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "main.h"

static const int WINDOW_WIDTH  = 800;
static const int WINDOW_HEIGHT = 600;

const char * vertexShaderSource = 
  R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos; // the position variable has attribute position 0

    void main()
    {
      gl_Position = vec4(aPos, 1.0);
    }
  )glsl";

const char* fragmentShaderSource1 =
  R"glsl(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 ourColor;

    void main()
    {
      FragColor = ourColor;
    }
  )glsl";

const char* fragmentShaderSource2 = 
  R"glsl(
    #version 330 core
    out vec4 FragColor;

    uniform vec4 ourColor;

    void main()
    {
      FragColor = vec4(1.0f, ourColor.yz, 1.0f);
    }
  )glsl";

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
  glViewport(0, 0, width, height);
}

bool CheckIfShaderCompile(unsigned int shaderIdx, char * errorLog)
{
  int success;
  glGetShaderiv(shaderIdx, GL_COMPILE_STATUS, &success);

  if (!success)
    glGetShaderInfoLog(shaderIdx, 512, NULL, errorLog);

  return success != 0;
}

bool CheckIfShaderProgramLinked(unsigned int shaderProgram, char* errorLog)
{
  int success;
  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

  if (!success)
    glGetProgramInfoLog(shaderProgram, 512, NULL, errorLog);

  return success != 0;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

int main()
{
  // glfw: initialize and configure
  // ------------------------------
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  // glfw window creation
  // --------------------
  GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TestOpenGL", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // glad: load all OpenGL function pointers
  // ---------------------------------------
  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  // build and compile our shader program
  // ------------------------------------
  // vertex shader
  unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
  glCompileShader(vertexShader);

  char infoLog[512];
  if (!CheckIfShaderCompile(vertexShader, infoLog))
  {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  // fragment shader 1
  unsigned int fragmentShader1 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader1, 1, &fragmentShaderSource1, NULL);
  glCompileShader(fragmentShader1);

  if (!CheckIfShaderCompile(fragmentShader1, infoLog))
  {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  // fragment shader 2
  unsigned int fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
  glCompileShader(fragmentShader2);

  if (!CheckIfShaderCompile(fragmentShader2, infoLog))
  {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  // link shaders
  unsigned int shaderProgram1 = glCreateProgram();
  glAttachShader(shaderProgram1, vertexShader);
  glAttachShader(shaderProgram1, fragmentShader1);
  glLinkProgram(shaderProgram1);

  if (!CheckIfShaderProgramLinked(shaderProgram1, infoLog))
  {
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  unsigned int shaderProgram2 = glCreateProgram();
  glAttachShader(shaderProgram2, vertexShader);
  glAttachShader(shaderProgram2, fragmentShader2);
  glLinkProgram(shaderProgram2);

  if (!CheckIfShaderProgramLinked(shaderProgram2, infoLog))
  {
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    glfwTerminate();
    return -1;
  }

  unsigned int shaderPrograms[] = { shaderProgram1, shaderProgram2 };

  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader1);
  glDeleteShader(fragmentShader2);

  // set up vertex data (and buffer(s)) and configure vertex attributes
  // ------------------------------------------------------------------
  float vertices[] = {
      // first triangle
      -0.5f,  0.5f, 0.0f,  // top
       0.0f, -0.5f, 0.0f,  // bottom right
      -1.0f, -0.5f, 0.0f,  // bottom left 
      // second triangle
       1.0f,  0.5f, 0.0f,  // top right
      -0.0f,  0.5f, 0.0f,  // top left
       0.5f, -0.5f, 0.0f   // bottom
    };

  const unsigned int TRIANGLES_COUNT = sizeof(vertices) / (sizeof(float) * 9);
  const auto         TRIANGLE_SIZE   = sizeof(vertices) / TRIANGLES_COUNT;

  unsigned int VBOs[TRIANGLES_COUNT];
  unsigned int VAOs[TRIANGLES_COUNT];
  glGenVertexArrays(TRIANGLES_COUNT, VAOs);
  glGenBuffers(TRIANGLES_COUNT, VBOs);

  for (unsigned int i = 0; i < TRIANGLES_COUNT; ++i)
  {
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAOs[i]);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
    glBufferData(GL_ARRAY_BUFFER, TRIANGLE_SIZE, vertices + i * 9, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);
  }

  // uncomment this call to draw in wireframe polygons.
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  while (!glfwWindowShouldClose(window))
  {
    // input
    // -----
    processInput(window);

    // render
    // -----
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // draw triangles

    for (int i = 0; i < TRIANGLES_COUNT; ++i)
    {
      glUseProgram(shaderPrograms[i]);

      // update the uniform color
      float timeValue = glfwGetTime();
      float greenValue = sin(timeValue) / 2.0f + 0.5f;
      int vertexColorLocation = glGetUniformLocation(shaderPrograms[i], "ourColor");
      glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

      glBindVertexArray(VAOs[i]);
      glDrawArrays(GL_TRIANGLES, 0, 3);
      glBindVertexArray(0);
    }
    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
  }
    
  // optional: de-allocate all resources once they've outlived their purpose:
  // ------------------------------------------------------------------------
  glDeleteVertexArrays(TRIANGLES_COUNT, VAOs);
  glDeleteBuffers(TRIANGLES_COUNT, VBOs);

  for (int i = 0; i < TRIANGLES_COUNT; ++i)
    glDeleteProgram(shaderPrograms[i]);

  // glfw: terminate, clearing all previously allocated GLFW resources.
  // ------------------------------------------------------------------
  glfwTerminate();

  return 0;
}