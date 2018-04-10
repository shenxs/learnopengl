#ifndef SHADER_H
#define SHADER_H

#ifdef USE_GLEW
    #include <GL/glew.h>
#else
    #warning "Not using GLEW"
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>



class Shader{
public:
  //程序的ID
  unsigned int  ID;
  //构造函数 ,顶点着色器路径 ,片段着色器路径
  Shader(const GLchar* vertexShaderSourcePath,const GLchar* fragmentShaderSourcePath);
  //激活
  void use();
  //uniform 工具函数
  void setBool(const std::string &name, bool value) const;
  void setInt(const std::string &name, int value) const;
  void setFloat(const std::string &name, float value) const;
};

#endif
