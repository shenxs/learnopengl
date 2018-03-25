#include <GL/glew.h>
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window ,int width,int height);

int main(){

  using namespace std;

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



  glViewport(0, 0, 800, 600);


  while(!glfwWindowShouldClose(window)){
    glfwSwapBuffers(window);

    //检查事件触发并调用相应的函数
    glfwPollEvents();
  }


  glfwTerminate();
  return 0;
}








