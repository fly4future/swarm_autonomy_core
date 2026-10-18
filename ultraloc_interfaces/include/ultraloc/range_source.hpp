#pragma once

#include <functional>

#include <ultraloc/types.hpp>

namespace ultraloc
{

/**
 * @brief Radio measurement source (UWB).
 *
 * Produces distance measurements towards identified targets asynchronously, as they become available.
 */
class RangeSource {
public:
  using RangeCallback = std::function<void(const Range &)>;

  virtual ~RangeSource() = default;

  /**
   * @brief Registers the function called for every new range measurement.
   */
  virtual void setCallback(RangeCallback callback) = 0;

  /**
   * @brief Starts producing measurements.
   *
   * @return true on success
   */
  virtual bool start() = 0;

  /**
   * @brief Stops producing measurements.
   */
  virtual void stop() = 0;
};

}  // namespace ultraloc
