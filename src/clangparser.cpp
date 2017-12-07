#include "clangparser.hpp"
#include "fs.hpp"

#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <set>
#include <stdexcept>

namespace {

std::string absolute(const std::string& path) {
    fs::path filePath{path};
    return fs::canonical(filePath).string();
}

template <typename Fn, typename... Args>
std::string convert(Fn&& f, Args&&... args) {
    auto cxstr = f(args...);
    std::string res = clang_getCString(cxstr);
    clang_disposeString(cxstr);

    return res;
}

/// stolen from:
/// https://stackoverflow.com/questions/12026551/how-to-find-out-whether-a-member-function-is-const-or-volatile-with-libclang/12131083
std::set<std::string> GetMethodQualifiers(CXTranslationUnit translationUnit,
                                          CXCursor cursor) {
    std::set<std::string> qualifiers;
    CXSourceRange range = clang_getCursorExtent(cursor);
    CXToken* tokens;
    unsigned int numTokens;
    clang_tokenize(translationUnit, range, &tokens, &numTokens);
    std::vector<CXToken> vecTokens;
    std::copy(tokens, tokens + numTokens, std::back_inserter(vecTokens));
    clang_disposeTokens(translationUnit, tokens, numTokens);

    bool insideBrackets = false;
    for (const auto& tok : vecTokens) {
        const std::string token =
            convert(clang_getTokenSpelling, translationUnit, tok);
        if (token == "(") {
            insideBrackets = true;
        } else if (token == "{" || token == ";") {
            break;
        } else if (token == ")") {
            insideBrackets = false;
        } else if (clang_getTokenKind(tok) == CXToken_Keyword &&
                   !insideBrackets) {
            qualifiers.insert(token);
        }
    }

    return qualifiers;
}

std::string getCursorKindName(CXCursorKind cursorKind) {
    return convert(clang_getCursorKindSpelling, cursorKind);
}

std::string getCursorSpelling(CXCursor cursor) {
    return convert(clang_getCursorSpelling, cursor);
}

constexpr const char* defaultArguments[] = {"-std=c++14"};

/// stolen from:
/// http://bastian.rieck.ru/blog/posts/2016/baby_steps_libclang_function_extents/
CXTranslationUnit compile(CXIndex index, const std::string& filename,
                          CXCompileCommand commands) {
    unsigned int numCompileCommands = clang_CompileCommands_getSize(commands);
    CXTranslationUnit tu{nullptr};

    if (numCompileCommands == 0) {
        tu = clang_parseTranslationUnit(
            index, filename.c_str(), defaultArguments,
            std::extent<decltype(defaultArguments)>::value, 0, 0,
            CXTranslationUnit_None);

    } else {
        CXCompileCommand compileCommand =
            clang_CompileCommands_getCommand(commands, 0);
        unsigned int numArguments = clang_CompileCommand_getNumArgs(commands);
        char** arguments = new char*[numArguments];

        for (unsigned int i = 0; i < numArguments; i++) {
            CXString argument = clang_CompileCommand_getArg(compileCommand, i);
            std::string strArgument = clang_getCString(argument);
            arguments[i] = new char[strArgument.size() + 1];

            std::fill(arguments[i], arguments[i] + strArgument.size() + 1, 0);

            std::copy(strArgument.begin(), strArgument.end(), arguments[i]);

            clang_disposeString(argument);
        }

        tu = clang_parseTranslationUnit(index, 0, arguments, numArguments, 0, 0,
                                        CXTranslationUnit_None);

        for (unsigned int i = 0; i < numArguments; i++) delete[] arguments[i];

        delete[] arguments;
    }

    return tu;
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

    _cbs[CXCursor_Namespace] = [this](CXCursor cursor) {
        _file._namespaces.push_back(Namespace{getCursorSpelling(cursor)});
    };

    _cbs[CXCursor_ClassDecl] = [this](CXCursor cursor) {

        if (_file._namespaces.empty()) {
            _file._namespaces.push_back(Namespace{""});
        }
        _file._namespaces.back()._classes.push_back(
            Class{getCursorSpelling(cursor)});
    };

    _cbs[CXCursor_StructDecl] = [this](CXCursor cursor) {
        if (_file._namespaces.empty()) {
            _file._namespaces.push_back(Namespace{""});
        }
        _file._namespaces.back()._classes.push_back(
            Class{getCursorSpelling(cursor)});
    };

    _cbs[CXCursor_Constructor] = [this](CXCursor cursor) {
        auto& cl = _file._namespaces.back()._classes.back();
        Method m;
        m._name = getCursorSpelling(cursor);
        m._returnType = "";
        // get params
        int nArgs = clang_Cursor_getNumArguments(cursor);
        auto type = clang_getCursorType(cursor);

        for (int i = 0; i < nArgs; ++i) {
            auto z = clang_getArgType(type, i);
            m._arguments.push_back(Arg{convert(clang_getTypeSpelling, z)});
        }
        cl._ctors.push_back(m);
    };

    _cbs[CXCursor_CXXMethod] = [this](CXCursor cursor) {
        auto& cl = _file._namespaces.back()._classes.back();

        auto returnType = clang_getResultType(clang_getCursorType(cursor));
        Method m;
        m._name = getCursorSpelling(cursor);
        m._returnType = convert(clang_getTypeSpelling, returnType);
        int nArgs = clang_Cursor_getNumArguments(cursor);
        auto type = clang_getCursorType(cursor);
        for (int i = 0; i < nArgs; ++i) {
            auto z = clang_getArgType(type, i);
            m._arguments.push_back(Arg{convert(clang_getTypeSpelling, z)});
        }
        auto s = GetMethodQualifiers(_unit, cursor);
        m._const = (s.find("const") != s.end());
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

    // std::cout << fmt::format("{} ({})",
    // getCursorKindName(clang_getCursorKind(cursor)),
    // getCursorSpelling(cursor))
    //<< std::endl;
    auto kind = clang_getCursorKind(cursor);
    if (_thiz->_cbs.find(kind) != _thiz->_cbs.end()) {
        _thiz->_cbs[kind](cursor);
    }
    clang_visitChildren(cursor, &ClangParser::visit, _thiz);

    return CXChildVisit_Continue;
}
