#ifdef USE_GLEW
    #include <GL/glew.h>
#else
    #warning "Not using GLEW"
    #define GL_GLEXT_PROTOTYPES
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <iostream>


const char *vertexShaderSource = "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;\n"
  "void main()\n"
  "{\n"
  "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
  "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
  "out vec4 FragColor;\n"
  "void main()\n"
  "{\n"
  "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
  "}\n\0";


void framebuffer_size_callback(GLFWwindow* window ,int width,int height);
void error_callback(int error,const char* description);
void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods);

int main(){

  using namespace std;

  glfwSetErrorCallback(error_callback);

  //初始化 glfw
  glfwInit();

  //设定glfw的设定值，第一个参数为参数名称，第二个为参数值

  //设置主版本号为3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //设置次版本号为3
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  //只是用opengl功能的子集
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow* window=glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if(window==NULL){
    cout<<"GLFW窗口创建失败"<<endl;
    glfwTerminate();
    return -1;
  }

  //将窗口的上下文设定为当前线程的上下文
  glfwMakeContextCurrent(window);

  //设置窗口大小变换的回调函数
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //设置按键的回调函数
  glfwSetKeyCallback(window, key_callback);

#ifdef USE_GLEW
  //使用实验性质，必须加上,不然会报错
  glewExperimental=GL_TRUE;
  GLenum err=glewInit();
  if(err!=GLEW_OK){
    std::cout<<"glew 初始化失败,程序结束"<<std::endl;
  }
#endif

  glViewport(0, 0, 800, 600);


  //顶点们
  float vertices[] = {
   -0.5f, -0.5f, 0.0f,  //left
    0.5f, -0.5f, 0.0f,  //right
    0.0f,  0.5f, 0.0f   //top
  };

  unsigned int VBO, VAO;
  //生成顶点数组
  glGenVertexArrays(1,&VAO);
  //生成顶点缓冲
  glGenBuffers(1,&VBO);

  //绑定当前操作的顶点数组对象
  glBindVertexArray(VAO);
  //绑定当前的缓冲
  glBindBuffer(GL_ARRAY_BUFFER,VBO);


  //将数据复制到buffer中
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

  //定义常规的顶点参数数组数据
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);



  glEnableVertexAttribArray(0);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
  glBindVertexArray(0);

  //声明顶点着色器
  int vertexShader = glCreateShader(GL_VERTEX_SHADER);
  //将着色器源码复制到 vertexShader 对象中
  glShaderSource(vertexShader,1,&vertexShaderSource,NULL);
  //编译顶点着色器
  glCompileShader(vertexShader);


  //获取编译信息
  int success;
  char infoLog[512];
  glGetShaderiv(vertexShader,GL_COMPILE_STATUS,&success);
  if(!success){
    glGetShaderInfoLog(vertexShader,512,NULL,infoLog);
    std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //同理片段着色器
  int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
  glCompileShader(fragmentShader);
  glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
  if (!success){
    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
  }

  //链接程序
  int shaderProgram = glCreateProgram();
  glAttachShader(shaderProgram, vertexShader);
  glAttachShader(shaderProgram, fragmentShader);
  glLinkProgram(shaderProgram);

  glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
  if (!success) {
    glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
    std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
  }

  //删除已经链接的着色器
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);


  while(!glfwWindowShouldClose(window)){

    //设定glClear使用的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glfwSwapBuffers(window);
    //检查事件触发并调用相应的函数
    glfwPollEvents();
  }

  glDeleteVertexArrays(1,&VAO);
  glDeleteBuffers(1,&VBO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}



void framebuffer_size_callback(GLFWwindow* window ,int width,int height){
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow* window,int key,int scancode,int action,int mods){
  if(key == GLFW_KEY_ESCAPE && action ==GLFW_PRESS){
    glfwSetWindowShouldClose(window, true);
  }
}

void error_callback(int error,const char* description){
  fputs(description, stderr);
}
