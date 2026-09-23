#!/usr/bin/env bash
# Launch one ROS 2 spawn launch per robot.  The caller's process group owns
# shutdown, so this script deliberately does not install signal handlers or
# kill child processes.
set -Eeuo pipefail

usage() {
  cat <<'EOF'
Usage:
  ./spawn_multiple.sh [n] [base_name] [dx] [dy] [x0] [y0] [z] [yaw] [-- extra ros2 args]

Defaults:
  n=3 base_name=rbwatcher dx=2.0 dy=0.0 x0=0.0 y0=0.0 z=0.05 yaw=0.0
Examples:
  ./spawn_multiple.sh 5 rbwatcher 3.0 0 0 0 0.05 0
  ./spawn_multiple.sh 4 rbwatcher 0 2.0 -- robot_model:=rbwatcher_slim run_rviz:=true
EOF
}

[[ "${1:-}" == "-h" || "${1:-}" == "--help" ]] && { usage; exit 0; }

n="${1:-3}"
base="${2:-robot}"
dx="${3:-2.0}"
dy="${4:-0.0}"
x0="${5:-0.0}"
y0="${6:-0.0}"
z="${7:-0.05}"
yaw="${8:-0.0}"

shift $(( $#>0 ? ($#>=8?8:$#) : 0 )) || true
if [[ "${1:-}" == "--" ]]; then shift; fi
extra_args=( "$@" )

pkg="robotnik_o3de"
file="spawn_robot.launch.py"

pids=()

# Spawn each robot in its own namespace.  Extra ROS launch arguments, for
# example ``run_rviz:=true``, are propagated unchanged to every robot launch.
for ((i=0;i<n;i++)); do
  name="${base}_$((i+1))"
  xi=$(python3 - <<PY
x0=float("$x0"); dx=float("$dx"); i=int("$i")
print(x0 + i*dx)
PY
)
  yi=$(python3 - <<PY
y0=float("$y0"); dy=float("$dy"); i=int("$i")
print(y0 + i*dy)
PY
)
  echo "Launching $name at ($xi,$yi,$z) yaw=$yaw"
  ros2 launch "$pkg" "$file" \
    robot:="$base" robot_model:="$base" robot_id:="$name" \
    x:="$xi" y:="$yi" z:="$z" \
    "${extra_args[@]}" &
  pids+=( "$!" )

  # Give the simulation time to register each spawned entity before the next.
  if (( i < n-1 )); then sleep 3; fi
done


echo "Spawned ${#pids[@]} robots."
echo "Press Ctrl-C to stop."
# wait and propagate failure
status=0
for pid in "${pids[@]}"; do
  if ! wait "$pid"; then status=1; fi
done
exit "$status"
