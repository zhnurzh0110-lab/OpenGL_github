#include "Shader.h"
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

#ifndef SHADER_DIR
#define SHADER_DIR "shaders/"
#endif

namespace {

std::string readFile(const std::string& name)
{
    const std::string candidates[] = { "shaders/" + name, std::string(SHADER_DIR) + name, "../shaders/" + name };
    for (const auto& path : candidates) {
        std::ifstream f(path);
        if (f) {
            std::stringstream ss;
            ss << f.rdbuf();
            return ss.str();
        }
    }
    throw std::runtime_error("Cannot open shader file: " + name);
}

unsigned int compile(GLenum type, const std::string& src, const std::string& label)
{
    unsigned int s = glCreateShader(type);
    const char* c = src.c_str();
    glShaderSource(s, 1, &c, nullptr);
    glCompileShader(s);
    int ok = 0;
    glGetShaderiv(s, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetShaderInfoLog(s, sizeof(log), nullptr, log);
        throw std::runtime_error("Shader compile error in " + label + ":\n" + log);
    }
    return s;
}

} // namespace

Shader::Shader(const std::string& vertName, const std::string& fragName)
{
    unsigned int vs = compile(GL_VERTEX_SHADER, readFile(vertName), vertName);
    unsigned int fs = compile(GL_FRAGMENT_SHADER, readFile(fragName), fragName);
    id_ = glCreateProgram();
    glAttachShader(id_, vs);
    glAttachShader(id_, fs);
    glLinkProgram(id_);
    int ok = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[2048];
        glGetProgramInfoLog(id_, sizeof(log), nullptr, log);
        throw std::runtime_error(std::string("Shader link error:\n") + log);
    }
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() { if (id_) glDeleteProgram(id_); }

void Shader::use() const { glUseProgram(id_); }

int Shader::location(const std::string& n) const
{
    auto it = cache_.find(n);
    if (it != cache_.end()) return it->second;
    int loc = glGetUniformLocation(id_, n.c_str());
    cache_[n] = loc;
    return loc;
}

void Shader::setInt(const std::string& n, int v) const { glUniform1i(location(n), v); }
void Shader::setFloat(const std::string& n, float v) const { glUniform1f(location(n), v); }
void Shader::setVec3(const std::string& n, const glm::vec3& v) const { glUniform3fv(location(n), 1, glm::value_ptr(v)); }
void Shader::setMat3(const std::string& n, const glm::mat3& m) const { glUniformMatrix3fv(location(n), 1, GL_FALSE, glm::value_ptr(m)); }
void Shader::setMat4(const std::string& n, const glm::mat4& m) const { glUniformMatrix4fv(location(n), 1, GL_FALSE, glm::value_ptr(m)); }
