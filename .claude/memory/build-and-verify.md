---
name: build-and-verify
description: How to build/verify the ws_px4_work stack; zsh aliases; what's missing on the current (post-machine-switch) machine
metadata:
  type: reference
---

**zsh aliases** (in `~/.zshrc`) for working with the stack:
- `jazz` → `source /opt/ros/jazzy/setup.zsh` (ROS 2 **Jazzy**)
- `cap` → `source /home/egmc/ws_mocap_px4_msgs_drivers/install/setup.zsh` (mocap workspace overlay → provides `px4_msgs`)
- `cb` / `cbp` → `colcon build` variants; `juggle` → plotjuggler; `vicon`/`relay`/`full` → mocap launches.

**ROS distro is Jazzy, but the README badges say "ROS 2 Humble"** — likely a stale badge across the stack. Confirm intended target with the user before mass-editing.

**Current machine is a partial setup** (post "machine switch" — see git history). As of 2026-06 it was MISSING: `jax`, `acados_template` (so Python controllers can't build/import), the `ws_mocap_px4_msgs_drivers` workspace entirely (so `cap` / px4_msgs overlay unavailable), and `/opt/ros/jazzy` is minimal (`ros2 pkg` not present). A full `colcon` build of the controllers must be done on the real dev machine. `px4_msgs` source exists at `/home/egmc/MoralesCuadrado_Baird_TCST2026/src/px4_msgs`.

**Lightweight verification that DOES work here** (used to verify the [[px4-stack-git-workflow]] fig8_akash rename):
- C++: `g++ -std=c++17 -I quad_trajectories_cpp/include -I /usr/include/eigen3 test.cpp` — `types.hpp` is standalone; `trajectories.hpp` needs only Eigen (autodiff only for the `<autodiff::real>` instantiations in registry.cpp/utils.cpp, which FetchContent autodiff from GitHub at configure time → needs network).
- Python: load a jax-free module standalone via `importlib.util.spec_from_file_location` (e.g. `types.py`) to bypass `__init__.py` (which pulls jax). `py_compile` works for syntax.
