# RoboBuilderSim
A simple system that uses multi-threading and classes to run a fake robotics production cycle.

Several Key assumptions are made as part of RoboBuilderSim on how the world works:
1. The world has infinite resources! So resource gathering operations can go on forever.
2. Everything runs in discrete time and takes exactly one tick to do.
3. All resource gatherers or factories for a particular resource deposit their resources in a single "silo" shared by all instances.
4. Multi-threading and shared memory will be used whenever it is possible as the point of this project is to show my c++ and multi-threading experience.
5. Factories can take from silos only in the amount they need to produce one unit of something.
6. Factories can take from silos independently of if they can produce one unit in that cycle - important as sometimes it looks like resources are missing due to this.

Known limitations:
1. For the moment the data directories are hard-coded so you have to run from project root.
2. Large parameter list for the main engine and non-interactive main is clunky to use.
3. Printing to the terminal is pretty ugly and unsatisfying.

How to run:
1. Pull down the repo
2. In the repo root run "mkdir build && cd build"
3. run "cmake .."
4. run "cmake --build ."
5. navigate back up to the project root and run "./build/robot_builder_simulation"

For now if you want to change the number of factories or number of ticks you have to directly edit src/main.cpp

Future Work:
1. Better interactivity through a GUI
2. Thread pool with some cap so users can dynamically create collectors and factories.
3. Ability to "sell" resources with the point of the game being to maximize profit.
