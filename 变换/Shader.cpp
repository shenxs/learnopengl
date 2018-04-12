#include"Shader.h"

//构造函数 ,顶点着色器路径 ,片段着色器路径
Shader::Shader(const GLchar* vertexShaderSourcePath,const GLchar* fragmentShaderSourcePath){
  using namespace std;
  string vertexCode;
  string fragmentCode;
  ifstream vertexShaderFile;
  ifstream fragmentShaderFile;

  vertexShaderFile.exceptions(ifstream::failbit|ifstream::badbit);
  fragmentShaderFile.exceptions(ifstream::failbit|ifstream::badbit);

  try{
    //打开文件
    vertexShaderFile.open(vertexShaderSourcePath);
    fragmentShaderFile.open(fragmentShaderSourcePath);
    stringstream vertexShaderStream ,fragmentShaderStream;
    vertexShaderStream << vertexShaderFile.rdbuf();
    fragmentShaderStream << fragmentShaderFile.rdbuf();

    //关闭文件
    vertexShaderFile.close();
    fragmentShaderFile.close();

    //转换数据流到stream
    vertexCode = vertexShaderStream.str();
    fragmentCode = fragmentShaderStream.str();
  }catch(ifstream::failure e){
    cout<<"ERROR::SHADER::文件读取失败"<<endl;
  }
  const char* vShaderCode=vertexCode.c_str();
  const char* fShaderCode=fragmentCode.c_str();

  //编译着色器
  unsigned int vertex,fragment;
  int success;
  char infoLog[512];

  //顶点着色器
  vertex=glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, 0);
  glCompileShader(vertex);
  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(vertex, sizeof(infoLog), 0, infoLog);
    cout<<"顶点着色器编译错误:\n"<<infoLog<<endl;
  }

  fragment=glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, 0);
  glCompileShader(fragment);
  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if(!success){
    glGetShaderInfoLog(fragment, sizeof(infoLog), 0, infoLog);
    cout<<"片段着色器编译错误:\n"<<infoLog<<endl;
  }
  ID=glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if(!success){
    glGetProgramInfoLog(ID, sizeof(infoLog), 0, infoLog);
    cout<<"管线链接失败:\n"<<infoLog<<endl;
  }
  glDeleteShader(vertex);
  glDeleteShader(fragment);

}
//激活
void Shader::use(){
  glUseProgram(ID);
}
//uniform 工具函数
void Shader::setBool(const std::string &name, bool value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}
void Shader::setInt(const std::string &name, int value) const{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
