#ifndef UNIQUEPTR_HPP_26OFG7HE
#define UNIQUEPTR_HPP_26OFG7HE

#include <memory>
#include <string>

struct InterfaceWithUniquePtr {
    virtual std::unique_ptr<std::string>  foo() const = 0;
};


#endif /* end of include guard: UNIQUEPTR_HPP_26OFG7HE */
