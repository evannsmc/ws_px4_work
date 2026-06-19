# Plan: README cleanup across PX4 controller packages

Source of task: `nmpc_acados_px4/Agents.md`

## Findings (investigation complete)
- **Submodules up to date**: all 5 targets verified at `origin/main` HEAD (0 ahead / 0 behind) after `git fetch`. Step 1 of Agents.md is satisfied. ✓
- **"Mocap not centered" root cause**: nav buttons are `<kbd>` caps padded with `U+2003` EM SPACE (wide, non-collapsing). The **Mocap** button alone uses ASCII spaces, which HTML collapses → skinny, misaligned button. **Same bug in BOTH `nmpc_acados_px4` AND `newton_raphson_px4`.**
- **`ros2_logger/README.md` is broken**: contains unresolved (nested) git merge-conflict markers. Must resolve.
- **Library READMEs** (`quad_trajectories`, `quad_platforms`, `ros2_logger`) have no badges / nav bar / TOC — plain style, need to be brought up to nmpc style.
- **nmpc is the style template**; user only flagged Mocap for it → minimal change to nmpc.
- Out of scope (not in Agents.md list): the `_cpp` variants, geometric_px4*, px4_ros_com, testing_jacobian.

## Decisions from user (resolved)
- Support packages (quad_trajectories, quad_platforms, ros2_logger): **most minimal TOC possible**; badges that make clear they're SUPPORT for the PX4 control stack (multiple algorithms). Top of each must state they support the controllers in **Python AND C++** and that a **Docker container** eases integration.
- **Conflict markers MUST be removed everywhere.** Full workspace sweep done → only `ros2_logger/README.md` AND tracked `ros2_logger/README_original.md` (stale conflict-corrupted backup) contain markers. No other file affected.

## Decisions round 2 (resolved)
- Support header: **Rich/informative** — badges (Part of: PX4-ROS2 Control Stack | Algorithms: NMPC|NR|Geometric | Languages: Python|C++ | Docker | License: MIT) + intro stating Python AND C++ support + Docker integration.
- Cross-links: **Controllers (Py+C++) + Docker + portfolio** — link nmpc_acados_px4 & newton_raphson_px4 (+ geometric_px4), the _cpp counterparts, PX4-ROS2-Docker, evannsmc.com/projects.
- Git: **Commit + push + bump pointers**.
- `README_original.md`: **Delete it**.

## Tasks
- [x] 1. Verify submodules up to date (DONE)
- [x] 2. `nmpc_acados_px4`: fix Mocap nav button em-spaces (DONE — verified em-space, 0 conflict markers)
- [x] 3. `newton_raphson_px4`: fix Mocap nav button; align "Papers" heading + section order to nmpc; fix anchors + typo (DONE — heading order mirrors nmpc, 0 conflict markers)
- [x] 5. `quad_trajectories`: support-package style (DONE — rich badges + minimal collapsed TOC + Used-by links; all anchors resolve)
- [x] 6. `quad_platforms`: support-package style (DONE — same treatment; all anchors resolve)
- [x] 4. `ros2_logger`: resolve merge conflict; support-package style; delete README_original.md (DONE — 0 conflict markers anywhere; backup removed via git rm)
- [x] 7. Git: committed + pushed all 5 submodules on main; bumped + pushed parent pointers (DONE)

## Completion summary
- All 5 submodule READMEs committed on `main` and pushed to their GitHub origins:
  - nmpc_acados_px4 `0e99cdb → ddb640f`
  - newton_raphson_px4 `4c9dcc3 → fc5728b`
  - quad_trajectories `026359c → 738e1f9`
  - quad_platforms `80b221b → c9dcff1`
  - ros2_logger `27f39b0 → 99b439f`
- Parent workspace pointer bump committed + pushed: `8b9e499 → 5b3ade3`.
- Full-workspace conflict-marker sweep: **0 markers remain**.
- `nmpc_acados_px4/Agents.md` left untracked (task spec; not committed).

## Notes
- (changes/eliminations recorded here as the plan evolves)
