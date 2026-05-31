// Copyright 2021 NNTU-CS
#include "train.h"

Train::Train() {
  first = nullptr;
  countOp = 0;
}

void Train::addCar(bool light) {
  Car *newCar = new Car;
  newCar->light = light;
  newCar->next = nullptr;
  newCar->prev = nullptr;
  if (first == nullptr) {
    first = newCar;
    newCar->next = newCar;
    newCar->prev = newCar;
  } else {
    Car *last = first->prev;
    last->next = newCar;
    newCar->prev = last;
    newCar->next = first;
    first->prev = newCar;
  }
}

int Train::getLength() {
  if (first == nullptr) return 0;
  Car *cur = first;

  if (cur->light == false) {
    cur->light = true;
    int steps = 0;
    Car *temp = cur;
    do {
      temp = temp->next;
      ++countOp;
      ++steps;
    } while (temp->light == false);
    if (temp == cur) {
      cur->light = false;
      for (int i = 0; i < steps; ++i) {
        cur = cur->prev;
        ++countOp;
      }
      return steps;
    } else {
      for (int i = 0; i < steps; ++i) {
        temp = temp->prev;
        ++countOp;
      }
      cur = temp;
    }
  }

  cur = first;
  cur->light = true;
  int k = 1;
  while (true) {
    for (int i = 0; i < k; ++i) {
      cur = cur->next;
      ++countOp;
    }
    cur->light = false;
    for (int i = 0; i < k; ++i) {
      cur = cur->prev;
      ++countOp;
    }
    if (first->light == false) {
      int len = k;
      first->light = true;
      return len;
    }
    ++k;
  }
}

int Train::getOpCount() {
  return countOp;
}
