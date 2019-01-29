/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class is the core of this webserver.
 *        Class platinum provide method to deal with HTTP affair,
 *        TODO: It may also provide multithread version,
 */

#ifndef PLATINUM_PLATINUM_H
#define PLATINUM_PLATINUM_H

#include <string>
#include <vector>
#include <thread>

namespace platinum {
class Platinum {
 public:
  explicit Platinum(int num);
  ~Platinum();
  void Start();

 private:
  std::vector<std::thread> threads_;
  int thread_num_;
};
}

#endif //PLATINUM_PLATINUM_H
