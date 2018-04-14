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
#include <math.h>

//矩阵运算
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext.hpp>


#include"Shader.h"
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void error_callback(int error, const char *description);
void key_callback(GLFWwindow *window, int key, int scancode, int action,int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
float alpha=0.2;
glm::vec3 cameraPos = glm::vec3(0.0f,0.0f,3.0f);

glm::vec3 cameraTarget=glm::vec3(0.0f,0.0f,0.0f);
glm::vec3 cameraDirection=glm::normalize(cameraPos-cameraTarget);

glm::vec3 up =glm::vec3(0.0f,1.0f,0.0f);
glm::vec3 cameraRight=glm::normalize(glm::cross(up, cameraDirection));
glm::vec3 cameraUp=glm::normalize(glm::cross(cameraRight, cameraDirection));
glm::vec3 cameraFront=glm::vec3(0.0f,0.0f,-1.0f);

//相机的俯仰角,偏航角
float pitch=0.0f,yaw=180.0f;
bool firstMouse=true;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float fov=45;

//鼠标的焦点位置
float lastX = 400, lastY = 300;

int main() {

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

  GLFWwindow *window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
  if (window == NULL) {
    cout << "GLFW窗口创建失败" << endl;
    glfwTerminate();
    return -1;
  }

  //将窗口的上下文设定为当前线程的上下文
  glfwMakeContextCurrent(window);

  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  //设置窗口大小变换的回调函数
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
  //设置按键的回调函数
  glfwSetKeyCallback(window, key_callback);
  //设置鼠标事件的回调
  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetScrollCallback(window, scroll_callback);

#ifdef USE_GLEW
  //使用实验性质，必须加上,不然会报错
  glewExperimental = GL_TRUE;
  GLenum err = glewInit();
  if (err != GLEW_OK) {
    std::cout << "glew 初始化失败,程序结束" << std::endl;
  }
#endif
  glViewport(0, 0, 800, 600);

  // int nrAttributes;
  // glGetIntegerv(GL_MAX_VERTEX_ATTRIBS,&nrAttributes);
  // std::cout<<"支持的最多的顶点向量"<<nrAttributes<<std::endl;

  //生成纹理

  unsigned int texture1,texture2;
  glGenTextures(1, &texture1);
  glGenTextures(1, &texture2);
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture1);

  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //加载纹理贴图
  int width,height,nrChannels;
  unsigned char* data=stbi_load("./container.jpg", &width, &height, &nrChannels, 0);
  if(data){
    //传输纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    //产生多级纹理
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    cout<<"纹理加载失败"<<endl;
  }
  stbi_image_free(data);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, texture2);
  // 为当前绑定的纹理对象设置环绕、过滤方式
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  stbi_set_flip_vertically_on_load(true);
  data=stbi_load("./awesomeface.png", &width, &height, &nrChannels, 0);
  if(data){
    //传输纹理数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    //产生多级纹理
    glGenerateMipmap(GL_TEXTURE_2D);
  }else{
    cout<<"纹理加载失败"<<endl;
  }
  stbi_image_free(data);


  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };


  //索引缓冲对象
  unsigned int indices[] = {0, 1, 3, 1, 2, 3};

  unsigned int VBO, VAO, EBO;

  //生成顶点数组
  glGenVertexArrays(1, &VAO);
  //生成顶点缓冲
  glGenBuffers(1, &VBO);
  //生成索引缓冲
  glGenBuffers(1, &EBO);

  //绑定当前操作的顶点数组对象
  glBindVertexArray(VAO);

  //绑定当前的缓冲
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  //将数据复制到buffer中
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //绑定索引缓冲对象
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  //将索引复制到buffer中
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  //定义常规的顶点参数数组数据
  //指定顶点参数的位置和使用的格式
  //数据传送的位置,与顶点着色器对应,
  //设定顶点属性大小,三维向量,所以是3,每一个数据包含3个float
  //数据类型,在这里是float
  //是否标准化,标准化会被映射到-1,1之间,对于unsigned会被映射到0,1之间
  //步长,两个数据之间的下标偏差
  //设置第一个数据在vbo中的起始位置
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  //纹理位置参数
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), (void*)(3*sizeof(float)));
  glEnableVertexAttribArray(2);

  // note that this is allowed, the call to glVertexAttribPointer registered VBO
  // as the vertex attribute's bound vertex buffer object so afterwards we can
  // safely unbind
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  // You can unbind the VAO afterwards so other VAO calls won't accidentally
  // modify this VAO, but this rarely happens. Modifying other
  // VAOs requires a call to glBindVertexArray anyways so we generally don't
  // unbind VAOs (nor VBOs) when it's not directly necessary.
  // glBindVertexArray(0);

  Shader shader("./shader.vert","./shader.frag");
  shader.use();
  shader.setInt("texture1", 0);
  shader.setInt("texture2", 1);
  shader.setFloat("alpha", alpha);

  //使用线性绘制
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  //填充
  // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


  //定义变换矩阵

  //模型矩阵
  glm::mat4 model;
  model=glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f,0.0f,0.0f));

  //观察矩阵
  glm::mat4 view;
  view=glm::translate(view, glm::vec3(0.0f,0.0f,-3.0f));


  //定义投影矩阵
  glm::mat4 projection;
  projection=glm::perspective(glm::radians(fov), 800.0f/600.0f, 0.1f, 100.0f);

  glm::vec3 cubePositions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };

  shader.setMatrix4fv("model", model);
  shader.setMatrix4fv("view", view);
  shader.setMatrix4fv("projection", projection);
  glEnable(GL_DEPTH_TEST);


  //设定上次的时间
  lastFrame=glfwGetTime();

  while (!glfwWindowShouldClose(window)) {

    lastFrame=glfwGetTime();
    //设定glClear使用的颜色
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    shader.setFloat("alpha", alpha);
    glBindVertexArray(VAO);

    projection=glm::perspective(glm::radians(fov), 800.f/600.f, 0.1f, 100.0f);
    shader.setMatrix4fv("projection", projection);
    //更新相机view
    view=glm::lookAt(cameraPos, cameraPos+cameraFront, cameraUp);
    shader.setMatrix4fv("view", view);

    for(int i=0;i<10;i++){
      glm::mat4 aModel;
      aModel=glm::translate(aModel,cubePositions[i]);
      float angle = 10.0f * i;
      aModel=glm::rotate(aModel, glm::radians(angle), glm::vec3(1.0f,0.3f,0.5f));
      if(i%3==0){
        aModel=glm::rotate(aModel,(float)sin(glfwGetTime()), glm::vec3(1.0f,0.3f,0.5f));
      }
      shader.setMatrix4fv("model", aModel);
      glDrawArrays(GL_TRIANGLES, 0, 36);

      deltaTime=glfwGetTime()-lastFrame;

    }

    glfwSwapBuffers(window);
    //检查事件触发并调用相应的函数
    glfwPollEvents();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
}

