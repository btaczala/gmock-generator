#include "clangparser.hpp"

#include <fmt/format.h>
#include <experimental/filesystem>
#include <functional>
#include <iostream>
#include <stdexcept>

namespace {

std::string absolute(const std::string& path) {
    namespace fs = std::experimental::filesystem;

    fs::path filePath{path};
    return fs::canonical(filePath).string();
}

template <typename T, typename Fn>
std::string convert(T&& t, Fn&& f) {
    auto cxstr = f(t);
    std::string res = clang_getCString(cxstr);
    clang_disposeString(cxstr);

    return res;
}

std::string getCursorKindName(CXCursorKind cursorKind) {
    return convert(cursorKind, clang_getCursorKindSpelling);
}

std::string getCursorSpelling(CXCursor cursor) {
    return convert(cursor, clang_getCursorSpelling);
}

constexpr const char* defaultArguments[] = {"-std=c++14"};

CXTranslationUnit compile(CXIndex index, const std::string& filename,
                          CXCompileCommand commands) {
    return clang_parseTranslationUnit(
        index, filename.c_str(), defaultArguments,
        std::extent<decltype(defaultArguments)>::value, nullptr, 0,
        CXTranslationUnit_None);
}

}  // namespace

ClangParser::ClangParser(const std::string& filename)
    : _filename(absolute(filename)), _index(clang_createIndex(0, 0)) {
    CXCompilationDatabase_Error error;
    _compilationDatabase = clang_CompilationDatabase_fromDirectory(".", &error);
    _compileCommands = clang_CompilationDatabase_getCompileCommands(
        _compilationDatabase, _filename.c_str());

    _unit = compile(_index, filename, nullptr);

    if (!_index || !_unit) {
        throw std::runtime_error(fmt::format("Unable to parse {}", _filename));
    }

    _file._filePath = _filename;

    _cbs[CXCursor_ClassDecl] = [this](CXCursor cursor) {
        _file._classes.push_back(Class{getCursorSpelling(cursor)});
    };
    _cbs[CXCursor_StructDecl] = [this](CXCursor cursor) {
        _file._classes.push_back(Class{getCursorSpelling(cursor)});
    };

    _cbs[CXCursor_Constructor] = [this](CXCursor cursor) {
        auto& cl = _file._classes.back();
        Method m;
        m._name = getCursorSpelling(cursor);
        m._returnType = "";
        // get params
        int nArgs = clang_Cursor_getNumArguments(cursor);
        auto type = clang_getCursorType(cursor);

        for (int i = 0; i < nArgs; ++i) {
            auto z = clang_getArgType(type, i);
            m._arguments.push_back(Arg{convert(z, clang_getTypeSpelling)});
        }
        cl._ctors.push_back(m);
    };

    _cbs[CXCursor_CXXMethod] = [this](CXCursor cursor) {
        auto& cl = _file._classes.back();

        auto type = clang_getCursorType(cursor);
        Method m;
        m._name = getCursorSpelling(cursor);
        m._returnType = convert(type, clang_getTypeSpelling);
        cl._methods.push_back(m);
    };
}

ClangParser::~ClangParser() {
    clang_CompileCommands_dispose(_compileCommands);
    clang_CompilationDatabase_dispose(_compilationDatabase);

    clang_disposeTranslationUnit(_unit);
    clang_disposeIndex(_index);
}

CXXFile ClangParser::parse() {
    auto cursor = clang_getTranslationUnitCursor(_unit);
    clang_visitChildren(cursor, &ClangParser::visit, this);

    return _file;
}

/**/ CXChildVisitResult ClangParser::visit(CXCursor cursor, CXCursor parent,
                                           CXClientData thiz) {
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0)
        return CXChildVisit_Continue;

    auto _thiz = reinterpret_cast<ClangParser*>(thiz);

    std::cout << fmt::format("{} ({})",
                             getCursorKindName(clang_getCursorKind(cursor)),
                             getCursorSpelling(cursor))
              << std::endl;
    auto kind = clang_getCursorKind(cursor);
    if (_thiz->_cbs.find(kind) != _thiz->_cbs.end()) {
        _thiz->_cbs[kind](cursor);
    }
    clang_visitChildren(cursor, &ClangParser::visit, _thiz);

    return CXChildVisit_Continue;
}
