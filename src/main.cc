/**
 * Created by Crow on 11/23/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief
 */

#include "core/server.h"

int main(int argc, char *argv[])
{
  platinum::Server pt;
  pt.Start();
  pt.Exec();

  return 0;
}