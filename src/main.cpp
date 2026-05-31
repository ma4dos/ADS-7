// Copyright 2025 NNTU-CS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "train.h"

void savePlotData(const std::string &filename,
                  const std::string &label,
                  int *n, int *ops, int count) {
  std::ofstream file(filename, std::ios::app);
  file << "# " << label << "\n";
  for (int i = 0; i < count; i++) {
    file << n[i] << " " << ops[i] << "\n";
  }
  file << "\n\n";
  file.close();
}

void writeGnuplotScript(const std::string &filename) {
  std::ofstream script(filename);
  script << "set terminal png size 1200,800\n";
  script << "set output 'result/plot.png'\n";
  script << "set title 'Operations vs Train length'\n";
  script << "set xlabel 'n (number of cars)'\n";
  script << "set ylabel 'Operations'\n";
  script << "set grid\n";
  script << "set key top left\n";
  script << "f1(x) = a1*x + b1\n";
  script << "f2(x) = a2*x + b2\n";
  script << "f3(x) = a3*x + b3\n";
  script << "fit f1(x) 'result/data.txt' i 0 u 1:2 via a1,b1\n";
  script << "fit f2(x) 'result/data.txt' i 1 u 1:2 via a2,b2\n";
  script << "fit f3(x) 'result/data.txt' i 2 u 1:2 via a3,b3\n";
  script << "plot 'result/data.txt' i 0 u 1:2 w p pt 7 lc 'red'";
  script << " title 'All OFF', ";
  script << "f1(x) w l lc 'red' title sprintf('Fit OFF: %.2f*n+%.2f', a1, b1), ";
  script << "'result/data.txt' i 1 u 1:2 w p pt 9 lc 'green'";
  script << " title 'All ON', ";
  script << "f2(x) w l lc 'green' title sprintf('Fit ON: %.2f*n+%.2f', a2, b2), ";
  script << "'result/data.txt' i 2 u 1:2 w p pt 5 lc 'blue'";
  script << " title 'Random', ";
  script << "f3(x) w l lc 'blue' title sprintf('Fit Rand: %.2f*n+%.2f', a3, b3)\n";
  script.close();
}

int main() {
  srand(time(nullptr));

  const int MAX_N = 200;
  const int STEP = 10;
  const int TESTS = 18;

  int nVals[TESTS];
  int opsOff[TESTS];
  int opsOn[TESTS];
  int opsRand[TESTS];

  std::remove("result/data.txt");

  int idx = 0;
  for (int n = 20; n <= MAX_N; n += STEP) {
    nVals[idx] = n;

    {
      Train train;
      for (int i = 0; i < n; i++) train.addCar(false);
      train.getLength();
      opsOff[idx] = train.getOpCount();
    }

    {
      Train train;
      for (int i = 0; i < n; i++) train.addCar(true);
      train.getLength();
      opsOn[idx] = train.getOpCount();
    }

    {
      Train train;
      for (int i = 0; i < n; i++) {
        train.addCar(rand() % 2 == 1);
      }
      train.getLength();
      opsRand[idx] = train.getOpCount();
    }

    idx++;
  }

  savePlotData("result/data.txt", "All OFF", nVals, opsOff, idx);
  savePlotData("result/data.txt", "All ON", nVals, opsOn, idx);
  savePlotData("result/data.txt", "Random", nVals, opsRand, idx);

  writeGnuplotScript("result/plot.gnu");
  int ret = system("gnuplot result/plot.gnu");
  if (ret == -1) {
    std::cerr << "Failed to run gnuplot" << std::endl;
  }

  std::cout << "Experiment finished." << std::endl;
  return 0;
}
