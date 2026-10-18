#pragma once

#include <cstdint>
#include <vector>

#include <ultraloc/types.hpp>

namespace ultraloc
{

/**
 * @brief Non-owning view of a single-channel camera image.
 */
struct ImageView
{
  const std::uint8_t *data   = nullptr;
  int                 width  = 0;
  int                 height = 0;
  int                 stride = 0;  // bytes per row
  Timestamp           stamp;
};

/**
 * @brief Optical measurement source (UVDAR).
 *
 * Consumes a stream of camera images and produces bearings towards identified targets.
 * One instance is created per camera.
 */
class BearingSource {
public:
  virtual ~BearingSource() = default;

  /**
   * @brief Processes the next image of the camera stream.
   *
   * Images must be supplied in order and at the camera's nominal frame rate.
   *
   * @param image the next camera image
   *
   * @return bearings towards all targets currently identified in the camera's field of view
   */
  virtual std::vector<Bearing> processImage(const ImageView &image) = 0;

  /**
   * @brief Resets the internal state, e.g., after a gap in the image stream.
   */
  virtual void reset() = 0;
};

}  // namespace ultraloc
