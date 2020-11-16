#include "rhio-state-machine.h"

StateMachine::StateMachine(unsigned int _size) {
  size = _size;
  stateNames = new unsigned char[_size];
  callbacks = new CallbacksPtr[_size];
  onChangeCallback = 0;
  currentState = -1;
  reset();
}

bool StateMachine::add(unsigned char stateName, void (*callback)()) {
  int index = getFreeIndex();
  if (index == -1) return false;
  stateNames[index] = stateName;
  callbacks[index] = callback;
  return true;
}

void StateMachine::reset() {
  run = []() {};
  currentState = -1;
  for (unsigned int i = 0; i < size; i++) {
    resetIndex((int)i);
  }
}

int StateMachine::set(unsigned char stateName) {
  int index = getStateIndex((int)stateName);
  if (index >= 0) {
    currentState = index;
    run = callbacks[index];
    if (onChangeCallback != 0) {
      int prevStateName = (currentState > -1) ? stateNames[currentState] : -1;
      onChangeCallback(prevStateName, stateName);
    }
    return index;
  }
  return -1;
}

bool StateMachine::removeByIndex(int index) {
  if (index < 0 || index >= (int)size) return false;
  if (index == currentState) return false;
  resetIndex(index);
  return true;
}

bool StateMachine::removeByName(unsigned char stateName) {
  int index = getStateIndex(stateName);
  return removeByIndex(index);
}

char StateMachine::getCurrent() {
  if (currentState >= 0 && currentState < (int)size) {
    return stateNames[currentState];
  }
  return -1;
}

int StateMachine::getFreeIndex() {
  for (unsigned int i = 0; i < size; i++) {
    if (stateNames[i] == 0) {
      return i;
    }
  }
  return -1;
};

int StateMachine::getStateIndex(unsigned char stateName) {
  for (unsigned int i = 0; i < size; i++) {
    if (stateNames[i] == stateName) {
      return i;
    }
  }
  return -1;
};

void StateMachine::resetIndex(int index) {
  stateNames[index] = 0;
  callbacks[index] = []() {};
}

void StateMachine::onChange(void (*callback)(int prevState, int newState)) {
  onChangeCallback = callback;
}
