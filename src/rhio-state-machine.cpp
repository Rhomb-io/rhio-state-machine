#include "rhio-state-machine.h"

StateMachine::StateMachine(unsigned int size) {
  this->size = size;
  this->stateNames = new unsigned char[size];
  this->callbacks = new CallbacksPtr[size];
  this->reset();
}

bool StateMachine::add(unsigned char stateName, void (*callback)()) {
  int index = getFreeIndex();
  if (index == -1) return false;
  stateNames[index] = stateName;
  callbacks[index] = callback;
  return true;
}

void StateMachine::reset() {
  this->run = []() {};
  for (unsigned int i = 0; i < this->size; i++) {
    this->resetIndex((int)i);
  }
}

int StateMachine::set(unsigned char stateName) {
  int index = this->getStateIndex(stateName);
  if (index >= 0) {
    this->currentState = index;
    this->run = callbacks[index];
    return index;
  }
  return -1;
}

bool StateMachine::removeByIndex(int index) {
  if (index < 0 || index >= (int)this->size) return false;
  if (index == currentState) return false;
  this->resetIndex(index);
  return true;
}

bool StateMachine::removeByName(unsigned char stateName) {
  int index = this->getStateIndex(stateName);
  return this->removeByIndex(index);
}

char StateMachine::getCurrent() {
  if (currentState >= 0 && currentState < (int)this->size) {
    return stateNames[currentState];
  }
  return -1;
}

int StateMachine::getFreeIndex() {
  for (unsigned int i = 0; i < this->size; i++) {
    if (this->stateNames[i] == 0) {
      return i;
    }
  }
  return -1;
};

int StateMachine::getStateIndex(unsigned char stateName) {
  for (unsigned int i = 0; i < this->size; i++) {
    if (this->stateNames[i] == stateName) {
      return i;
    }
  }
  return -1;
};

void StateMachine::resetIndex(int index) {
  stateNames[index] = 0;
  callbacks[index] = []() {};
}
