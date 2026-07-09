#include "Shader.h"
#include "runtime/tool/Utils.h"
#include <iostream>
#include <string_view>

namespace hamu
{

    Shader Shader::default_shader;

    Shader::Shader(const fs::path& vertexPath, const fs::path& fragmentPath) {
        Create(vertexPath, fragmentPath);
    }

    void Shader::Use() {
        if (!CheckLinked()) return;
        glUseProgram(this->id_);
    }

    void Shader::Create(const fs::path& vertexPath, const fs::path& fragmentPath) {
        if (is_created_) {
            std::cerr << "Warning: Shader program already created." << std::endl;
            return;
        }

        std::string vertexCode {utils::ReadText(vertexPath)};
        std::string fragmentCode {utils::ReadText(fragmentPath)};

        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertexShader, fragmentShader;
        int success;
        char infoLog[512];

        // 顶点着色器
        vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "Error - Shader - Vertex - compile_failed\n" << infoLog << '\n';
        }


        fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
        glCompileShader(fragmentShader);
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "Error - Shader - Fragment - compile_failed\n" << infoLog << '\n';
        }

        // 着色器程序
        id_ = glCreateProgram();
        glAttachShader(id_, vertexShader);
        glAttachShader(id_, fragmentShader);
        glLinkProgram(id_);
        // 连接错误
        glGetProgramiv(id_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(id_, 512, NULL, infoLog);
            std::cerr << "Error - Shader - Program - linking_failed\n" << infoLog << '\n';
        }

        // 可以删除着色器了
        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        is_created_ = true;
    }

    void Shader::SetBool(const std::string_view& name, bool value) const {
        if (!CheckLinked()) return;
        glUniform1i(GetUniformLocation(name), (int)value);
    }

    void Shader::SetInt(const std::string_view& name, int value) const {
        if (!CheckLinked()) return;
        glUniform1i(GetUniformLocation(name), value);
    }

    void Shader::SetFloat(const std::string_view& name, float value) const {
        if (!CheckLinked()) return;
        glUniform1f(GetUniformLocation(name), value);
    }

    void Shader::SetMatrix4fv(const std::string_view& name, const float* value, size_t count, bool transpose) const {
        if (!CheckLinked()) return;
        glUniformMatrix4fv(
            GetUniformLocation(name), static_cast<GLsizei>(count), transpose ? GL_TRUE : GL_FALSE, value
        );
    }

    void Shader::SetUniformBlockBinding(const std::string_view& name, size_t bindingPoint) const {
        if (!CheckLinked()) return;
        glUniformBlockBinding(id_, GetUniformBlockIndex(name), bindingPoint);
    }

    GLint Shader::GetUniformLocation(const std::string_view& name) const {
        auto it = uniform_cache_.find(name.data());
        if (it != uniform_cache_.end()) {
            return it->second;
        }

        auto location = glGetUniformLocation(id_, name.data());
        if (location == -1) {
            std::cerr << "[warning] uniform '" << name << "' not found in shader program " << id_ << '\n';
            // 将错误 location 也进行记录
        }

        uniform_cache_[name.data()] = location;
        std::cout << "[info] getUniformLocation: " << "<" << name << "," << location << ">\n";

        return location;
    }

    GLint Shader::GetUniformBlockIndex(const std::string_view& name) const {
        auto it = uniform_cache_.find(name.data());
        if (it != uniform_cache_.end()) {
            return it->second;
        }

        auto location = glGetUniformBlockIndex(id_, name.data());
        if (location == GL_INVALID_INDEX) {
            std::cerr << "[warning] uniform index '" << name << "' not found in shader program " << id_ << '\n';
            // 将错误 location 也进行记录
        }

        uniform_cache_[name.data()] = location;
        std::cout << "[info] getUniformBlockIndex: " << "<" << name << "," << location << ">\n";

        return location;
    }

    bool Shader::CheckLinked() const noexcept {
        if (!is_created_) {
            std::cerr << "Error: Shader program not linked." << std::endl;
            return false;
        }
        return true;
    }

} // namespace hamu
