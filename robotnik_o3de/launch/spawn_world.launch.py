import os
import tempfile
from pathlib import Path

from launch import LaunchDescription
from launch.actions import ExecuteProcess, OpaqueFunction, RegisterEventHandler, EmitEvent, DeclareLaunchArgument
from launch.event_handlers import OnProcessExit
from launch.events import Shutdown
from launch.substitutions import LaunchConfiguration


def _default_game_launcher():
    """Return the launcher built with the O3DE project next to this ROS package.

    With ``colcon build --symlink-install`` the installed launch file resolves to
    this source file, so this also works from the workspace installation.  A
    separately installed package should set ``O3DE_GAME_LAUNCHER`` (or pass the
    ``game_launcher`` launch argument) instead.
    """
    source_root = Path(__file__).resolve().parents[2]
    return source_root / "project" / "robotnik_roscon25" / "build" / "linux" / "bin" / "profile" / "robotnik_roscon25.GameLauncher"


def _start_game(context, executable, world, gui):
    executable = Path(executable.perform(context)).expanduser()
    if not executable.is_file():
        raise RuntimeError(
            f"O3DE GameLauncher not found at '{executable}'. Build the project or "
            "pass game_launcher:=/absolute/path/to/robotnik_roscon25.GameLauncher."
        )

    world = world.perform(context)
    valid_worlds = {"EmptyLevel", "BasicLevel"}
    if world not in valid_worlds:
        valid = ", ".join(sorted(valid_worlds))
        raise RuntimeError(f"Unsupported O3DE benchmark world {world!r}; expected one of: {valid}.")
    gui = gui.perform(context)
    render_fps = os.environ.get("ROBOTNIK_RENDER_FPS")
    if render_fps:
        try:
            render_fps_value = int(render_fps)
        except ValueError as exc:
            raise RuntimeError(
                "ROBOTNIK_RENDER_FPS must be an integer between 1 and 1000"
            ) from exc
        if not 1 <= render_fps_value <= 1000:
            raise RuntimeError(
                "ROBOTNIK_RENDER_FPS must be an integer between 1 and 1000"
            )
        render_fps = str(render_fps_value)

    extended_args = []
    if gui.lower() != "true":
        extended_args.append("-console-mode")

    # ``game.cfg`` is a CVar configuration file: a ``LoadLevel`` command in it
    # is rejected by O3DE.  A console-command file is the appropriate mechanism
    # for an O3DE command.  Give every launch its own file to avoid concurrent
    # benchmark runs overwriting a shared /tmp/game.cfg.
    level_name = world.lower()
    fd, cfg_path = tempfile.mkstemp(prefix="o3de-level-", suffix=".cfg")
    with os.fdopen(fd, "w") as cfg_file:
        cfg_file.write(f"LoadLevel {level_name}\n")
        if gui.lower() == "true" and render_fps:
            # Keep O3DE's own frame pacing aligned with the common benchmark cap.
            cfg_file.write("r_VSync=0\n")
            cfg_file.write(f"sys_maxfps={render_fps}\n")
    print(f"Starting O3DE level {world!r} from {cfg_path}")

    env_vars = {
        # Para sistemas con GPU integrada + dedicada (Optimus/Prime)
        "__NV_PRIME_RENDER_OFFLOAD": "1",
        "__GLX_VENDOR_LIBRARY_NAME": "nvidia",
        "__VK_LAYER_NV_optimus": "NVIDIA_only",

        # En entornos sin pantalla física (Headless con GPU)
        "ENABLE_VK_LAYER_VALVE_steam_overlay_1": "0",
    }

    proc = ExecuteProcess(
        cmd=[str(executable), f"--console-command-file={cfg_path}", *extended_args],
        name="robotnik_game_launcher",
        output="log",
        shell=False,
        emulate_tty=True,
        additional_env=env_vars,
    )



    def remove_command_file(_context):
        Path(cfg_path).unlink(missing_ok=True)
        return []

    shutdown_on_exit = RegisterEventHandler(
        OnProcessExit(
            target_action=proc,
            on_exit=[
                OpaqueFunction(function=remove_command_file),
                EmitEvent(event=Shutdown()),
            ],
        )
    )
    return [proc, shutdown_on_exit]

def generate_launch_description():
    ld = LaunchDescription()
    ld.add_action(DeclareLaunchArgument(
        "world",
        default_value="BasicLevel",
        description="O3DE level name (benchmark values: EmptyLevel or BasicLevel)",
    ))
    ld.add_action(DeclareLaunchArgument(
        "gui",
        default_value="true",
        description="Run with a rendered window; false adds O3DE -console-mode",
    ))
    ld.add_action(DeclareLaunchArgument(
        "game_launcher",
        default_value=os.environ.get("O3DE_GAME_LAUNCHER", str(_default_game_launcher())),
        description="Absolute path to robotnik_roscon25.GameLauncher",
    ))

    ld.add_action(
        OpaqueFunction(
            function=_start_game,
            kwargs={
                "executable": LaunchConfiguration("game_launcher"),
                "world": LaunchConfiguration("world"),
                "gui": LaunchConfiguration("gui"),
            },
        )
    )
    return ld
