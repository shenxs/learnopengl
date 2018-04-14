#include"Camera.h"


glm::mat4 Camera::getViewMatrix(){
  return glm::lookAt(Position,Position+Front, Up);
}

void Camera::ProcessKeyBoard(Camera_Movement direction ,float deltaTime){
  float veloccity=MovementSpeed*deltaTime;
  if(direction == FORWARD)
    Position += veloccity*Front;
  if(direction == BACKWARD)
    Position -= veloccity*Front;
  if(direction == LEFT)
    Position -= veloccity*Right;
  if(direction == RIGHT)
    Position += veloccity*Right;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch){
  xoffset *= MouseSensitivity;
  yoffset *= MouseSensitivity;

  Yaw   += xoffset;
  Pitch += yoffset;

  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (constrainPitch)
    {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

  // Update Front, Right and Up Vectors using the updated Euler angles
  updateCameraVectors();
}
void Camera::ProcessMouseScroll(float yoffset){
  if (Zoom >= 1.0f && Zoom <= 45.0f)
    Zoom -= yoffset;
  if (Zoom <= 1.0f)
    Zoom = 1.0f;
  if (Zoom >= 45.0f)
    Zoom = 45.0f;
}
void Camera::updateCameraVectors(){
  glm::vec3 front;
  front.x = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  front.y = sin(glm::radians(Pitch));
  front.z = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
  Front = glm::normalize(front);
  Right =glm::normalize(glm::cross(Front, WorldUp));
  Up  =glm::normalize(glm::cross(Front, Right));
}
