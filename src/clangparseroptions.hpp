#ifndef CLANGPARSEROPTIONS_HPP_U9HUXMQA
#define CLANGPARSEROPTIONS_HPP_U9HUXMQA

enum class ClangParserOptions : unsigned {
    //! Should we actually try to resolve all includes
    StrictMode = 0x1,
    //! Should hack for unique ptr be included or not
    UniquePtrHack = 0x2
};

inline bool operator&(ClangParserOptions lhs, ClangParserOptions rhs) {
    return static_cast<std::underlying_type<ClangParserOptions>::type>(lhs) &
           static_cast<std::underlying_type<ClangParserOptions>::type>(rhs);
}

inline ClangParserOptions& operator&=(ClangParserOptions& lhs,
                                      ClangParserOptions rhs) {
    lhs = static_cast<ClangParserOptions>(
        static_cast<std::underlying_type<ClangParserOptions>::type>(lhs) &
        static_cast<std::underlying_type<ClangParserOptions>::type>(rhs));

    return lhs;
}

inline ClangParserOptions& operator|=(ClangParserOptions& lhs,
                                      ClangParserOptions rhs) {
    lhs = static_cast<ClangParserOptions>(
        static_cast<std::underlying_type<ClangParserOptions>::type>(lhs) |
        static_cast<std::underlying_type<ClangParserOptions>::type>(rhs));

    return lhs;
}

#endif /* end of include guard: CLANGPARSEROPTIONS_HPP_U9HUXMQA */
