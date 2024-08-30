#include "camera.hpp"
#include "glm/ext/scalar_constants.hpp"
#include <cmath>

/**
 * \brief update's the camera's look direction based on changes changes in yaw
 * and pitch \author cuppajoeman \date created: 2024, last edited: 2024-02-26
 */
void Camera::update_look_direction(double change_in_yaw_angle, double change_in_pitch_angle) {
    this->yaw_angle += change_in_yaw_angle;
    this->pitch_angle += change_in_pitch_angle;

    this->clamp_camera_pitch_angle();

    this->look_direction = Camera::yaw_pitch_angles_to_vector(this->yaw_angle, this->pitch_angle);
    this->look_direction = glm::normalize(this->look_direction);
}

void Camera::set_look_direction(double new_yaw_angle, double new_pitch_angle) {
    this->look_direction = Camera::yaw_pitch_angles_to_vector(new_yaw_angle, new_pitch_angle);
    this->look_direction = glm::normalize(this->look_direction);

    this->clamp_camera_pitch_angle();
}

/**
 * \brief converts a tuple (yaw, pitch), to a 3 dimensional vector pointing in
 * that direction. \bug converts doubles into floats causing less accurate
 * aiming \author cuppajoeman \date created: 2024, last edited: 2024-02-26
 */
glm::vec3 Camera::yaw_pitch_angles_to_vector(double yaw, double pitch) {
    glm::vec3 vector;
    vector.x = cos(pitch) * sin(yaw);
    vector.y = sin(pitch);
    vector.z = cos(pitch) * cos(yaw);
    return vector;
}

/**
 * \brief clamp the mouse
 * \details clamps this camera's angle between -pi/2 to pi/2, with some margins
 * \note The margins are required because if we allow the camera to look
 * straight up or down, opengl's view of the world gets inverted.
 *
 * \author cuppajoeman
 *
 * \date created: 2023
 */
void Camera::clamp_camera_pitch_angle() {

    auto PI = glm::pi<float>();

    float angle_cutoff_margin = 0.001;

    float a_little_less_than_two_pi = PI / 2 - angle_cutoff_margin;
    float a_little_more_than_negative_two_pi = -PI / 2 + angle_cutoff_margin;

    if (this->pitch_angle > a_little_less_than_two_pi) {
        this->pitch_angle = a_little_less_than_two_pi;
    }

    if (this->pitch_angle < a_little_more_than_negative_two_pi) {
        this->pitch_angle = a_little_more_than_negative_two_pi;
    }
}

/**
 * given an input snapshot, return a normalized vector which represents how the
 * pressed keys should change our movement relative to the current look
 * direction
 *
 * ex) pressing forward should eventually move the player in the direction
 * they're looking
 *
 */
glm::vec3 Camera::input_snapshot_to_input_direction(bool forward_pressed, bool backward_pressed, bool right_pressed,
                                                    bool left_pressed) {

    // TODO calling everything a compmoent is confusing af, fix this
    float forward_component = (float)forward_pressed - (float)backward_pressed;
    float strafe_component = (float)right_pressed - (float)left_pressed;

    // std::cout << glm::to_string(player.camera.look_direction) << std::endl;

    glm::vec3 xz_player_look_direction = glm::vec3(this->look_direction.x, 0.0f, this->look_direction.z);

    if (glm::length(xz_player_look_direction) != 0) {
        xz_player_look_direction = glm::normalize(xz_player_look_direction);
    }

    // TODO once this is in the player class we will be using player look
    // direction, then doing stuff the camera with that information
    glm::vec3 forward_input_component = forward_component * xz_player_look_direction;
    glm::vec3 strafe_direction = glm::cross(xz_player_look_direction, this->up_direction);
    glm::vec3 strafe_input_component = strafe_component * strafe_direction;
    glm::vec3 input_vector = forward_input_component + strafe_input_component;

    if (glm::length(input_vector) == 0) {
        return input_vector;
    } else {
        return glm::normalize(input_vector);
    }
}
