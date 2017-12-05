#ifndef MUTLIPLENAMESPACES_HPP_ILNXZA5O
#define MUTLIPLENAMESPACES_HPP_ILNXZA5O

namespace n1 {
namespace n2 {
namespace n3 {
class Interface {
    virtual ~Interface() = default;

    virtual void test() const noexcept;
};
}  // namespace n3
}  // namespace n2
};  // namespace n1

#endif /* end of include guard: MUTLIPLENAMESPACES_HPP_ILNXZA5O */
