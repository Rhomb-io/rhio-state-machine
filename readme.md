# rhio-state-machine

In terms of application development, a state machine may be defined as a mechanism to control how the code should run, for example grouping functions to run in different moments of the application life cycle.

This is a simple c++ portable library written for any platform but developed to fit in embedded constrained systems, allowing to better organize the main application loop.

## How it works

You can define a number of states, give a name to each one and a callback, and then add all of them to a stack. Only one state (or callback) will run at the same time.

The class uses a dynamic allocated array. The size of the array is created on the class constructor and never is modified.

Minimal example:

```c++
#include "rhio-state-machine.h"

// Create a stack of one state
StateMachine states(1);

int main() {

  states.add(0x01, []() {
    // do any job on the loop
  });

  states.set(0x01);

  while (1) {
    states.run(); // "do any job..."
  }
}
```

Adding more states

```c++
#include "rhio-state-machine.h"

// Create a stack of two states
StateMachine states(2);

// Define our states
#define STATE_1 0x01
#define STATE_2 0x02

// define the callbacks for our states
void state_1() {
  // do work while running in the loop
  // ...
  if (something_happens) {
    states.set(STATE_2);
  }
}

void state_2() {
  // do work while running in the loop
  // ...
  if (something_happens) {
    states.set(STATE_1);
  }
}

int main() {

  // Create the stack
  states.add(STATE_1, state_1);
  states.add(STATE_2, state_2);

  // Set the default state (required/important).
  // If not set the loop will run a void function to the infinity
  states.set(STATE_1);

  while (1) {
    // On the first run, will execute state_1(), as defined as default.
    // When "something_happens" on state_1() it will change to run state_2
    states.run();
  }
}
```

## Working on the real life

This library was born because of the need to organize the code in large IoT applications for embedded devices.

The states allow to organize each moment of the life cycle of the application: the boot or setup, the normal operation mode, low consumption mode, the system error mode and any other that may be needed.

Several states can be instantiated for different parts of the application, for example a network module can use the states to manage the whole process of connection and data transmission.

## Development

If you want to contribute to the development, the best way is to use [rhio-devkit](https://github.com/Rhomb-io/rhio-devkit), a project with VSCode and Platformio that includes a development copy of this repository.

## License

Developed by [Rhomb.io](https://rhomb.io) Software Team from Tecnofigners. Licensed under the GNU/GPL V3. Please concat us for commercial license or custom development.
