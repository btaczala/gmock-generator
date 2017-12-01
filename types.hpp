#ifndef TYPES_HPP_WLEIA01H
#define TYPES_HPP_WLEIA01H

#include <string>
#include <vector>

struct Arg {
    std::string _type;
};

struct Method {
    std::string _name;
    std::vector<Arg> _arguments;
    std::string _returnType;
};

struct Class {
    std::string _name;
    std::vector<Method> _methods;
    std::vector<Method> _ctors;
};

struct CXXFile {
    std::string _filePath;
    std::vector<Class> _classes;
};

#endif /* end of include guard: TYPES_HPP_WLEIA01H */
