# ws_px4_work

ROS 2 workspace for quadrotor flight controllers and utilities targeting PX4-based platforms. All packages are included as git submodules.

## Cloning

```bash
git clone --recurse-submodules git@github.com:evannsmc/ws_px4_work.git
```

If already cloned without submodules:

```bash
git submodule update --init --recursive
```

## Submodules

### Controllers (Python)

| Package | Description |
|---------|-------------|
| [geometric_px4](https://github.com/evannsmc/geometric_px4) | Geometric attitude/position controller |
| [geometric_px4_flat](https://github.com/evannsmc/geometric_px4_flat) | Geometric controller using differential flatness |
| [geometric_px4_flip](https://github.com/evannsmc/geometric_px4_flip) | Geometric controller for aggressive flip maneuvers |
| [geometric_px4_quat](https://github.com/evannsmc/geometric_px4_quat) | Geometric controller with quaternion representation |
| [newton_raphson_px4](https://github.com/evannsmc/newton_raphson_px4) | Newton-Raphson based controller |
| [nmpc_acados_px4](https://github.com/evannsmc/nmpc_acados_px4) | Nonlinear MPC controller using ACADOS |

### Controllers (C++)

| Package | Description |
|---------|-------------|
| [geometric_px4_cpp](https://github.com/evannsmc/geometric_px4_cpp) | Geometric controller (C++) |
| [newton_raphson_px4_cpp](https://github.com/evannsmc/newton_raphson_px4_cpp) | Newton-Raphson based controller (C++) |

### Platform / Trajectory Utilities

| Package | Description |
|---------|-------------|
| [quad_platforms](https://github.com/evannsmc/quad_platforms) | Quadrotor platform definitions (Python) |
| [quad_platforms_cpp](https://github.com/evannsmc/quad_platforms_cpp) | Quadrotor platform definitions (C++) |
| [quad_trajectories](https://github.com/evannsmc/quad_trajectories) | Trajectory generators (Python) |
| [quad_trajectories_cpp](https://github.com/evannsmc/quad_trajectories_cpp) | Trajectory generators (C++) |

### Logging / Utilities

| Package | Description |
|---------|-------------|
| [ros2_logger](https://github.com/evannsmc/ROS2Logger) | ROS 2 data logger (Python) |
| [ros2_logger_cpp](https://github.com/evannsmc/ROS2Logger_cpp) | ROS 2 data logger (C++) |
| [testing_jacobian](https://github.com/evannsmc/jax_cpp_nr_speed_test) | JAX vs C++ speed benchmark for Jacobian computations |

### PX4 Interface

| Package | Description |
|---------|-------------|
| [px4_ros_com](https://github.com/PX4/px4_ros_com) | PX4-ROS 2 bridge (upstream PX4 package) |

### Docker

| Package | Description |
|---------|-------------|
| [PX4-ROS2-Docker](https://github.com/evannsmc/PX4-ROS2-Docker) | Dockerfile and requirements for the ROS 2 + JAX development container |

---

## Docker workflow

All `make` commands are run from `src/`:

```bash
cd ws_px4_work/src
make build       # build the Docker image (once, or after Dockerfile changes)
make run         # start the container
make build_ros   # build all ROS 2 packages inside the container
make attach      # open a shell inside the container
make stop        # stop the container
```

The `PX4-ROS2-Docker` submodule must be populated for these to work — use `--recurse-submodules` when cloning (see above).
