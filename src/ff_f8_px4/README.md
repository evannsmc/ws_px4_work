# Feedforward Figure-8 Controller for PX4-ROS2

![Status](https://img.shields.io/badge/Status-Hardware_Validated-blue)
[![ROS 2 Compatible](https://img.shields.io/badge/ROS%202-Humble_%7C_Jazzy-blue)](https://docs.ros.org/)
[![PX4 Compatible](https://img.shields.io/badge/PX4-Autopilot-pink)](https://github.com/PX4/PX4-Autopilot)
[![Docker: PX4-ROS2-Docker](https://img.shields.io/badge/Docker-PX4--ROS2--Docker-2496ED?logo=docker&logoColor=white)](https://github.com/evannsmc/PX4-ROS2-Docker)
![Control](https://img.shields.io/badge/Control-Feedforward_(flatness)-brightgreen)

A ROS 2 controller for the `fig8_contraction` trajectory that publishes body-rate and thrust commands derived purely from **differential-flatness feedforward**. It is the open-loop baseline of the [evannsmc PX4-ROS2 control stack](https://www.evannsmc.com/projects): it inverts the flat output of the figure-8 directly into `[throttle, p, q, r]`, with no feedback from odometry by default.

Three layers can be combined:

- **flatness feedforward** for the nominal `fig8_contraction` motion (always on),
- an optional **startup ramp** (`--ramp-seconds`) so the controller does not jump instantly from hover to the moving trajectory,
- an optional **light feedback layer** (`--p-feedback`) that adds position, velocity, attitude, and body-rate damping.

<div align="center">

---

**[<kbd> <br> Build <br> </kbd>](#build)** 
**[<kbd> <br> Approach <br> </kbd>](#how-it-works)** 
**[<kbd> <br> Modes <br> </kbd>](#modes)** 
**[<kbd> <br> Usage <br> </kbd>](#usage)** 
**[<kbd> <br> Arguments <br> </kbd>](#arguments)** 
**[<kbd> <br> Logs <br> </kbd>](#log-filenames)** 

---

</div>

<details open>
<summary><b>📖 Table of Contents</b></summary>

- [Modes](#modes)
- [How It Works](#how-it-works)
- [Build](#build)
- [Usage](#usage)
  - [Command Matrix](#command-matrix)
- [Arguments](#arguments)
- [Practical Guidance](#practical-guidance)
- [Log Filenames](#log-filenames)
- [Dependencies](#dependencies)
- [Package Structure](#package-structure)
- [Technical Note](#technical-note)
- [License](#license)

</details>

## Modes

- **Default** — pure open-loop feedforward
- **`--p-feedback`** — feedforward plus light proportional position / attitude correction
- **`--ramp-seconds`** — startup blend from hover commands into feedforward (default `2.0`)

## How It Works

At each control tick the node evaluates the `fig8_contraction` flat output and runs `flat_to_x_u(...)` from `quad_trajectories`. That returns:

- `x_ff = [px, py, pz, vx, vy, vz, f_specific, phi, th, psi]`
- `u_ff = [df, dphi, dth, dpsi]`

The controller then:

1. Converts `u_ff[1:4]` from Euler-angle rates into body rates `[p, q, r]`.
2. Converts `x_ff[6]` from specific thrust to force with `force = mass * f_specific`.
3. Converts force into PX4 throttle with the platform-specific thrust curve.
4. Publishes `VehicleRatesSetpoint` with `[throttle, p, q, r]`.

When `--p-feedback` is enabled, the node adds small corrections on top of the flatness command:

- XY position and velocity error adjust desired roll/pitch
- Z position and velocity error adjust collective thrust
- yaw error adjusts yaw rate
- measured body rates are damped directly before publishing

When `--ramp-seconds > 0`, the controller also does two startup-smoothing steps:

- it time-warps the trajectory at the start so reference speed rises gradually
- it blends commanded inputs from hover `[hover thrust, 0, 0, 0]` into the feedforward command

This is important because the vehicle begins from hover, while the figure-8 reference implies nonzero velocity and nonzero angular-rate demand almost immediately.

## Build

```bash
cd ~/ws_px4_work
colcon build --packages-select quad_platforms quad_trajectories ff_f8_px4
source install/setup.bash
```

## Usage

```bash
# Pure feedforward, default ramp
ros2 run ff_f8_px4 run_node --platform sim --log

# Pure feedforward, no ramp
ros2 run ff_f8_px4 run_node --platform sim --ramp-seconds 0 --log

# Feedforward with light proportional feedback, default ramp
ros2 run ff_f8_px4 run_node --platform sim --p-feedback --log

# Feedforward with light proportional feedback, longer ramp
ros2 run ff_f8_px4 run_node --platform sim --p-feedback --ramp-seconds 4.0 --log

# Double speed variants
ros2 run ff_f8_px4 run_node --platform sim --double-speed --log
ros2 run ff_f8_px4 run_node --platform sim --double-speed --p-feedback --log

# Hardware
ros2 run ff_f8_px4 run_node --platform hw --p-feedback --log
```

### Command Matrix

**Simulation**

```bash
# 1x, pure ff
ros2 run ff_f8_px4 run_node --platform sim --log
ros2 run ff_f8_px4 run_node --platform sim --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform sim --ramp-seconds 0 --log

# 1x, ff + p-feedback
ros2 run ff_f8_px4 run_node --platform sim --p-feedback --log
ros2 run ff_f8_px4 run_node --platform sim --p-feedback --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform sim --p-feedback --ramp-seconds 0 --log

# 2x, pure ff
ros2 run ff_f8_px4 run_node --platform sim --double-speed --log
ros2 run ff_f8_px4 run_node --platform sim --double-speed --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform sim --double-speed --ramp-seconds 0 --log

# 2x, ff + p-feedback
ros2 run ff_f8_px4 run_node --platform sim --double-speed --p-feedback --log
ros2 run ff_f8_px4 run_node --platform sim --double-speed --p-feedback --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform sim --double-speed --p-feedback --ramp-seconds 0 --log
```

**Hardware**

```bash
# 1x, pure ff
ros2 run ff_f8_px4 run_node --platform hw --log
ros2 run ff_f8_px4 run_node --platform hw --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform hw --ramp-seconds 0 --log

# 1x, ff + p-feedback
ros2 run ff_f8_px4 run_node --platform hw --p-feedback --log
ros2 run ff_f8_px4 run_node --platform hw --p-feedback --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform hw --p-feedback --ramp-seconds 0 --log

# 2x, pure ff
ros2 run ff_f8_px4 run_node --platform hw --double-speed --log
ros2 run ff_f8_px4 run_node --platform hw --double-speed --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform hw --double-speed --ramp-seconds 0 --log

# 2x, ff + p-feedback
ros2 run ff_f8_px4 run_node --platform hw --double-speed --p-feedback --log
ros2 run ff_f8_px4 run_node --platform hw --double-speed --p-feedback --ramp-seconds 4.0 --log
ros2 run ff_f8_px4 run_node --platform hw --double-speed --p-feedback --ramp-seconds 0 --log
```

## Arguments

| Argument | Description |
|----------|-------------|
| `--platform {sim,hw}` | Platform type |
| `--double-speed` | Mark log filename with `_2x` (trajectory period remains fixed at 10s) |
| `--p-feedback` | Add light proportional position / attitude feedback |
| `--ramp-seconds` | Startup blend duration in seconds; `0` disables the ramp |
| `--log` | Enable logging |
| `--log-file NAME` | Custom log filename |
| `--flight-period SECONDS` | Override default flight duration |

## Practical Guidance

- `pure ff` is mainly useful as a baseline comparison and is sensitive to model mismatch.
- `--p-feedback` is the recommended mode if you want the controller to actually track the trajectory reasonably.
- increasing `--ramp-seconds` reduces the startup discontinuity when switching from hover to figure-8 flight.
- the hover / return position is the actual first `fig8_contraction` reference point, not a generic hardcoded hover point.
- logs are saved under `src/data_analysis/log_files/ff_f8_px4/`.

## Log Filenames

```text
{platform}_ff_f8[_pfb][_rampXs]_{speed}.csv
```

Examples:

- `sim_ff_f8_ramp2p0s_1x.csv`
- `sim_ff_f8_pfb_ramp2p0s_1x.csv`
- `hw_ff_f8_pfb_ramp2p0s_2x.csv`

## Dependencies

- [quad_trajectories](https://github.com/evannsmc/quad_trajectories) — provides `fig8_contraction` and `flat_to_x_u`
- [quad_platforms](https://github.com/evannsmc/quad_platforms) — mass and thrust-throttle conversion
- [ros2_logger](https://github.com/evannsmc/ROS2Logger) — CSV experiment logging
- [px4_msgs](https://github.com/evannsmc/px4_msgs/tree/v1.16_minimal_msgs) — PX4 ROS 2 message definitions
- JAX — flat-output differentiation

## Package Structure

```
ff_f8_px4/
├── ff_f8_px4/
│   ├── run_node.py        # CLI entry point and argument parsing
│   └── ros2px4_node.py    # ROS 2 node (feedforward control loop, ramp, optional feedback)
├── docs/
│   └── ff_f8_controller.qmd   # detailed derivation (rendered to PDF)
├── package.xml
└── setup.py
```

## Technical Note

A more detailed derivation is available in:

- `docs/ff_f8_controller.qmd`
- rendered PDF: `docs/ff_f8_controller.pdf`

## License

MIT
