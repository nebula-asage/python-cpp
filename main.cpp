#include <iostream>
#include <string>
#include <typeinfo>

/*
/usr/include/boost/bind.hpp:36:1: note: ‘#pragma message: The practice of declaring the Bind placeholders (_1, _2, ...) in the global namespace is deprecated. Please use <boost/bind/bind.hpp> + using namespace boost::placeholders, or define BOOST_BIND_GLOBAL_PLACEHOLDERS to retain the current behavior.’
   36 | BOOST_PRAGMA_MESSAGE(
      | ^~~~~~~~~~~~~~~~~~~~
*/
#define BOOST_BIND_GLOBAL_PLACEHOLDERS // 上記警告メッセージ抑制のための対処

#include <boost/python.hpp>

namespace bpy = boost::python;

int foo() {
    std::cout << "hello world" << std::endl;
    return 0;
}

using PyObj = bpy::api::object;
using PyList = bpy::list;
using PyDict = bpy::dict;
using PyTuple = bpy::tuple;

PyObj py_extract(PyObj py_value, int deps = 0);

static constexpr const int kIndentSize = 4;

#include <boost/core/demangle.hpp>
template<typename T>
PyObj extract(PyObj py_value, int deps = 0) {
    const auto extracted = bpy::extract<T>(py_value);
    if (!extracted.check()) {
        return PyObj();
    }
    const auto value = static_cast<T>(extracted);
    std::cout << std::string(deps * kIndentSize, ' ') << "pyobj is " <<  boost::core::demangle(typeid(T).name())  << " : " << value << std::endl;
    return PyObj(value);
}

template<typename OP, const char*& TYPE>
PyObj extract_list(PyObj py_value, int deps = 0) {
    // list
    const auto extracted = bpy::extract<OP>(py_value);
    if (!extracted.check()) {
        return PyObj();
    }
    std::cout << std::string(deps * kIndentSize, ' ') << "pyobj is " << TYPE << ".";
    const auto py_list = extracted();
    auto size = bpy::len(py_list);
    std::cout << " size : " << size << std::endl;
    auto current_deps = ++deps;
    for (auto i = 0U; i < size; ++i) {
        std::cout << std::string(current_deps * kIndentSize, ' ') << "[" << i << "] : " << std::endl;
        py_extract(py_list[i], current_deps + 1);
    }
    return py_value;
}

template<>
PyObj extract<PyList>(PyObj py_value, int deps) {
    static const char* name = "list";
    return extract_list<PyList, name>(py_value, deps);
}

template<>
PyObj extract<PyDict>(PyObj py_value, int deps) {
    // dict
    const auto extracted = bpy::extract<PyDict>(py_value);
    if (!extracted.check()) {
        return PyObj();
    }
    std::cout << std::string(deps * kIndentSize, ' ') << "pyobj is " << "dict";
    const auto py_list = extracted().items();
    auto size = bpy::len(py_list);
    std::cout << " size : " << size << std::endl;
    auto current_deps = ++deps;
    for (auto i = 0U; i < size; ++i) {
        std::cout << std::string(current_deps * kIndentSize, ' ') << "[" << i << "] : " << std::endl;
        py_extract(py_list[i], current_deps + 1);
    }
    return py_value;
}

template<>
PyObj extract<PyTuple>(PyObj py_value, int deps) {
    static const char* name = "tuple";    
    return extract_list<PyTuple, name>(py_value, deps);
}


using Extractor = std::function<PyObj(PyObj, int)>;
PyObj py_extract(PyObj py_value, int deps) {
    if (py_value.is_none()) {
        std::cout << "pyobj is None." << std::endl;
        return PyObj();
    }

    for (auto func : std::vector<Extractor>({
        // extract<bool>,
        extract<int64_t>,// bool と 整数型は区別がつかない → 整数で指定して bool 型で検査すると精度が落ちるだけで bool 型として解釈可能
        extract<double>,
        extract<const char*>,
        // extract<std::string>,
        extract<PyList>,
        extract<PyDict>,
        extract<PyTuple>,

    })) {
        const auto value = func(py_value, deps);
        if (!value.is_none()) {
            return value;
        }
    }
    std::cout << "Not supported type." << std::endl; 
    return PyObj();
}
PyObj py_foo(PyObj py_value) {
    return py_extract(py_value, 0);
}

int64_t py_int(int64_t value) {
    std::cout << "py_int : " << value << std::endl;
    return value;
}

bool py_bool(bool value) {
    std::cout << "py_bool : " << value << std::endl;
    return value;
}

double py_float(double value) {
    std::cout << "py_float : " << value << std::endl;
    return value;
}

std::string py_str(const std::string &value) {
    std::cout << "py_str : " << value << std::endl;
    return value;
}

PyList py_list(size_t size) {
    std::cout << "py_list : " << size << std::endl;
    PyList list;
    for (size_t i = 0; i < size; ++i) {
        list.append(i);
    }
    return list;
}

PyObj py_apply(PyObj callable) {
    return bpy::call<PyObj>(callable.ptr(), 123);
}

BOOST_PYTHON_MODULE(foo) {
    // import 時に呼ばれる
    std::cout << "initialize." << std::endl;

    bpy::def("foo", foo);
    bpy::def("py_int", py_int);
    bpy::def("py_bool", py_bool);
    bpy::def("py_float", py_float);
    bpy::def("py_str", py_str);
    bpy::def("py_list", py_list);
    bpy::def("py_foo", py_foo);
    bpy::def("py_apply", py_apply);
}