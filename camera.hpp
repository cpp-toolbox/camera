#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "glm/glm.hpp"

class Camera {
public:
  // measured in radians
  double yaw_angle = 0.0;
  double pitch_angle = 0.0;
  glm::vec3 look_direction = glm::vec3(0.0, 0.0, 1.0);
  glm::vec3 up_direction = glm::vec3(0.0, 1.0, 0.0);
  void update_look_direction(double change_in_yaw_angle,
                             double change_in_pitch_angle);
  glm::vec3 input_snapshot_to_input_direction(bool forward_pressed,
                                              bool backward_pressed,
                                              bool right_pressed,
                                              bool left_pressed);

private:
  static glm::vec3 yaw_pitch_angles_to_vector(double yaw, double pitch);
  void clamp_camera_pitch_angle();
};

#endif // CAMERA_HPP
