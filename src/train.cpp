// Copyright 2021 NNTU-CS
#include "train.h"

Train::Train() {
  first = nullptr;
  countOp = 0;
  size = 0;
}

Train::~Train() {
  clear();
}

void Train::clear() {
  if (first == nullptr) return;
  Car *current = first;
  Car *nextCar = nullptr;
  for (int i = 0; i < size; i++) {
    nextCar = current->next;
    delete current;
    current = nextCar;
  }
  first = nullptr;
  size = 0;
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
  size++;
}

int Train::getLength() {
  if (first == nullptr) return 0;

  Car *current = first;
  current->light = true;

  while (true) {
    current = current->next;
    countOp++;

    if (current->light == true) {
      current->light = false;
      break;
    }
  }

  int steps = 1;
  current = current->next;
  countOp++;

  while (current->light == false) {
    current = current->next;
    countOp++;
    steps++;
  }

  return steps;
}

int Train::getOpCount() {
  return countOp;
}
