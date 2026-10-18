# autonomy_interfaces

Plugin interfaces of the onboard autonomy core. These are the extension points
through which feedback controllers, reference trackers, state estimators and
hardware back ends are added to the system without modifying the core.

| Interface | Header | Purpose |
|---|---|---|
| Controller | [`mrs_uav_managers/controller.h`](mrs_uav_managers/include/mrs_uav_managers/controller.h) | Feedback control: turns the desired reference and the current state into a command for the flight controller. |
| Tracker | [`mrs_uav_managers/tracker.h`](mrs_uav_managers/include/mrs_uav_managers/tracker.h) | Reference generation: turns goals and trajectories into a smooth, feasible reference for the controller. |
| State estimator | [`mrs_uav_managers/state_estimator.h`](mrs_uav_managers/include/mrs_uav_managers/state_estimator.h) | Estimates the UAV state from the available sensors. |
| AGL estimator | [`mrs_uav_managers/agl_estimator.h`](mrs_uav_managers/include/mrs_uav_managers/agl_estimator.h) | Estimates height above ground level. |
| Hardware API | [`mrs_uav_hw_api/api.h`](mrs_uav_hw_api/include/mrs_uav_hw_api/api.h) | Abstraction over the flight controller or simulator: sensor data in, control commands out, at whichever control level the hardware supports. |

The remaining headers are the shared types and handlers that these interfaces
depend on.

## Origin

These headers are copied unmodified from the open-source
[MRS UAV System](https://github.com/ctu-mrs/mrs_uav_system) (ROS 2 branch),
developed by the [Multi-Robot Systems group](http://mrs.felk.cvut.cz/) at the
Czech Technical University in Prague:

| Upstream repository | Commit |
|---|---|
| [ctu-mrs/mrs_uav_managers](https://github.com/ctu-mrs/mrs_uav_managers/tree/ros2) | `7f6d6195d035d9b88706098f37aa1d54c88d4e05` |
| [ctu-mrs/mrs_uav_hw_api](https://github.com/ctu-mrs/mrs_uav_hw_api/tree/ros2) | `222b7b615fc491d2ba07abbe62d740903402c767` |

They are distributed under the BSD 3-Clause License, see [LICENSE](LICENSE).

This directory is a reference, not a buildable package. The headers depend on
`mrs_lib` and `mrs_msgs`, which are part of the MRS UAV System.
