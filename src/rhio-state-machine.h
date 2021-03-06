#pragma once

typedef void stateCallback;

/**
 * @brief
 *
 * States are represented with a unsigned char, a uniq value for each one.
 * Each state has an associated callback. The arrays *stateNames and
 * *callbacks are always of the same size. The state on index 0 of *stateNames
 * has his associated callback on the index 0 of *callbacks array.
 *
 * - The method `add()` is used to add a new state, with name and callback.
 * - The method `set()` is used to set the current state to run.
 * - The method `run()` will run the callback of the current state
 *
 */
class StateMachine {
 public:
  /**
   * @brief Construct a new State Machine object
   *
   * Dynamically allocate memory to stack the callbacks for each state.
   *
   * @param size Number of states
   */
  StateMachine(unsigned int size);

  /**
   * @brief Add a new state to the stack
   *
   * The name of each state is represented with a char value. Usually you
   * should create a set of macro defines or similar to store your "char names"
   * as macros with standar names.
   *
   * @example
   * #define STATE_SETUP 0x01
   * void callback_setup() { ... }
   * add(STATE_SETUP, callback);
   *
   * @param stateName A char value representing the name
   * @param callback
   * @return true
   * @return false
   */
  bool add(unsigned char stateName, void (*callback)());

  /**
   * @brief Set a state name as active.
   *
   * The active state will be executed with each call to method `run()`.
   *
   * This method returns the index number where the callback is allocated in the
   * stack of callbacks.
   *
   * Everytime a state changes the method `onChangeCallback()` will be called.
   * If the new method is not valid the callback will not be called.
   *
   * @example
   * #define STATE_SETUP 0x01
   * void callback_setup() { ... }
   * add(STATE_SETUP, callback);
   * set(STATE_SETUP);
   * while (1) {
   *   run(); // will run callback_setup()
   * }
   *
   * @param stateName A char value representing the name
   * @return int The index number in the stack
   */
  int set(unsigned char stateName);

  /**
   * @brief Get the current state
   *
   * @example
   * #define STATE_SETUP 0x01
   * #define STATE_NORMAL 0x02
   * set(STATE_SETUP);
   * if (getCurrent() == STATE_SETUP) {
   *   set(STATE_NORMAL);
   * }
   *
   * @return char The state name
   */
  char getCurrent();

  /**
   * @brief Clear all the state callbacks and names.
   */
  void reset();

  /**
   * @brief Execute the current state callback
   */
  void (*run)();

  /**
   * @brief Remove a state using the char name (more slow than removeByIndex)
   *
   * @param stateName
   * @return true
   * @return false
   */
  bool removeByName(unsigned char stateName);

  /**
   * @brief Remove a state using the index on the stack array (more fast than
   * removeByIndex)
   *
   * @param index
   * @return true
   * @return false
   */
  bool removeByIndex(int index);

  /**
   * @brief Method to call every time a state changes. It may be used to alert
   * about changes.
   *
   * @param callback The callback to call. Will receive the value of the
   * previous and new state
   */
  void onChange(void (*callback)(int prevState, int newState));

 private:
  /**
   * @brief Max number of states. It will allocate dynamic memory to save the
   * callbacks
   */
  unsigned int size;

  /** @brief State/Callback to execute. Index of `CallbacksPtr* callbacks` */
  int currentState;

  /**
   * @brief An array to save the names of every state.
   *
   * This array is equivalent to `CallbacksPtr* callbacks`, this means that the
   * name on the index 0 of `stateNames` has an associated callback on index 0
   * of `CallbacksPtr* callbacks`
   */
  unsigned char* stateNames;

  /** @brief Function pointer typedef for state callbacks */
  typedef stateCallback (*CallbacksPtr)();
  CallbacksPtr* callbacks;

  /** @brief Callback to run with every state change */
  void (*onChangeCallback)(int prevState, int newState);

  /**
   * @brief Return the first empty index in the array `stateNames`
   * @return int Index number, from 0 to (this->size - 1) or -1 if no more free
   * indexes
   */
  int getFreeIndex();

  /**
   * @brief Return the index in stateNames for the given stateName
   *
   * @param stateName
   * @return int Index or -1 if not found
   */
  int getStateIndex(unsigned char stateName);

  /**
   * @brief Remove the name and the callback for the given index in both
   * `stateNames` and `callbacks` arrays
   *
   * @param index
   */
  void resetIndex(int index);
};
