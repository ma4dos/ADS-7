#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include "train.h"

int main() {
  std::ofstream dataFile("result/data.csv");
  dataFile << "n,all_off,all_on,random\n";

  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<> dis(0, 1);

  for (int n = 2; n <= 100; n += 2) {
    int ops_all_off = 0, ops_all_on = 0, ops_random = 0;

    Train train1;
    for (int i = 0; i < n; i++) train1.addCar(false);
    train1.getLength();
    ops_all_off = train1.getOpCount();

    Train train2;
    for (int i = 0; i < n; i++) train2.addCar(true);
    train2.getLength();
    ops_all_on = train2.getOpCount();

    Train train3;
    for (int i = 0; i < n; i++) train3.addCar(dis(gen));
    train3.getLength();
    ops_random = train3.getOpCount();

    dataFile << n << "," << ops_all_off << "," << ops_all_on << "," << ops_random << "\n";
  }
  dataFile.close();

  std::ofstream plotScript("plot.py");
  plotScript << R"(import matplotlib.pyplot as plt
import pandas as pd

data = pd.read_csv('result/data.csv')

plt.figure(figsize=(10, 6))
plt.plot(data['n'], data['all_off'], label='Все выключены', marker='o')
plt.plot(data['n'], data['all_on'], label='Все включены', marker='s')
plt.plot(data['n'], data['random'], label='Случайное', marker='^')

z1 = np.polyfit(data['n'], data['all_off'], 2)
p1 = np.poly1d(z1)
plt.plot(data['n'], p1(data['n']), "r--", alpha=0.8)

z2 = np.polyfit(data['n'], data['all_on'], 2)
p2 = np.poly1d(z2)
plt.plot(data['n'], p2(data['n']), "g--", alpha=0.8)

z3 = np.polyfit(data['n'], data['random'], 2)
p3 = np.poly1d(z3)
plt.plot(data['n'], p3(data['n']), "b--", alpha=0.8)

plt.xlabel('Количество вагонов (n)')
plt.ylabel('Число операций')
plt.title('Зависимость числа операций от длины поезда')
plt.legend()
plt.grid(True)
plt.savefig('result/plot.png')
plt.show())";
  plotScript.close();

  system("python plot.py");

  return 0;
}

