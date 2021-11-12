#ifndef RPG_SHADER_H
#define RPG_SHADER_H

#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "IGLObject.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

#define UNIFORMTAG(ph_1, ph_2) glUniform ## ph_1 ## ph_2

#define CallFuncs(type, separate) CallIfCallable(UNIFORMTAG(1, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    CallIfCallable(UNIFORMTAG(2, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    CallIfCallable(UNIFORMTAG(3, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate \
    CallIfCallable(UNIFORMTAG(4, type), glGetUniformLocation(ID, val_name.c_str()), std::forward<Params>(params)...) separate

#define CallFuncsForMatrix(type, separate) CallIfCallable(UNIFORMTAG(type, 2fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate \
    CallIfCallable(UNIFORMTAG(type, 3fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...)) separate  \
    CallIfCallable(UNIFORMTAG(type, 4fv), glGetUniformLocation(ID, val_name.c_str()), 1, GL_FALSE, glm::value_ptr(std::forward<Params>(params)...))

template <typename F, typename... S>
constexpr void CallIfCallable(F&& func, S&& ...args){
    if constexpr (std::is_invocable_v<F, decltype(std::forward<S>(args))...>){
        std::invoke(std::forward<F>(func), std::forward<S>(args)...);
    } else {
        return;
    }
}
template<typename T, typename... types>
struct get_types {
    using type1 = T;
    using type2 = void;
};

template<typename T, typename U, typename... types>
struct get_types<T, U, types...> {
    using type1 = T;
    using type2 = U;
};

class Shader : public IGLObject
{
private:
    unsigned int ID{};
public:
    Shader() = default;
    Shader(unsigned int id);

    // Активация шейдера
    void use() const;

    template <typename ... Params>
    void setUniform(const std::string &val_name, Params... params){
        if constexpr (std::is_same_v<typename get_types<Params...>::type1, float>) {
            if constexpr (std::is_pointer_v<typename get_types<Params...>::type2>) {
                CallFuncs(fv, ;);
            } else { CallFuncs(f, ;); }
        }
        else if constexpr (std::is_same_v<typename get_types<Params...>::type1, int>) {
            if constexpr (std::is_pointer_v<typename get_types<Params...>::type2>) {
                CallFuncs(iv, ;);
            } else { CallFuncs(i, ;); }
        }
        else if constexpr (std::is_same_v<typename get_types<Params...>::type1, glm::vec3>) {
            CallFuncs(f, ;);
        }
        else if constexpr (std::is_same_v<typename get_types<Params...>::type1, glm::mat4>){
            CallFuncsForMatrix(Matrix, ;);
        }
    }

    unsigned int getId() const noexcept override;

    void destroy() override;

    static Shader createShader(const std::string& vertexPath, const std::string& fragmentPath);

private:
    // Полезные функции для проверки ошибок компиляции/связывания шейдеров
    static unsigned int compileShader(const std::string& path, unsigned int type);

    static void checkCompileErrors(unsigned int glHandelr, unsigned int status, 
                                   void (*GLget)(unsigned int, unsigned int, int*),
                                   void (*GLinfoLog)(unsigned int, int, int*, char*));
};

#endif //RPG_SHADER_H
