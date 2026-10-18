# ultraloc_interfaces

Public interfaces of the ULTRALOC relative localization system. They describe
how ULTRALOC is split into components and what goes in and out of each one,
without the implementations.

## Pipeline

```
camera 1 ──► BearingSource ──┐
camera N ──► BearingSource ──┼──► RelativeLocalizer ──► RelativePosition (per target)
UWB      ──► RangeSource ────┘
```

| Component | Header | Input | Output |
|---|---|---|---|
| `BearingSource` | [`bearing_source.hpp`](include/ultraloc/bearing_source.hpp) | UV camera image stream (one instance per camera) | `Bearing`: unit direction towards each identified target |
| `RangeSource` | [`range_source.hpp`](include/ultraloc/range_source.hpp) | UWB radio | `Range`: distance to each identified target |
| `RelativeLocalizer` | [`relative_localizer.hpp`](include/ultraloc/relative_localizer.hpp) | bearings and ranges, asynchronous | `RelativePosition`: 3D position and covariance of each target |

The shared data types are defined in [`types.hpp`](include/ultraloc/types.hpp).
The C++ interfaces are header-only and depend on ROS 2 only for the messages.

## ROS 2 messages

| Message | Content |
|---|---|
| [`Bearing`](msg/Bearing.msg) | target id, unit direction in the sensor frame |
| [`Range`](msg/Range.msg) | target id, distance |
| [`RelativePosition`](msg/RelativePosition.msg) | target id, position, covariance |
| [`RelativePositionArray`](msg/RelativePositionArray.msg) | all currently localized targets |

## Building

A standard ROS 2 (Jazzy) package:

```bash
colcon build --packages-select ultraloc_interfaces
```

## License

Proprietary, all rights reserved. See [LICENSE](LICENSE). For licensing,
contact [info@fly4future.com](mailto:info@fly4future.com).
