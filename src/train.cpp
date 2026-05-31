// Copyright 2021 NNTU-CS
#include "train.h"
#include <cstdlib>

Train::Train() : countOp(0), first(nullptr) {}

void Train::addCar(bool light) {
  Car* newCar = new Car{light, nullptr, nullptr};
  if (!first) {
    first = newCar;
    first->next = first;
    first->prev = first;
  } else {
    newCar->next = first;
    newCar->prev = first->prev;
    first->prev->next = newCar;
    first->prev = newCar;
  }
}

int Train::getLength() {
  if (!first) return 0;
  if (first->next == first) return 1;

  countOp = 0;
  Car* current = first;

  // Шаг 1: выключаем свет в стартовом вагоне (если включён)
  if (current->light) {
    current->light = false;
    current = current->next;
    countOp++;
  }

  int steps = 0;
  bool found = false;

  while (!found) {
    steps++;
    current = current->next;
    countOp++;

    // Двигаемся вперёд, считая шаги, пока не вернёмся к вагону с выключенным светом
    if (!current->light) {
      // Включаем свет — это метка, что мы здесь уже были
      current->light = true;

      // Возвращаемся на steps шагов назад
      for (int i = 0; i < steps; i++) {
        current = current->prev;
        countOp++;
      }

      // Если стартовый вагон теперь светится — значит, мы обошли весь круг
      if (current->light) {
        found = true;
      } else {
        // Иначе продолжаем — выключаем стартовый, идём дальше
        current->light = false;
        current = current->next;
        countOp++;
        steps = 0;
      }
    }
  }

  return steps;
}

int Train::getOpCount() {
  return countOp;
}

  
 
