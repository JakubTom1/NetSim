## NetSim
# Network Simulation Group Project in C++

This is a group project created for the "Advanced Object-Oriented Programming" course. It's goal was to design and implement a system for modeling and simulating the operation of a network - using the example of "production lines" in a factory.

# Model
A production line in a factory consists of the following elements (nodes), connected into a coherent whole:

 * unloading ramps - deliver semi-finished products to the factory (they are the sources of the network)
 * workers - process semi-finished products
 * warehouses - store semi-finished products after the entire technological process has been completed (they are the sinks of the network)

The following connections are allowed within the technological process:

 * unloading ramp → worker (i.e. delivery of semi-finished products to the first production stage)
 * worker → worker (i.e. delivery of a semi-finished product for further processing; in particular, we allow the situation where the same worker hands over the result of their work to themselves for further processing)
 * worker → warehouse (i.e. placing the final semi-finished product in the warehouse)
Each ramp and each worker can only pass semi-finished products to a certain subset of all possible recipients (workers and warehouses) - this subset is defined individually for each ramp/worker. In the example below, allowable node recipients are marked with arrows.

Example network (R - ramp, W - worker, S - warehouse)

Additional assumptions:

- Each semi-finished product delivered to the factory has its own unique serial number, which does not change between production stages (this number is a positive integer).
- Deliveries of semi-finished products to ramps take place with a specified frequency (e.g. every other simulation turn), starting from the first turn. One unit of semi-finished product is delivered at a time.
- A worker processes a single unit of semi-finished product at a time (in a given turn), and each worker's process takes a specified number of turns (it may differ between workers, but is always at least one turn).
- Each worker has their own preferences regarding the order in which they process the semi-finished products delivered to them - for example, some prefer to process them in the order they arrive (first in, first out, FIFO), while others prefer to start with the semi-finished product delivered last (last in, first out, LIFO, stack). Currently, workers only process products in either the FIFO or LIFO mode, but in the future they will definitely want to develop new schemes for organizing their work...
- A worker does not interrupt work on the currently processed product when new semi-finished products arrive - they are simply added to the queue and wait for the worker to finish their current task.
- Currently, each ramp/worker passes the processed semi-finished product to one of the recipients assigned to them without favoring any of them (i.e. the recipient is selected from a pool of n recipients with equal probability - 1/n). Each ramp/worker has the possibility to have their own preferences in this regard (e.g. passing semi-finished products to worker A twice as often as to worker B).
- Each warehouse has the ability to store products independently of other warehouses (i.e. the internal implementation of the "semi-finished product container" functionality may differ between warehouses).

# Simulation

For a given network model (loaded from a file), we want to be able to simulate its operation - for example, to check the potential efficiency of a production line.

The unit of time in the simulation is a turn. Each turn consists of stages, considered in the following order:

  1. Delivery of semi-finished products to ramps
  _Semi-finished products "appear" on the ramp._
  2. Transfer of semi-finished products to recipients.
  _The semi-finished product is instantly transferred from the sender to the recipient - it is never "in transit."_
  3. Processing of semi-finished products by workers.
  4. Reporting on the state of the simulation (if applicable for a given turn)
We have assumed that within each stage of the simulation, nodes are considered sequentially, in ascending order of ID (so, for example, worker #1 performs the task first, then worker #2, etc.) - this eliminates the need to use multi-threading mechanisms.
The simulation time starts from 1 (i.e. the first turn has ID 1).
#
_This repository is a copy of original, private repository and does not reflect the number of commits added by each of the contributors._
