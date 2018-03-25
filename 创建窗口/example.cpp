#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  glfwSetKeyCallback(window, key_callback);

  glewExperimental=GL_TRUE;
  GLenum err=glewInit();
  if(err!=GLEW_OK){
    std::cout<<"glew 初始化失败,程序结束"<<std::endl;
  }

  glViewport(0, 0, 800, 600);
  while(!glfwWindowShouldClose(window)){

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    //检查事件触发并调用相应的函数
    glfwPollEvents();
  }


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

