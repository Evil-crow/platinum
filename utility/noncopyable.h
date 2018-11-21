/* Created by Crow on 11/21/18.
 * This file is for Class Noncopyable.
 * We can use 'default', 'delete' to identify if we need
 *      Copyconstructor, copy operator=, Moveconstructor, move operator=
*/

class noncopyable {
 public:
  noncopyable(const noncopyable &) = delete;
  noncopyable &operator=(const noncopyable &) = delete;

 protected:
  noncopyable() = default;
  ~noncopyable() = default;
};


