---
name: readme-style-system
description: README styling convention across the ws_px4_work PX4 control-stack repos (controllers vs support packages); invisible em-space nav-button gotcha
metadata: 
  node_type: memory
  type: reference
  originSessionId: 51fed24e-7b2a-4d90-ade9-e1192cdd50ac
---

`nmpc_acados_px4/README.md` is the canonical style template for this workspace. Two README tiers:

- **Controllers** (`nmpc_acados_px4`, `newton_raphson_px4`, geometric_px4*): full chrome — badge row, a centered `<div align="center">` nav bar of `<kbd>` "key cap" buttons, and an open `<details>` Table of Contents. Section order ends: … → Hardware with Motion Capture → Papers and Repositories → (### Related Work) → Website → License.
- **Support packages** (`quad_trajectories`, `quad_platforms`, `ros2_logger`): NO nav bar. Rich badge row (Part of: PX4-ROS2 Control Stack | Algorithms: NMPC | NR | Geometric | Languages: Python | C++ | Docker: PX4-ROS2-Docker | License: MIT), a **collapsed minimal** `<details>` TOC (top-level sections only), and a `## Used by` section linking the Python + C++ controllers, the package's `_cpp` counterpart, the Docker repo, and the evannsmc.com/projects portfolio.

**GOTCHA (caused the "Mocap not centered" bug):** every `<kbd>` nav button pads its label with `U+2003` EM SPACE (bytes `e2 80 83`), NOT ASCII `0x20`. ASCII spaces collapse in HTML → skinny, misaligned button. When editing/adding a nav button, copy the em-spaces from a sibling button or insert ` ` programmatically; never type a normal space. Verify with `cat -A` (`M-bM-^@M-^C` = em-space).

Badge `message` encoding (shields.io): `|`→`%7C`, `++`→`%2B%2B`, literal dash in a label→`--`.

See [[px4-stack-git-workflow]] for how README edits in these submodules get committed/pushed.
