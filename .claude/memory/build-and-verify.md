---
name: build-and-verify
description: How to build/verify the ws_px4_work stack — Docker is the real build path; zsh aliases; host gaps
metadata:
  type: reference
---

**Canonical build path = Docker.** A prebuilt image **`px4_controllers_jazzy:latest`** ships ROS 2 Jazzy + colcon + JAX + acados_template + a prebuilt `px4_msgs` overlay (`/opt/ws_px4_msgs/install`). A `bash -lc` login shell in it auto-sources ROS + px4_msgs. Verified recipe (packages live under `src/` — see [[px4-stack-git-workflow]]):

```
docker run --rm -v /home/egmc/ws_px4_work:/workspace px4_controllers_jazzy:latest bash -lc \
  'cd /workspace && colcon build --symlink-install --packages-up-to <pkg> --cmake-args -DCMAKE_BUILD_TYPE=Release'
```

- **Image-name mismatch:** the `src/makefile` uses `IMAGE_NAME=px4_ros2_jazzy`, but the actual prebuilt image is `px4_controllers_jazzy`. So `make run` as-is won't find it — drive `docker run` directly (above) or retag/fix the makefile.
- **Root-owned artifacts gotcha:** the container runs as root, so `build/ install/ log/` it writes to the mounted host dir are **root-owned** → can't `rm` without sudo. Clean them from a container instead: `docker run --rm -v <ws>:/workspace px4_controllers_jazzy:latest rm -rf /workspace/build /workspace/install /workspace/log`.
- **acados C++ controller** (`nmpc_acados_px4_cpp`) needs its solver C-code generated first: `cd /workspace && python3 src/nmpc_acados_px4/ensure_solver.py --platform sim` (CMake errors otherwise). This is a standing prereq, unrelated to source edits.
- Build results (2026-06, fig8_akash rename verification): all Python pkgs + `quad_trajectories_cpp` + `newton_raphson_px4_cpp` + `geometric_px4_cpp` compiled clean in-container.

**zsh aliases** (`~/.zshrc`): `jazz`=`source /opt/ros/jazzy/setup.zsh`; `cap`=`source /home/egmc/ws_mocap_px4_msgs_drivers/install/setup.zsh` (mocap overlay → px4_msgs); `cb`/`cbp`=colcon build variants; `juggle`/`vicon`/`relay`/`full`=mocap launches. NOTE: zsh does NOT word-split unquoted `$VARS` — list items inline in `for` loops or use `${=VAR}`.

**Host (outside Docker) is a partial post-machine-switch setup:** missing `jax`, `acados_template`, the `ws_mocap_px4_msgs_drivers` workspace (so `cap` fails), and `/opt/ros/jazzy` is minimal. So build ON HOST is not viable — use Docker. `px4_msgs` source also exists at `/home/egmc/MoralesCuadrado_Baird_TCST2026/src/px4_msgs`.

**ROS badges:** READMEs now carry a **dual `ROS 2 Humble | Jazzy`** badge (machine runs Jazzy; stack historically Humble) per user decision.
