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

---

# Phase 2 (new request)
Two tasks: (A) extend README treatment to ALL remaining packages; (B) rename `fig8_contraction` trajectory → `fig8_akash` without breaking anything.

## Phase 2 findings
### README scope — remaining packages (README line counts)
- Controllers w/ real READMEs: `ff_f8_px4` (168), `geometric_px4` (162), `geometric_px4_flat` (223), `geometric_px4_flip` (133), `geometric_px4_quat` (199), `nmpc_acados_px4_cpp` (251), `newton_raphson_px4_cpp` (93).
- STUB READMEs (~1 line, need full build): `geometric_px4_cpp`, `quad_trajectories_cpp`, `quad_platforms_cpp`, `ros2_logger_cpp`, `testing_jacobian`.
- `offboard_cpp`: NO README.
- `px4_ros_com`: PX4 **upstream third-party** (url PX4/px4_ros_com) — likely SKIP (not user's repo).
- (Package purposes/tiers: awaiting Explore survey agent.)

### Rename scope — CRITICAL details
- **Cross-language inconsistency**: Python uses `fig8_contraction` (enum `FIG8_CONTRACTION`, fn `fig8_contraction`); C++ uses `f8_contraction` (enum `F8_CONTRACTION`, fn `f8_contraction`). Two parallel renames OR unify — DECISION NEEDED.
- **`fig8_contraction` is NOT a substring of siblings** (`fig8_heading_contraction`, etc.) → whole-token replace is safe. Do NOT touch `hover_/spiral_/trefoil_/heading_contraction` or the "contraction CONTROLLER" references (different concept).
- **Pre-existing bug**: `ff_f8_px4` references `TrajectoryType.F8_CONTRACTION` which doesn't exist on the Python enum (`FIG8_CONTRACTION`). Will fix as part of rename (point to `FIG8_AKASH`).
- **Files to edit (source only, NOT data)**: quad_trajectories {core,registry,types,__init__}.py + README; quad_trajectories_cpp {trajectories.hpp,types.hpp,registry.cpp,utils.cpp}; nmpc_acados_px4 {ros2px4_node.py,README,commands.md,test_*_commands.md}; nmpc_acados_px4_cpp {offboard_control_node.cpp/.hpp,README,commands.md}; newton_raphson_px4 {ros2px4_node.py,README,commands.md,test_*}; newton_raphson_px4_cpp {offboard_control_node.cpp,run_node.cpp,README,commands.md,test_*}; ff_f8_px4 {ros2px4_node.py,run_node.py,README,commands.md,package.xml,setup.py,docs/*.qmd}.
- **LEAVE UNTOUCHED**: `data_analysis/log_files/**.csv` (historical experiment data) — renaming past logs would falsify records.

## Phase 2 decisions (resolved)
- Edge packages: **SKIP** offboard_cpp, testing_jacobian, px4_ros_com.
- Polished controller READMEs: **rebuild to nmpc template** (preserve technical content + citations).
- Rename: **unify to `fig8_akash` everywhere** — C++ drops `f8_contraction`/`F8_CONTRACTION`; all langs → `fig8_akash` / `FIG8_AKASH` / fn `fig8_akash`.
- Rename: **hard replace, no alias**; also fix ff_f8_px4 `F8_CONTRACTION` bug AND nmpc_cpp `FIG8_CONTRACTION`-vs-`F8_CONTRACTION` mismatch (unification reconciles both).
- Verify after rename: static consistency sweep + colcon build if toolchain present.

### Unification token map (replace in SOURCE only; skip data_analysis/ + *.csv + build/ + install/)
- `fig8_contraction` → `fig8_akash`
- `f8_contraction`  → `fig8_akash`
- `FIG8_CONTRACTION` → `FIG8_AKASH`
- `F8_CONTRACTION`  → `FIG8_AKASH`
(None are substrings of protected siblings hover_/spiral_/trefoil_/heading_contraction → safe. Also fixes my Phase-1 quad_trajectories README which wrongly used `F8_CONTRACTION`.)

## Phase 2 tasks
- [x] P2-1. Finalize scope via interview
- [x] P2-0. Freshness audit (3 behind: quad_trajectories_cpp+6, newton_raphson_px4_cpp+5, quad_platforms_cpp+2) → updated all 3 to origin/main per user
- [x] P2-2..4. Rename fig8_contraction→fig8_akash applied + verified (Python+C++, unify, hard replace); fixed ff_f8 + nmpc_cpp latent bugs; py_compile OK; siblings intact
- [x] P2-5. Rename committed + PUSHED in 6 submodules (quad_trajectories, quad_trajectories_cpp, nmpc, nmpc_cpp, NR, nr_cpp). ff_f8 rename held in PARENT working tree (ff_f8 is NOT a submodule — it's a parent-tracked dir).
- [x] P2-6. README rebuilds done: [agents] geometric_px4{,_flat,_flip,_quat}, nmpc_cpp, nr_cpp, geometric_px4_cpp; [me] ff_f8_px4, quad_trajectories_cpp, quad_platforms_cpp (resolved its conflict markers too), ros2_logger_cpp
- [x] P2-6b. Nav-bar EM SPACE normalized (8 files); all anchors resolve (fixed geometric_px4_flat ψ̈ heading→ASCII); 0 conflict markers; 0 old tokens
- [x] P2-7. 10 submodule README commits pushed. Parent: commit ff_f8 + pointer bumps + plan.md, push. (in progress)

## Phase 2 completion summary
- Rename `fig8_contraction`→`fig8_akash` unified across Python + C++; hard replace; fixed ff_f8 + nmpc_cpp latent enum bugs. Committed+pushed in 6 submodules.
- Updated 3 stale C++ submodules to origin/main first (quad_trajectories_cpp, newton_raphson_px4_cpp, quad_platforms_cpp) — pulled upstream gravity/trajectory/battery changes per user.
- READMEs: 10 submodules + ff_f8_px4 (parent dir) standardized to house style. px4_ros_com / offboard_cpp / testing_jacobian skipped per user.
- Historical data_analysis/*.csv left untouched.

### IMPORTANT git note
- `ff_f8_px4` and `offboard_cpp` are NOT submodules — plain dirs in the PARENT repo. `git -C ff_f8_px4` resolves to the PARENT .git. Commit their changes in the parent.

## Notes
- (changes/eliminations recorded here as the plan evolves)
