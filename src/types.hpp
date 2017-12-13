#ifndef TYPES_HPP_WLEIA01H
#define TYPES_HPP_WLEIA01H

#include <string>
#include <vector>

struct Arg {
    std::string _type;
    std::string _name;
};

struct Method {
    std::string _name;
    std::vector<Arg> _arguments;
    std::string _returnType;
    bool _const{false};
    bool _hasProxy{false};
};

struct Class {
    std::string _name;
    std::vector<Method> _methods;
    std::vector<Method> _ctors;
};

struct Namespace {
    std::string _name;
    std::vector<Class> _classes;
};

struct CXXFile {
    std::string _filePath;
    std::vector<Namespace> _namespaces;
};

#endif /* end of include guard: TYPES_HPP_WLEIA01H */
