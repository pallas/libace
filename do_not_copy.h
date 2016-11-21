#ifndef LACE__DO_NOT_COPY_H
#define LACE__DO_NOT_COPY_H

namespace lace {

class do_not_copy {
public:
  do_not_copy() { }

private:
  do_not_copy(const do_not_copy &);
  do_not_copy& operator=(const do_not_copy &);
};

} // namespace lace

#endif//LACE__DO_NOT_COPY_H
