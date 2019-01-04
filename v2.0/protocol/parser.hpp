/**
 * Created by Crow on 12/29/18.
 * Copyright (c) 2018 Crow All rights reserved.
 * @author Crow
 * @brief  This file is used to describe the basic Base Class Parser
 *         Every parser for Connection must inherit from this base class
 */

#ifndef PLATINUM_PARSER_HPP
#define PLATINUM_PARSER_HPP

namespace platinum {

class Parser {
 public:
  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;

 protected:
  Parser() = default;
  virtual ~Parser() = default;
};

}

#endif //PLATINUM_PARSER_HPP
