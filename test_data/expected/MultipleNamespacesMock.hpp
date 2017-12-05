#ifndef Interface__MOCK_HPP
#define Interface__MOCK_HPP

/* This file is generated, do not edit */

#include <gmock/gmock.h>
#include "/home/bata/Projects/gmock-generator/test_data/MutlipleNamespaces.hpp"

namespace n1 {
namespace n2 {
namespace n3 {

class InterfaceMock : public Interface {
public :
 
    MOCK_CONST_METHOD0(test, void());

};
} // namespace n3
} // namespace n2
} // namespace n1


#endif // Interface__MOCK_HPP
