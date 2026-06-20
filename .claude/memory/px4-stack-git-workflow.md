---
name: px4-stack-git-workflow
description: "How to land edits in ws_px4_work submodules (detached HEAD, direct-to-main, push, bump parent pointers) and the session auto-save hook"
metadata: 
  node_type: memory
  type: project
  originSessionId: 51fed24e-7b2a-4d90-ade9-e1192cdd50ac
---

`ws_px4_work` is a parent repo (`git@github.com:evannsmc/ws_px4_work.git`) of 17 submodules, all personal `evannsmc` GitHub repos.

**LAYOUT (changed 2026-06):** all packages live under **`src/`** — `src/<pkg>`, plus plain dirs `src/ff_f8_px4`, `src/offboard_cpp`, `src/data_analysis`. The Docker infra is now the **`src/PX4-ROS2-Docker` submodule** (canonical source of truth; the old redundant `src/docker/` + `src/makefile` were removed). Root keeps only `README.md`, `plan.md`, `.gitmodules`, `.gitignore`, `.claude/`, `.vscode/`.

Workflow when editing a submodule's tracked files (paths are now `src/<sub>`):

1. Submodules check out in **detached HEAD** at the `main` tip. Before committing, `git -C src/<sub> checkout main` (safe no-op since HEAD==main, preserves working-tree edits).
2. Commit specific files, then `git -C src/<sub> push origin main`.
3. In the parent, `git add src/<sub>` (only the changed submodules — not `plan.md`) and commit a "Bump … submodules" pointer-update, then `git push origin main`.

The user works **direct-to-main** on these personal repos (no PR flow) and authorizes commit+push for doc work.

**Session auto-save hook:** a hook periodically auto-commits `plan.md` and submodule-pointer changes to the parent (e.g. `chore: session auto-save — …`) and may push them. So expect `plan.md` to already be tracked/committed and the parent to be ahead of origin between manual commits. `plan.md` is the user's mandated planning file (see CLAUDE.md) and is intentionally tracked.

Repo-name vs package-name quirk: `ROS2Logger` (GitHub repo) installs the package `ros2_logger`; its C++ counterpart repo is `ROS2Logger_cpp`. See [[readme-style-system]].

**GOTCHA — not every package dir is a submodule.** `src/ff_f8_px4`, `src/offboard_cpp`, and `src/data_analysis` are plain dirs tracked directly in the PARENT repo (absent from `.gitmodules`). So `git -C src/ff_f8_px4 add -u && git -C src/ff_f8_px4 commit` resolves to the PARENT `.git` and will sweep parent changes into one mislabeled commit. Commit these dirs' changes directly in the parent. Verify membership with `.gitmodules` before using `git -C <dir>`. (Docker infra is the `src/PX4-ROS2-Docker` submodule now.)

**GOTCHA — the parent can pin submodules BEHIND their `origin/main`.** Audited (2026-06): `quad_trajectories_cpp` (+6), `newton_raphson_px4_cpp` (+5), `quad_platforms_cpp` (+2) were pinned behind, and the newer commits carried behavioral changes (gravity 9.8, trajectory reshaping, battery-logic removal) + an upstream `f8_contraction`→`fig8_contraction` rename. Before any cross-cutting edit, run a freshness audit (`HEAD..origin/main` per submodule) and ASK before pulling — pulling behavioral deltas can invalidate the user's pinned/reproducible setup.

Trajectory naming: the figure-eight trajectory is **`fig8_contraction`** (enum `FIG8_CONTRACTION`, fn `fig8_contraction`), unified across Python + C++ (the C++ `f8_contraction` form is gone). It was briefly renamed to `fig8_akash` mid-session, then reverted to `fig8_contraction` per the user. Do NOT confuse it with the separate `*_contraction` trajectory family (`hover_contraction`, `fig8_heading_contraction`, `spiral_contraction`, `trefoil_contraction`) — those are different trajectories, never renamed. The trajectory was written for the user's **[contraction_controller_px4](https://github.com/evannsmc/contraction_controller_px4)** repo; READMEs should link that repo when explaining it, NOT assume the reader knows "the contraction controller". README rule: write for a random external reader — reframe internal cross-references from the user's perspective with hyperlinks.
