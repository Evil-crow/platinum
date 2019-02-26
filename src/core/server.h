/**
 * Created by Crow on 1/29/19.
 * Copyright (c) 2019 Crow All rights reserved.
 * @author Crow
 * @brief  This class is the core of this webserver.
 *        Class Server provide method to deal with HTTP affair,
 */

#ifndef PLATINUM_CORE_SERVER_H
#define PLATINUM_CORE_SERVER_H

#include <string>
#include <vector>
#include <thread>

namespace platinum {

class Server {
 public:
  Server() = default;
  void Exec();
  void Start();

 private:
  std::vector<std::thread> threads_;
  int thread_num_ = 1;
};

}

#endif //PLATINUM_CORE_SERVER_H
