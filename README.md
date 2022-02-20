# MicroMouse_MazeSolver
Depth-First Search (DFS) algorithm based C++ project to task a micro mouse to reach a goal node in a 16X16 maze.

Instructions to run the project:

Clone the repository into your catkin_build workspace:
`git clone https://github.com/Madhunc5229/MicroMouse_MazeSolver`

Setting up API Simulator:
1. Create a folder where you will download the micromouse controller. In this example the folder is created in the home directory but it can be created anywhere else.
mkdir ∼/RWA2_simulator

2. Clone the micromouse simulator in this new folder:
cd ∼/RWA2_simulator
git clone https://github.com/mackorone/mms.git
   
3. Clone a set of maze files:
git clone https://github.com/micromouseonline/mazefiles.git

4. Clone a test C++ program which simply turns the robot left and right:
git clone https://github.com/mackorone/mms-cpp.git

5. Compile the simulator.
• Install QT: sudo apt-get install qt5-default
• Build the simulator.
cd mms/src
qmake && make

6. Run the Simulator
cd ∼/RWA2_simulator/mms/bin
./mms will open the simulator

Running the package:
1. Click on “Edit Algorithm” or edit button in mouse:
2. Set the name of the mouse.
3. Choose the Directory file as the source folder (src) of the given package.
4. Enter the Build Command as “g++ api.cpp main.cpp mouse.cpp node.cpp”.
5. Run Command  : ./a.out.
6. Click on BUILD in the simulator
7. Select the maze 2 or maze 4.
8. Click on RUN.
