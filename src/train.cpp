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
  int len = 0;
  do {
    cur->light = false;
    cur = cur->next;
    countOp++;
    len++;
  } while (cur != first);
  for (int i = 0; i < len; i++) {
    cur = cur->prev;
    countOp++;
  }
  return len;
}

int Train::getOpCount() {
  return countOp;
}
