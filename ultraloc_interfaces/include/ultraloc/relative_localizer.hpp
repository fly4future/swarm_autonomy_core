#pragma once

#include <optional>
#include <vector>

#include <ultraloc/types.hpp>

namespace ultraloc
{

/**
 * @brief Fuses bearings and ranges into relative positions of targets.
 *
 * Measurements may arrive from any number of bearing and range sources, at different rates and out of sync.
 * A target's estimate remains available while either measurement type is temporarily missing,
 * and is dropped when the target has not been observed for too long.
 */
class RelativeLocalizer {
public:
  virtual ~RelativeLocalizer() = default;

  virtual void addBearing(const Bearing &bearing) = 0;

  virtual void addRange(const Range &range) = 0;

  /**
   * @brief Returns the current estimate of a single target.
   *
   * @return the estimate, or std::nullopt if the target is not being localized
   */
  [[nodiscard]] virtual std::optional<RelativePosition> getEstimate(const TargetId id) const = 0;

  /**
   * @brief Returns the current estimates of all targets being localized.
   */
  [[nodiscard]] virtual std::vector<RelativePosition> getEstimates() const = 0;
};

}  // namespace ultraloc
