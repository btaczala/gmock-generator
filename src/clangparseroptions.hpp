#ifndef CLANGPARSEROPTIONS_HPP_U9HUXMQA
#define CLANGPARSEROPTIONS_HPP_U9HUXMQA

enum class ClangParserOptions : unsigned {
    StrictMode = 0x1,
    UniquePtrHack = 0x2
};

inline bool operator&(ClangParserOptions lhs, ClangParserOptions rhs) {
    return static_cast<std::underlying_type<ClangParserOptions>::type>(lhs) &
           static_cast<std::underlying_type<ClangParserOptions>::type>(rhs);
}

#endif /* end of include guard: CLANGPARSEROPTIONS_HPP_U9HUXMQA */
