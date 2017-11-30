#include "generator.hpp"

#include <fmt/format.h>
#include <iostream>
#include <stdexcept>

namespace {
std::string getCursorKindName(CXCursorKind cursorKind) {
    CXString kindName = clang_getCursorKindSpelling(cursorKind);
    std::string result = clang_getCString(kindName);

    clang_disposeString(kindName);
    return result;
}
std::string getCursorSpelling(CXCursor cursor) {
    CXString cursorSpelling = clang_getCursorSpelling(cursor);
    std::string result = clang_getCString(cursorSpelling);

    clang_disposeString(cursorSpelling);
    return result;
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

    _cbs[CXCursor_ClassDecl] = [this](const std::string& name) {
        _file._classes.push_back(Class{name});
    };

    _cbs[CXCursor_CXXMethod] = [this](const std::string& name) {
        auto& cl = _file._classes.back();
        cl._methods.push_back(Method{name});
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

    std::cout << fmt::format("{} ({})",
                             getCursorKindName(clang_getCursorKind(cursor)),
                             getCursorSpelling(cursor))
              << std::endl;
    auto kind = clang_getCursorKind(cursor);
    if (_thiz->_cbs.find(kind) != _thiz->_cbs.end()) {
        _thiz->_cbs[kind](getCursorSpelling(cursor));
    }
    clang_visitChildren(cursor, &GMOCKGenerator::visit, _thiz);

    return CXChildVisit_Continue;
}
