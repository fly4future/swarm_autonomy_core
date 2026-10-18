#pragma once

#include <chrono>
#include <cstddef>
#include <cstdint>

#include <Eigen/Dense>

namespace ultraloc
{

/**
 * @brief Identity of a localized target (a neighbouring UAV or a marked point on the ground).
 *
 * Every measurement source reports measurements already associated with a target identity.
 */
using TargetId = std::int32_t;

using Timestamp = std::chrono::system_clock::time_point;

/**
 * @brief Direction from a sensor towards a target.
 */
struct Bearing
{
  TargetId        id = -1;
  Eigen::Vector3d direction = Eigen::Vector3d::UnitZ();  // unit vector, expressed in the sensor frame
  std::size_t     sensor_index = 0;                       // index of the sensor that produced the measurement
  Timestamp       stamp;
};

/**
 * @brief Distance from the own UAV to a target.
 */
struct Range
{
  TargetId  id       = -1;
  double    distance = 0.0;  // [m]
  Timestamp stamp;
};

/**
 * @brief Estimated position of a target relative to the own UAV.
 */
struct RelativePosition
{
  TargetId        id = -1;
  Eigen::Vector3d position   = Eigen::Vector3d::Zero();      // [m], expressed in the localization origin frame
  Eigen::Matrix3d covariance = Eigen::Matrix3d::Identity();  // [m^2]
  Timestamp       stamp;
};

}  // namespace ultraloc
