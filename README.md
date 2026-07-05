# RoboBuilderSim
A simple system that uses multi-threading and classes to run a fake robotics production cycle.

Several Key assumptions are made as part of RoboBuilderSim on how the world works:
1. The world has infinite resources! So resource gathering operations can go on forever.
2. Everything runs in discrete time and takes exactly one tick to do.
3. All resource gatherers or factories for a particular resource deposit their resources in a single "silo" shared by all instances.
4. Multi-threading and shared memory will be used whenever it is possible as the point of this project is to show my c++ and multi-threading experience.
