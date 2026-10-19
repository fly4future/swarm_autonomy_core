# Relative Robot Autonomy

Digest of the relative robot autonomy software at [Fly4Future](https://github.com/fly4future),
and the starting point of the project **Intent-Aware Sensor for Agile, Dense
Robot Teams**.

This repository describes what the project aims to build, and the technology it
builds on: what the existing building blocks are, what each one does, and how
they fit together. The implementations live in separate, private repositories,
see [Software repositories](#software-repositories).

## Project focus: Intent-Aware Sensor for Agile, Dense Robot Teams

Our smart sensor will let any mobile manipulator, ground robot or humanoid
perceive the robots and people around it as **fully dynamic agents**, without
central communication or external localization. It will sense their identity,
position, velocity, acceleration, jerk and snap, and estimate their **intent**
using learned intent-prediction models, so robots can coexist and collaborate
much faster in a decentralized manner in unstructured environments.

The sensor fuses various advanced sensing modalities to sense even through
walls and vegetation, in bright light, darkness, smoke and clutter, indoors and
outdoors. This enables agile, dense, decentralized robot teams for factories,
warehouses, ports and cities, aiming far beyond the speed, scale and
reliability of today's decentralized systems.

### What the sensor will provide

| Output | Description |
|---|---|
| **Identity** | Which robot or person each perceived agent is. |
| **Full motion state** | Position, velocity, acceleration, jerk and snap of every agent, relative to the host robot. |
| **Intent** | Where each agent is going and what it is about to do, estimated by learned intent-prediction models. |

### Design principles

- **Fully decentralized**: no central communication, no external localization,
  no infrastructure. Every robot perceives its neighbours with its own sensor.
- **Platform-agnostic**: one sensor for mobile manipulators, ground robots and
  humanoids, perceiving both robots and people.
- **Robust in any conditions**: multimodal fusion that keeps working through
  walls and vegetation, in bright light, darkness, smoke and clutter, indoors
  and outdoors.
- **Built for density and agility**: perception fast and reliable enough for
  many robots moving quickly and closely together.

```
                ┌──────────────────────── intent-aware sensor ────────────────────────┐
  robots and    │                                                                     │
  people around │  multiple sensing ──► fusion & tracking ──► full motion state ──┐   │
  ─────────────►│  modalities             (identity)          (pos … snap)        │   │
                │                                                                 ▼   │
                │                                              learned intent prediction
                └─────────────────────────────────────────────────────────────────┬───┘
                                                                                  ▼
                                         host robot: planning, control, collaboration
```

### From today's technology to the sensor

The project builds on our existing, field-proven relative localization system
ULTRALOC, described below.

| | Today: ULTRALOC | Target: intent-aware sensor |
|---|---|---|
| **Host platforms** | Our own robot platforms | Any mobile manipulator, ground robot or humanoid |
| **Perceived agents** | Cooperating robots | Robots and people |
| **Sensing** | UV optical markers and UWB radio ranging | Fusion of various advanced sensing modalities, through walls and vegetation, in bright light, darkness, smoke and clutter |
| **Output** | Identity and 3D relative position with covariance | Identity, position, velocity, acceleration, jerk, snap, and intent |
| **Infrastructure** | None | None |

## Existing foundation

The rest of this document describes the technology the project starts from.

Moving many robots close together needs two things: every robot has to move
safely on its own, and every robot has to know where its neighbours are,
including in places where GNSS is unavailable, unreliable, or not accurate
enough for close formation motion.

Our stack covers both:

| Layer | What it does |
|---|---|
| **Multi-sensor fusion and state estimation** | Moves each robot on its own: state estimation, control, trajectory following, motion management, safety. |
| **ULTRALOC relative localization** | Measures where neighbouring robots (and marked points in the environment) are, relative to the robot itself, using onboard sensors only. |
| **ULTRALOC hardware** | A small onboard board that provides the radio ranging and drives the optical markers. |
| **Deployment tooling** | Ready-to-run, containerised configurations for different cameras and compute platforms. |

```
            ┌───────────────────────────── one robot ─────────────────────────────┐
            │                                                                     │
 neighbours │  UV cameras ──► optical detection & tracking ──┐                    │
 ──────────►│                                                ├──► fusion ──► relative positions of neighbours
            │  ULTRALOC board ──► radio ranging ─────────────┘                    │        │
            │        │                                                            │        ▼
            │        └──► UV marker (seen by the others)          multi-sensor fusion and state estimation
            │                                                     (control, planning, swarm behaviour)
            └─────────────────────────────────────────────────────────────────────┘
```

## Multi-sensor fusion and state estimation

Every robot runs our multi-sensor fusion and state estimation framework, which
handles:

- hardware abstraction (one interface over different motion controllers and simulators),
- state estimation,
- feedback control,
- reference and trajectory generation,
- motion management and safety supervision,
- plugin interfaces, so new controllers, trackers, estimators and hardware back ends can be added without changing the core,
- a robot simulator for testing before real-world motion.

The framework is built on ROS 2 and is based on an open-source framework.

## ULTRALOC: relative localization

ULTRALOC tells each robot where its neighbours are, relative to itself. It needs
no external infrastructure: no GNSS, no motion capture, no ground beacons. It
combines two sensing methods whose strengths and weaknesses complement each other.

### UVDAR: optical direction finding

- Every robot carries **ultraviolet LED markers** that blink in a pattern unique to that robot.
- UV-sensitive cameras on the other robots see these markers. Working in the
  UV band suppresses most of the background, which makes detection robust in
  cluttered scenes and outdoors.
- Onboard software finds the markers in each camera frame, follows them over
  time, and works out from the blinking which robot each marker belongs to.
- The result is the **direction (bearing)** from the camera to each identified neighbour.
- Any number of cameras is supported, which allows full surround coverage, and
  wide-angle and fisheye lenses are supported.
- Image processing runs in real time on the robot's computer and can be
  accelerated on a GPU where one is available.

### UWB: radio ranging

- Each robot carries an **ultra-wideband (UWB)** radio that measures its
  **distance** to the other robots' radios.
- Ranging works in a network of many nodes, without central coordination.
- UWB does not need a line of sight in the same way the cameras do, so it keeps
  working when a neighbour is briefly out of view.

### Fusion

Each method has a weakness on its own. A camera gives a precise direction but
no reliable distance, and a radio gives a distance but no direction. ULTRALOC
pairs each neighbour's optical identity with its radio identity and combines
both measurements in a **probabilistic estimator**. The output is a continuous
**3D relative position** for every neighbour, and the estimator keeps running
when one of the two sensors briefly drops out.

## ULTRALOC hardware

A compact embedded board, built in-house, that provides:

- the UWB radio and its ranging protocol,
- control of the UV marker blinking, synchronised with the robot's identity,
- a plug-and-play USB connection to the robot's computer, plus a serial link,
- simple configuration and status reporting over USB, with no special tools needed.

## Deployment

- Containerised (Docker) images for the whole stack, in development and
  binary-only release variants.
- Reference configurations for several camera families and compute targets
  (CPU-only and NVIDIA GPU).
- A camera simulator for developing and testing without real robot runs.
- Tooling for camera calibration and session management on the robot.

## Software repositories

The software itself is kept in the following private repositories. Access is
available on request, see [Contact](#contact).

| Repository | Content |
|---|---|
| [Multi-sensor fusion and state estimation](https://github.com/fly4future/f4f_uav_core) | Framework core: state estimation, control, trajectory following, motion management, safety. |
| [fly4future/ultraloc](https://github.com/fly4future/ultraloc) | ULTRALOC relative localization: optical detection and tracking, UWB driver, fusion. |
| [fly4future/uvdar_firmware](https://github.com/fly4future/uvdar_firmware) | Firmware of the ULTRALOC board: UWB ranging, UV marker control, USB configuration. |
| [fly4future/ultraloc_deployment](https://github.com/fly4future/ultraloc_deployment) | Containerised deployment and reference configurations for cameras and compute targets. |
| [fly4future/uvdar_ros_fusion](https://github.com/fly4future/uvdar_ros_fusion) | Earlier UWB and UVDAR fusion, superseded by ULTRALOC. |

## Repository contents

| Directory | Content |
|---|---|
| [`ultraloc_interfaces/`](ultraloc_interfaces) | Public C++ interfaces and ROS 2 messages of ULTRALOC: bearing sources, range sources and the relative localizer. |

## Publications

Publications on the optical relative localization method used in ULTRALOC:

- V. Walter, N. Staub, A. Franchi, M. Saska. *UVDAR System for Visual Relative
  Localization With Application to Leader–Follower Formations of Multirotor UAVs.*
  IEEE Robotics and Automation Letters 4(3), 2019.
  [doi:10.1109/LRA.2019.2901683](https://doi.org/10.1109/LRA.2019.2901683)
- V. Walter, M. Saska, A. Franchi. *Fast Mutual Relative Localization of UAVs
  Using Ultraviolet LED Markers.* ICUAS 2018.
- V. Walter, N. Staub, M. Saska, A. Franchi. *Mutual Localization of UAVs Based
  on Blinking Ultraviolet Markers and 3D Time-Position Hough Transform.* CASE 2018.
- V. Walter, M. Vrba, M. Saska. *On Training Datasets for Machine Learning-Based
  Visual Relative Localization of Micro-Scale UAVs.* ICRA 2020.
- T. Lakemann, D. Bonilla Licea, V. Walter, T. Báča, M. Saska. *Towards Agile
  Multi-Robot Systems in the Real World: Fast Onboard Tracking of Active Blinking
  Markers for Relative Localization.* Robotics and Autonomous Systems 194, 2025.
  [doi:10.1016/j.robot.2025.105175](https://doi.org/10.1016/j.robot.2025.105175)

## License

Proprietary, © Fly4Future s.r.o., all rights reserved. See [LICENSE](LICENSE).

## Contact

For collaboration, licensing or access to the technology, contact
[info@fly4future.com](mailto:info@fly4future.com).
