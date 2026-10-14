# TinyKart2

**STILL NEW, BUGS MAY BE PRESENT**
*(Please feel free to complain about issues, i will hopefully resolve them!)*

**Known Issues:**
- ~~Haven't gotten around to setting up udev/dialout rules, please manually check the USB interface for the kart (`ls /dev` and plug/unplug USB connections) and set permissions with `sudo chmod 777 <filepath>`.~~
    - This *should* be fixed with newest update. Still needs testing to see if it ever goes wrong. Make sure to follow the serial setup step.
- On a fresh build, the `tk2_main` package may not build correctly when it can't find the `tk2_msgs` package. To fix this, build `tk2_msgs` seperately first with `colcon build --packages-select tk2_msgs`

**Important info/Considerations:**
- Your "workspace root" is the folder that contains the `src` directory, which itself contains all of the other nodes and packages. After building for the first time, it will also contain the `build`, `install`, and `log` folders. Unless otherwise stated, commands and filepaths will be from this location

**To Build:**
- Clone repo
- run `sudo apt install python3-vcstool` to get the tool for importing external repos
- in the `src` folder, run `cat tk2.repos | vcs import` (pipe the contents of the tk2.repos file into the vcs import command)
- run `source /opt/ros/jazzy/setup.bash` to configure your terminal instance for ROS2
    - **Once your workspace is set up and built for the first time, use your repo's own setup script in the `install` folder `source install/setup.bash` (from workspace root)**
- run `colcon build` in your workspace root to build all the packages
- run `source install/setup.bash` to configure your terminal for the workspace

**To Run**
- Before running for the first time, go to the `src/serial_setup_scripts` folder, and run `source ./udev_rules.sh`
- For individual nodes, use `ros2 run [package name] [node name]`, or `ros2 launch [package name] [launch file]` if the package comes with launch files
- To run all of the nodes for regular usage, run `ros2 launch launch/tinykart_launch.xml`