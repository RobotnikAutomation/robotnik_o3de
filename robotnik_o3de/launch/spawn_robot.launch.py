from launch import LaunchDescription
from launch.actions import ExecuteProcess, OpaqueFunction
from launch.substitutions import TextSubstitution, LaunchConfiguration, FindExecutable
from launch.conditions import IfCondition
from launch_ros.actions import Node
from launch_ros.substitutions import FindPackageShare

from robotnik_common.launch import AddArgumentParser, ExtendedArgument

def create_spawn_entity_cmd(robot_id, robot_model, x, y, z):
    yaml_arg = [
        TextSubstitution(text='\'{ "name": "'), robot_model, '", ',
        TextSubstitution(text='xml: "", "robot_namespace": "'), robot_id,
        TextSubstitution(text='", initial_pose: { position: { x: '), x,
        TextSubstitution(text=', y: '), y,
        TextSubstitution(text=', z: '), z,
        TextSubstitution(text=' }, orientation: { x: 0.0, y: 0.0, z: 0.0, w: 1.0 } }, reference_frame: "'),
        robot_id,
        TextSubstitution(text='"}\''),
    ]
    return [
        FindExecutable(name='ros2'),
        'service', 'call',
        '/spawn_entity',
        'gazebo_msgs/srv/SpawnEntity',
        yaml_arg,
    ]

def _wait_and_spawn(context, robot_id, robot, x, y, z, timeout_lc):
    """Wait for O3DE's spawner without creating a service client.

    The O3DE ROS2 spawner creates ``/spawn_entity`` while the level is loaded.
    Creating a client before that server exists also creates the DDS request
    topic.  With the O3DE gem this races with server creation and can make the
    GameLauncher abort with an incompatible ``rq/spawn_entityRequest`` type.
    Querying the ROS graph is passive, so wait for the advertised service first
    and create the real client only in the subsequent ``ros2 service call``.
    """
    import rclpy
    import time
    from rclpy.node import Node

    timeout = float(timeout_lc.perform(context)) if timeout_lc else 60.0

    rclpy.init(args=None)
    node = Node('wait_for_spawn_entity_service')
    deadline = time.monotonic() + timeout
    service_available = False
    try:
        while time.monotonic() < deadline:
            services = dict(node.get_service_names_and_types())
            service_types = services.get('/spawn_entity', [])
            if 'gazebo_msgs/srv/SpawnEntity' in service_types:
                service_available = True
                break
            time.sleep(0.1)
    finally:
        node.destroy_node()
        rclpy.shutdown()

    if not service_available:
        raise RuntimeError(
            f"Timed out after {timeout:.1f}s waiting for /spawn_entity to spawn "
            f"robot '{robot_id.perform(context)}'."
        )

    # Give the O3DE executor a moment to finish activating the advertised service.
    time.sleep(0.5)

    proc = ExecuteProcess(
        shell=True,
        output='screen',
        cmd=create_spawn_entity_cmd(robot_id, robot, x, y, z),
    )
    return [proc]

def generate_launch_description():
    raw_args = [
        ("robot_id", "Unique Robot Identifier", "robot", "ROBOT_ID"),
        ("robot", "Robot Model Name", "", "ROBOT"),
        ("robot_model", "Robot Variant or Type", LaunchConfiguration('robot'), "ROBOT_MODEL"),
        ("x", "Initial X Coordinate", "0.0", "X"),
        ("y", "Initial Y Coordinate", "0.0", "Y"),
        ("z", "Initial Z Coordinate", "0.0", "Z"),
        ("run_rviz", "Run RViz", "True", "RUN_RVIZ"),
        ("spawn_timeout", "Seconds to wait for /spawn_entity", "60.0", "SPAWN_TIMEOUT"),
    ]

    ld = LaunchDescription()
    add_to_launcher = AddArgumentParser(ld)
    for arg in raw_args:
        add_to_launcher.add_arg(
            ExtendedArgument(
                name=arg[0],
                description=arg[1],
                default_value=arg[2],
                use_env=True,
                environment=arg[3],
            )
        )
    params = add_to_launcher.process_arg()

    # wait for service, then spawn
    ld.add_action(
        OpaqueFunction(
            function=_wait_and_spawn,
            kwargs=dict(
                robot_id=params['robot_id'],
                robot=params['robot'],
                x=params['x'],
                y=params['y'],
                z=params['z'],
                timeout_lc=params['spawn_timeout'],
            ),
        )
    )

    # rviz (unchanged)
    ld.add_action(
        Node(
            package="rviz2",
            executable="rviz2",
            namespace=params['robot_id'],
            arguments=[
                '-d', [FindPackageShare('robotnik_o3de'), '/config/rviz_config.rviz'],
                '-f', [params['robot_id'], '/odom'],
                '-t', [params['robot_id'], ' - ', params['robot_model'], ' - RViz'],
            ],
            condition=IfCondition(params['run_rviz'])
        )
    )

    # dummy action to keep the launch alive
    ld.add_action(ExecuteProcess(
        cmd=["bash", "-c", "while true; do sleep 1; done"],
        name="keep_alive",
        output="screen",
    ))

    return ld
