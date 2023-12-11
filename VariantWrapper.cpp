#include <emscripten/val.h>
#include <emscripten/bind.h>
#include <emscripten/emscripten.h>

using namespace emscripten;

class VariantWrapper {
public:
    emscripten::val value;

    template <typename T>
    VariantWrapper(const T& v) : value(emscripten::val::object()) {
        setValue(v);
    }

    template <typename T>
    void setValue(const T& v) {
        value.set("type", typeid(T).name());
        value.set("data", emscripten::val(v));
    }

    template <typename T>
    T getValue() const {
        if (value["type"].as<std::string>() != typeid(T).name()) {
            // Handle type mismatch error
            // You can throw an exception or return a default value as needed
        }
        return value["data"].as<T>();
    }
};

EMSCRIPTEN_BINDINGS(variant_wrapper) {
    class_<VariantWrapper>("VariantWrapper")
        .constructor<const emscripten::val&>()
        .function("setValue", &VariantWrapper::setValue<int>)  // Adjust to support other types
        .function("getValue", &VariantWrapper::getValue<int>);  // Adjust to support other types
}
