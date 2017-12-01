#ifndef CLANGPARSER_HPP_EI7CPHU1
#define CLANGPARSER_HPP_EI7CPHU1

#include <clang-c/Index.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

#include "types.hpp"

struct ClangParser {
    ClangParser(const std::string& filename);
    ~ClangParser();

    CXXFile parse();

   private:
    using NamespaceCb = std::function<void(CXCursor)>;
    static CXChildVisitResult visit(CXCursor cursor, CXCursor parent,
                                    CXClientData thiz);

    const std::string _filename;
    CXIndex _index;
    CXTranslationUnit _unit;

    std::unordered_map<CXCursorKind, NamespaceCb> _cbs;
    CXXFile _file;
};

#endif /* end of include guard: CLANGPARSER_HPP_EI7CPHU1 */
