#include "generator.hpp"

#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <stdexcept>

namespace {

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
}  // namespace

GMOCKGenerator::GMOCKGenerator(const std::string& filename)
    : _filename(filename),
      _index(clang_createIndex(0, 0)),
      _unit(clang_parseTranslationUnit(_index, _filename.c_str(), nullptr, 0,
                                       nullptr, 0, CXTranslationUnit_None)) {
    if (!_index || !_unit) {
        throw std::runtime_error(fmt::format("Unable to parse {}", _filename));
    }

    _cbs[CXCursor_ClassDecl] = [this](CXCursor cursor) {
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

GMOCKGenerator::~GMOCKGenerator() {
    clang_disposeTranslationUnit(_unit);
    clang_disposeIndex(_index);
}

CXXFile GMOCKGenerator::parse() {
    auto cursor = clang_getTranslationUnitCursor(_unit);
    clang_visitChildren(cursor, &GMOCKGenerator::visit, this);

    return _file;
}

/**/ CXChildVisitResult GMOCKGenerator::visit(CXCursor cursor, CXCursor parent,
                                              CXClientData thiz) {
    CXSourceLocation location = clang_getCursorLocation(cursor);
    if (clang_Location_isFromMainFile(location) == 0)
        return CXChildVisit_Continue;

    auto _thiz = reinterpret_cast<GMOCKGenerator*>(thiz);

    // std::cout << fmt::format("{} ({})",
    // getCursorKindName(clang_getCursorKind(cursor)),
    // getCursorSpelling(cursor))
    //<< std::endl;
    auto kind = clang_getCursorKind(cursor);
    if (_thiz->_cbs.find(kind) != _thiz->_cbs.end()) {
        _thiz->_cbs[kind](cursor);
    }
    clang_visitChildren(cursor, &GMOCKGenerator::visit, _thiz);

    return CXChildVisit_Continue;
}
