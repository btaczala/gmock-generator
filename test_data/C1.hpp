#ifndef C1_HPP
#define C1_HPP

#include <string>
#include <memory>

namespace n1 {

class C1 {
   public:
    explicit C1(int);
    virtual ~C1() {}

    void f0() const;
    virtual std::shared_ptr<std::string> f1();
    virtual void f2(int, std::string) {}
};

}  // namespace n1

#endif
