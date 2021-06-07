#include "Shader.h"
#include <glad/glad.h>
#include <iostream>

//
// Globals
//

static const char * g_DefaultVertexShaderSource =
R"glsl(
    #version 330 core
    layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
    layout (location = 1) in vec3 aColor; // the color variable has attribute position 1

    out vec3 ourColor; // output a color to the fragment shader

    void main()
    {
      gl_Position = vec4(aPos, 1.0);
      ourColor    = aColor;
    }
  )glsl";

static const char * g_DefaultFragmentShaderSource =
R"glsl(
    #version 330 core
    out vec4 FragColor;
    in vec3  ourColor;

    void main()
    {
      FragColor = vec4(ourColor, 1.0);
    }
  )glsl";

//
// Construction / Destruction
//

CShader::CShader()
{
  LoadFromSource(g_DefaultVertexShaderSource, g_DefaultFragmentShaderSource);
}

CShader::CShader(
	const std::string & _VertexPath,
	const std::string & _FragmentPath
  )
{
  LoadFromFile(_VertexPath, _FragmentPath);
}

CShader::~CShader()
{
  glDeleteProgram(m_ProgramID);
}

//
// Interface
//

void CShader::Use()
{
  glUseProgram(m_ProgramID);
}

//
// Service
//

void CShader::LoadFromFile(
    const std::string & _VertexPath,
    const std::string & _FragmentPath
  )
{
  // TODO

  LoadFromSource(g_DefaultVertexShaderSource, g_DefaultFragmentShaderSource); // TEMP
}

void CShader::LoadFromSource(
    std::string _VertexSource,
    std::string _FragmentSource
  )
{
  m_VertexSource   = std::move(_VertexSource);
  m_FragmentSource = std::move(_FragmentSource);

  CreateShaderProgram(m_VertexSource.c_str(), m_FragmentSource.c_str());
}

void CShader::CreateShaderProgram(
    const char * _VertexSource,
    const char * _FragmentSource
  )
{
  unsigned int VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(VertexShaderID, 1, &_VertexSource, NULL);
  glCompileShader(VertexShaderID);

  char InfoLog[512];
  if (!CheckIfShaderCompile(VertexShaderID, InfoLog))
  {
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << InfoLog << std::endl;
    return;
  }

  unsigned int FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(FragmentShaderID, 1, &_FragmentSource, NULL);
  glCompileShader(FragmentShaderID);

  if (!CheckIfShaderCompile(FragmentShaderID, InfoLog))
  {
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << InfoLog << std::endl;
    return;
  }

  m_ProgramID = glCreateProgram();
  glAttachShader(m_ProgramID, VertexShaderID);
  glAttachShader(m_ProgramID, FragmentShaderID);
  glLinkProgram(m_ProgramID);

  if (!CheckIfShaderProgramLinked(m_ProgramID, InfoLog))
  {
    std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << InfoLog << std::endl;
    return;
  }

  glDeleteShader(VertexShaderID);
  glDeleteShader(FragmentShaderID);
}

bool CShader::CheckIfShaderCompile(
    unsigned int _ShaderIdx,
    char *       _ErrorLog
  )
{
  int IsSuccess;
  glGetShaderiv(_ShaderIdx, GL_COMPILE_STATUS, &IsSuccess);

  if (!IsSuccess)
    glGetShaderInfoLog(_ShaderIdx, 512, NULL, _ErrorLog);

  return IsSuccess != 0;
}

bool CShader::CheckIfShaderProgramLinked(
    unsigned int _ShaderProgramIdx,
    char *       _ErrorLog
  )
{
  int IsSuccess;
  glGetProgramiv(_ShaderProgramIdx, GL_COMPILE_STATUS, &IsSuccess);

  if (!IsSuccess)
    glGetShaderInfoLog(_ShaderProgramIdx, 512, NULL, _ErrorLog);

  return IsSuccess != 0;
}