void key_callback(GLFWwindow *window, int key, int scancode, int action,
                  int mods) {
  if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, true);
  }else if(key == GLFW_KEY_UP && action == GLFW_PRESS){
    if(alpha<=0.95){
      alpha+=0.05;
    }
  }else if(key == GLFW_KEY_DOWN && action ==GLFW_PRESS ){
    if(alpha>=0.05){
      alpha-=0.05;
    }
  }
  float cameraSpeed=2.5f * deltaTime;
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void error_callback(int error, const char *description) {
  fputs(description, stderr);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos){

  if(firstMouse){
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float xoffset=xpos-lastX;
  float yoffset=ypos-lastY;

  lastX=xpos;
  lastY=ypos;

  float sensitivity=0.5f;
  xoffset*=sensitivity;
  yoffset*=sensitivity;

  if(pitch>89.0f){
    pitch=89.0f;
  }else if(pitch<-89.f){
    pitch=-89.0f;
  }else{
    pitch+=yoffset;
  }

  yaw+=xoffset;

  cameraFront.y=sin(glm::radians(pitch));
  cameraFront.z=cos(glm::radians(pitch))*cos(glm::radians(yaw));
  cameraFront.x=cos(glm::radians(pitch))*sin(glm::radians(yaw));

}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
  if(fov >= 1.0f && fov <= 45.0f)
    fov -= yoffset;
  if(fov <= 1.0f)
    fov = 1.0f;
  if(fov >= 45.0f)
    fov = 45.0f;
  std::cout<<'1'<<std::endl;
}
