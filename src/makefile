# =============================================================================
# ws_px4_work — Docker + ROS 2 workspace management
# =============================================================================
#
# HOW THE DOCKER IMAGE IS BUILT
# ──────────────────────────────
# The image (px4_ros2_jazzy) is built from src/docker/Dockerfile with
# the workspace root (ws_px4_work/) as the build context.  It contains:
#
#   • ROS 2 Jazzy          — from osrf/ros:jazzy-desktop-full base image
#   • px4_msgs             — cloned from github.com/evannsmc/px4_msgs
#                            (branch: v1.16_minimal_msgs) and pre-built into
#                            /opt/ws_px4_msgs/install/ so it is always
#                            available as an upstream ROS 2 overlay
#   • Python packages      — JAX, equinox, immrax, linrax, casadi,
#                            acados_template, scipy, matplotlib installed to system Python
#
# HOW THE CONTAINER INTERACTS WITH THE HOST WORKSPACE
# ─────────────────────────────────────────────────────
# `make run` mounts ws_px4_work/ → /workspace inside the container.
# This means:
#   • /workspace/src/     — all your host packages
#   • /workspace/build/   — colcon build artifacts  (persisted on host)
#   • /workspace/install/ — colcon install tree      (persisted on host)
#   • /workspace/log/     — colcon logs              (persisted on host)
#
# The container uses --net host, so it sees all ROS 2 topics from the host
# PX4 sim and MicroXRCE bridge without any extra networking setup.
#
# Source chain inside the container (.bashrc / profile.d):
#   /opt/ros/jazzy/setup.bash
#   → /opt/ws_px4_msgs/install/setup.bash   (px4_msgs overlay)
#   → /workspace/install/setup.bash         (your built packages, if present)
#
# TYPICAL WORKFLOW
# ────────────────
#   make build          # build the image once (or after Dockerfile changes)
#   make run            # start the container
#   make build_ros      # build all packages in the workspace
# =============================================================================

IMAGE_NAME     = px4_ros2_jazzy
CONTAINER_NAME = px4_ros2
# src/ is CWD; ws_px4_work/ is one level up
WS_ROOT        := $(abspath ..)

# ── Docker image ──────────────────────────────────────────────────────────────
# Build context is src/ so COPY docker/... resolves correctly.
build:
	docker build -f docker/Dockerfile . -t $(IMAGE_NAME)

# ── Run container ─────────────────────────────────────────────────────────────
run:
	docker rm -f $(CONTAINER_NAME) 2>/dev/null || true
	docker run -itd --rm \
		--net host \
		-e ROS_DOMAIN_ID=31 \
		-v $(WS_ROOT):/workspace \
		--name $(CONTAINER_NAME) \
		$(IMAGE_NAME)

# ── Container lifecycle ───────────────────────────────────────────────────────
stop:
	docker stop $(CONTAINER_NAME)

kill:
	docker kill $(CONTAINER_NAME)

attach:
	docker exec -it $(CONTAINER_NAME) bash

# ── Build ROS 2 workspace ─────────────────────────────────────────────────────
# Optional: PACKAGES="pkg1 pkg2" to build only specific packages.
PACKAGES ?=

build_ros:
	docker exec -it $(CONTAINER_NAME) bash -lc \
		"cd /workspace && colcon build \
		   --symlink-install \
		   --cmake-args -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
		   $(if $(PACKAGES),--packages-select $(PACKAGES),)"

# Wipe build/install/log then rebuild from scratch.
clean_build_ros:
	docker exec -it $(CONTAINER_NAME) bash -lc \
		"rm -rf /workspace/build /workspace/install /workspace/log"
	$(MAKE) build_ros PACKAGES="$(PACKAGES)"

# ── Run a ROS 2 node inside the container ─────────────────────────────────────
# Usage: make ros2_run PKG=newton_raphson_px4 EXEC=run_node ARGS="--platform sim --trajectory hover"
PKG  ?=
EXEC ?=
ARGS ?=

ros2_run:
	docker exec -it $(CONTAINER_NAME) bash -lc \
		"ros2 run $(PKG) $(EXEC) $(ARGS)"

.PHONY: build run stop kill attach build_ros clean_build_ros ros2_run
