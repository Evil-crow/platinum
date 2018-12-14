/* Created by Crow on 11/21/18.
 * This file is for Class Noncopyable.
 * We can use 'default', 'delete' to identify if we need
 *      Copyconstructor, copy operator=, Moveconstructor, move operator=
*/

namespace platinum {

class Noncopyable {
 public:
  Noncopyable(const Noncopyable &) = delete;
  Noncopyable &operator=(const Noncopyable &) = delete;

 protected:
  Noncopyable() = default;
  virtual ~noncopyable() = default;
};
}

