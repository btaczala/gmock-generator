#ifndef C1__MOCK_HPP
#define C1__MOCK_HPP

/* This file is generated, do not edit */

#include <gmock/gmock.h>
// SKIP #include "/home/bata/Projects/gmock-generator/test_data/C1.hpp"

namespace n1 {

class C1Mock : public C1 {
public :
    C1Mock(int arg1) : C1(arg1) {}

    MOCK_CONST_METHOD0(f0, void());
    MOCK_METHOD0(f1, std::shared_ptr<std::string>());
    MOCK_METHOD2(f2, void(int, std::string));

};
} // namespace n1


#endif // C1__MOCK_HPP
