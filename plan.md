# Plan: README cleanup across PX4 controller packages

Source of task: `nmpc_acados_px4/Agents.md`

## Findings (investigation complete)
- **Submodules up to date**: all 5 targets verified at `origin/main` HEAD (0 ahead / 0 behind) after `git fetch`. Step 1 of Agents.md is satisfied. ✓
- **"Mocap not centered" root cause**: nav buttons are `<kbd>` caps padded with `U+2003` EM SPACE (wide, non-collapsing). The **Mocap** button alone uses ASCII spaces, which HTML collapses → skinny, misaligned button. **Same bug in BOTH `nmpc_acados_px4` AND `newton_raphson_px4`.**
- **`ros2_logger/README.md` is broken**: contains unresolved (nested) git merge-conflict markers. Must resolve.
- **Library READMEs** (`quad_trajectories`, `quad_platforms`, `ros2_logger`) have no badges / nav bar / TOC — plain style, need to be brought up to nmpc style.
- **nmpc is the style template**; user only flagged Mocap for it → minimal change to nmpc.
- Out of scope (not in Agents.md list): the `_cpp` variants, geometric_px4*, px4_ros_com, testing_jacobian.

## Open decisions (interview)
- [ ] Library style scope (full nmpc chrome vs lighter)
- [ ] Badges/links for libraries (project pages? which badges?)
- [ ] Git workflow (commit+push+bump pointers vs local vs edit-only)
- [ ] ros2_logger conflict resolution canonical content

## Tasks
- [x] 1. Verify submodules up to date (DONE)
- [ ] 2. `nmpc_acados_px4`: fix Mocap nav button em-spaces (ONLY change)
- [ ] 3. `newton_raphson_px4`: fix Mocap nav button; align "Papers" heading + section order to nmpc; tidy intro
- [ ] 4. `ros2_logger`: resolve merge conflict; bring to nmpc style
- [ ] 5. `quad_trajectories`: bring to nmpc style
- [ ] 6. `quad_platforms`: bring to nmpc style
- [ ] 7. Git: commit / push per chosen workflow; bump parent submodule pointers

## Notes
- (changes/eliminations recorded here as the plan evolves)
