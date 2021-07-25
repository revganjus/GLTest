#pragma once

#include <string>

class CShader
{
public: // Comstruction / Destruction

  CShader();

  CShader(
      const std::string & _VertexPath,
      const std::string & _FragmentPath
    );

  ~CShader();

public: // Interface

  void Use();

  void LoadFromFile(
      const std::string & _VertexPath,
      const std::string & _FragmentPath
    );

protected: // Service

  std::string LoadFromFile(
      const std::string & _FilePath,
      const char *        _DefaultContent
    );

  void LoadFromSource(
      std::string _VertexSource,
      std::string _FragmentSource
    );

  void CreateShaderProgram(
      const char * _VertexSource,
      const char * _FragmentSource
    );

  bool CheckIfShaderCompile(
      unsigned int _ShaderIdx,
      char *       _ErrorLog
    );

  bool CheckIfShaderProgramLinked(
      unsigned int _ShaderProgramIdx,
      char *       _ErrorLog
    );

protected: // Attributes

  unsigned int m_ProgramID;
  std::string  m_VertexSource;
  std::string  m_FragmentSource;
};