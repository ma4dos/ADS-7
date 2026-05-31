// Copyright 2022 NNTU-CS
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "train.h"

void saveData(const std::string &fname,
              const std::string &label,
              int *n, int *ops, int cnt) {
  std::ofstream out(fname, std::ios::app);
  out << "# " << label << "\n";
  for (int i = 0; i < cnt; i++)
    out << n[i] << " " << ops[i] << "\n";
  out << "\n\n";
  out.close();
}

void writeGnuplot(const std::string &fname) {
  std::ofstream scr(fname);
  scr << "set terminal png size 1200,800\n";
  scr << "set output 'result/plot.png'\n";
  scr << "set title 'Operations vs Train length'\n";
  scr << "set xlabel 'n (cars)'\n";
  scr << "set ylabel 'Operations'\n";
  scr << "set grid\n";
  scr << "set key top left\n";
  scr << "f1(x)=a1*x+b1\n";
  scr << "f2(x)=a2*x+b2\n";
  scr << "f3(x)=a3*x+b3\n";
  scr << "fit f1(x) 'result/data.txt' i 0 u 1:2 via a1,b1\n";
  scr << "fit f2(x) 'result/data.txt' i 1 u 1:2 via a2,b2\n";
  scr << "fit f3(x) 'result/data.txt' i 2 u 1:2 via a3,b3\n";
  scr << "plot 'result/data.txt' i 0 u 1:2 w p pt 7 lc 'red'"
      << " title 'All OFF', f1(x) w l lc 'red'"
      << " title sprintf('Fit: %.0f*n+%.0f',a1,b1),"
      << " 'result/data.txt' i 1 u 1:2 w p pt 9 lc 'green'"
      << " title 'All ON', f2(x) w l lc 'green'"
      << " title sprintf('Fit: %.0f*n+%.0f',a2,b2),"
      << " 'result/data.txt' i 2 u 1:2 w p pt 5 lc 'blue'"
      << " title 'Random', f3(x) w l lc 'blue'"
      << " title sprintf('Fit: %.0f*n+%.0f',a3,b3)\n";
  scr.close();
}

int main() {
  std::srand(std::time(nullptr));
  const int MAX_N = 200;
  const int STEP = 10;
  const int TESTS = 19;
  int nVals[TESTS];
  int opsOff[TESTS];
  int opsOn[TESTS];
  int opsRand[TESTS];

  std::remove("result/data.txt");
  int idx = 0;
  for (int n = 20; n <= MAX_N; n += STEP) {
    nVals[idx] = n;
    {
      Train t;
      for (int i = 0; i < n; i++) t.addCar(false);
      t.getLength();
      opsOff[idx] = t.getOpCount();
    }
    {
      Train t;
      for (int i = 0; i < n; i++) t.addCar(true);
      t.getLength();
      opsOn[idx] = t.getOpCount();
    }
    {
      Train t;
      for (int i = 0; i < n; i++)
        t.addCar(std::rand() % 2 == 1);  // NOLINT(runtime/threadsafe_fn)
      t.getLength();
      opsRand[idx] = t.getOpCount();
    }
    idx++;
  }

  saveData("result/data.txt", "All OFF", nVals, opsOff, idx);
  saveData("result/data.txt", "All ON", nVals, opsOn, idx);
  saveData("result/data.txt", "Random", nVals, opsRand, idx);
  writeGnuplot("result/plot.gnu");
  int ret = system("gnuplot result/plot.gnu");
  if (ret == -1)
    std::cerr << "gnuplot failed" << std::endl;

  std::cout << "Done. Plot saved to result/plot.png" << std::endl;
  return 0;
}
