# swarm_autonomy_core

Digest of the swarm autonomy software at [fly4future](https://github.com/fly4future).

This repository gives an overview of the technology behind our autonomous
multi-UAV systems: what the building blocks are, what each one does, and how
they fit together. The implementations live in separate, private repositories.

## Overview

Flying many drones close together needs two things: every drone has to fly
safely on its own, and every drone has to know where its neighbours are,
including in places where GNSS is unavailable, unreliable, or not accurate
enough for close formation flight.

Our stack covers both:

| Layer | What it does |
|---|---|
| **Onboard autonomy** | Flies each drone on its own: state estimation, control, trajectory following, flight management, safety. |
| **ULTRALOC relative localization** | Measures where neighbouring drones (and marked points on the ground) are, relative to the drone itself, using onboard sensors only. |
| **ULTRALOC hardware** | A small onboard board that provides the radio ranging and drives the optical markers. |
| **Deployment tooling** | Ready-to-run, containerised configurations for different cameras and compute platforms. |

```
            ┌───────────────────────────── one drone ─────────────────────────────┐
            │                                                                     │
 neighbours │  UV cameras ──► optical detection & tracking ──┐                    │
 ──────────►│                                                ├──► fusion ──► relative positions of neighbours
            │  ULTRALOC board ──► radio ranging ─────────────┘                    │        │
            │        │                                                            │        ▼
            │        └──► UV marker (seen by the others)          onboard autonomy (estimation, control,
            │                                                     planning, swarm behaviour)
            └─────────────────────────────────────────────────────────────────────┘
```

## Onboard autonomy

Every drone runs a complete autonomy stack that handles:

- hardware abstraction (one interface over different flight controllers and simulators),
- state estimation,
- feedback control,
- reference and trajectory generation,
- flight management, takeoff and landing, and safety supervision,
- plugin interfaces, so new controllers, trackers, estimators and hardware back ends can be added without changing the core,
- a multirotor simulator for testing before flight.

The core is based on the open-source
[MRS UAV System](https://github.com/ctu-mrs/mrs_uav_system) (ROS 2), developed
by the [Multi-Robot Systems group](http://mrs.felk.cvut.cz/) at the Czech
Technical University in Prague. We keep our own maintained fork, adapted for our
platforms and deployments.

## ULTRALOC: relative localization

ULTRALOC tells each drone where its neighbours are, relative to itself. It needs
no external infrastructure: no GNSS, no motion capture, no ground beacons. It
combines two sensing methods whose strengths and weaknesses complement each other.

### UVDAR: optical direction finding

- Every drone carries **ultraviolet LED markers** that blink in a pattern unique to that drone.
- UV-sensitive cameras on the other drones see these markers. Working in the
  UV band suppresses most of the background, which makes detection robust in
  cluttered scenes and outdoors.
- Onboard software finds the markers in each camera frame, follows them over
  time, and works out from the blinking which drone each marker belongs to.
- The result is the **direction (bearing)** from the camera to each identified neighbour.
- Any number of cameras is supported, which allows full surround coverage, and
  wide-angle and fisheye lenses are supported.
- Image processing runs in real time on the drone's computer and can be
  accelerated on a GPU where one is available.

UVDAR originates from research at the MRS group, CTU Prague. See
[Publications](#publications).

### UWB: radio ranging

- Each drone carries an **ultra-wideband (UWB)** radio that measures its
  **distance** to the other drones' radios.
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
- control of the UV marker blinking, synchronised with the drone's identity,
- a plug-and-play USB connection to the drone's computer, plus a serial link,
- simple configuration and status reporting over USB, with no special tools needed.

## Deployment

- Containerised (Docker) images for the whole stack, in development and
  binary-only release variants.
- Reference configurations for several camera families and compute targets
  (CPU-only and NVIDIA GPU).
- A camera simulator for developing and testing without real flights.
- Tooling for camera calibration and session management on the drone.

## Repository contents

| Directory | Content |
|---|---|
| [`autonomy_interfaces/`](autonomy_interfaces) | Plugin interfaces of the onboard autonomy core (controllers, trackers, state estimators, hardware API), taken from the MRS UAV System. |
| [`ultraloc_interfaces/`](ultraloc_interfaces) | Public C++ interfaces and ROS 2 messages of ULTRALOC: bearing sources, range sources and the relative localizer. |

## Publications

The scientific foundations of the optical part of ULTRALOC:

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

- [`ultraloc_interfaces/`](ultraloc_interfaces) and the rest of this repository: proprietary, all rights reserved, see [LICENSE](LICENSE).
- [`autonomy_interfaces/`](autonomy_interfaces): BSD 3-Clause, © Multi-robot Systems group, CTU Prague, see [LICENSE](autonomy_interfaces/LICENSE).

## Acknowledgements

This work builds on many years of research and open-source software from the
[Multi-Robot Systems group](http://mrs.felk.cvut.cz/) at the Czech Technical
University in Prague.

## Contact

For collaboration, licensing or access to the technology, contact
[info@fly4future.com](mailto:info@fly4future.com).
