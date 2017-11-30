#ifndef GENERATOR_HPP_5GABWUY3
#define GENERATOR_HPP_5GABWUY3

#include <clang-c/Index.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

struct Method {
    std::string _name;
};

struct Class {
    std::string _name;
    std::vector<Method> _methods;
};

struct CXXFile {
    std::vector<Class> _classes;
};

struct GMOCKGenerator {
    using NamespaceCb = std::function<void(const std::string&)>;

    GMOCKGenerator(const std::string& filename);
    ~GMOCKGenerator();

    CXXFile parse();

   private:
    static CXChildVisitResult visit(CXCursor cursor, CXCursor parent,
                                    CXClientData thiz);

    const std::string _filename;
    CXIndex _index;
    CXTranslationUnit _unit;

    std::unordered_map<CXCursorKind, NamespaceCb> _cbs;
    CXXFile _file;
};

#endif /* end of include guard: GENERATOR_HPP_5GABWUY3 */
