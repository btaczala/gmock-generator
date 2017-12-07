#ifndef Interface__MOCK_HPP
#define Interface__MOCK_HPP

/* This file is generated, do not edit */

#include <gmock/gmock.h>
// SKIP #include "/home/bata/Projects/gmock-generator/test_data/MultipleNamespaces.hpp"

namespace n1 {
namespace n2 {
namespace n3 {

class InterfaceMock : public Interface {
public :
    InterfaceMock(int arg1, float arg2, char arg3) : Interface(arg1,arg2,arg3) {}

    MOCK_CONST_METHOD0(test, void());
    MOCK_CONST_METHOD3(foo, void(int, char, float));

};
} // namespace n3
} // namespace n2
} // namespace n1


#endif // Interface__MOCK_HPP
