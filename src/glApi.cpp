#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

#include "glApi.hpp"
#include "utils.hpp"

Buffer::Buffer(GLenum target) : m_location(0), m_target(target), m_attributeSize(0) {
    glGenBuffers(1, &m_location);
}

Buffer::~Buffer() {
    glDeleteBuffers(1, &m_location);
}

void Buffer::bind() const {
    glBindBuffer(m_target, m_location);
}

void Buffer::unbind() const {
    glBindBuffer(m_target, 0);
}

uint Buffer::attributeCount() const {
    return m_attributeCount;
}

GLenum Buffer::attributeType() const {
    return m_attributeType;
}

uint Buffer::attributeSize() const {
    return m_attributeSize;
}

VAO::VAO(uint nbVBO) : m_location(0), m_vbos(nbVBO), m_ibo(GL_ELEMENT_ARRAY_BUFFER) {
    for (auto &vbo : m_vbos) {
        vbo = std::shared_ptr<Buffer>(new Buffer(GL_ARRAY_BUFFER));
    }

    int max = 16;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &max);
    assert(nbVBO <= (uint) max); // You may want to replace 16 by the real hardware limitation

    glGenVertexArrays(1, &m_location);
}

VAO::~VAO() {
    glDeleteVertexArrays(1, &m_location);
}

void VAO::bind() const {
    glBindVertexArray(m_location);
}

void VAO::unbind() const {
    glBindVertexArray(0);
}

void VAO::encapsulateVBO(unsigned int attributeIndex) const {
    bind();
    glEnableVertexAttribArray(attributeIndex);
    m_vbos[attributeIndex]->bind();
    glVertexAttribPointer(attributeIndex, m_vbos[attributeIndex]->attributeSize(),
                          m_vbos[attributeIndex]->attributeType(), GL_FALSE, 0, 0);
    m_vbos[attributeIndex]->unbind();
    unbind();
}

std::shared_ptr<VAO> VAO::makeSlaveVAO() const {
    unsigned int nbVBO = m_vbos.size();
    std::shared_ptr<VAO> slave(new VAO(nbVBO));
    slave->m_vbos = m_vbos;
    slave->bind();
    for (unsigned int attributeIndex = 0; attributeIndex < nbVBO; attributeIndex++) {
        slave->encapsulateVBO(attributeIndex);
    }
    slave->unbind();
    return slave;
}

void VAO::draw(GLenum mode) const {
    bind();
    glDrawElements(mode, m_ibo.attributeCount(), m_ibo.attributeType(), 0);
    unbind();
}

Shader::Shader(GLenum type, const std::string &filename) : m_location(0) {
    m_location = glCreateShader(type);
    std::string content = fileContent(filename);
    const GLchar *char_content = content.c_str();
    int length = content.length();
    glShaderSource(m_location, 1, &char_content, &length);
    glCompileShader(m_location);
}

Shader::~Shader() {
    glDeleteShader(m_location);
}

uint Shader::location() const {
    return m_location;
}

Program::Program(const std::string &vname, const std::string &fname) : m_location(0),
                                                                       m_vshader(GL_VERTEX_SHADER, vname),
                                                                       m_fshader(GL_FRAGMENT_SHADER, fname) {
    m_location = glCreateProgram();
    glAttachShader(m_location, m_vshader.location());
    glAttachShader(m_location, m_fshader.location());
    glLinkProgram(m_location);
    glDetachShader(m_location, m_vshader.location());
    glDetachShader(m_location, m_fshader.location());
}

Program::~Program() {
    glDeleteProgram(m_location);
}

void Program::bind() const {
    glUseProgram(m_location);
}

void Program::unbind() const {
    glUseProgram(0);
}

bool Program::getUniformLocation(const std::string &name, int &location) const {
    location = glGetUniformLocation(m_location, name.c_str());
    return location != -1;
}

template<>
void Program::uniformDispatcher(int location, const int &val) {
    glUniform1i(location, val);
}

template<>
void Program::uniformDispatcher(int location, const uint &val) {
    glUniform1ui(location, val);
}

template<>
void Program::uniformDispatcher(int location, const float &val) {
    glUniform1f(location, val);
}

template<>
void Program::uniformDispatcher(int location, const glm::vec2 &val) {
    glUniform2fv(location, 1, glm::value_ptr(val));
}

template<>
void Program::uniformDispatcher(int location, const glm::vec3 &val) {
    glUniform3fv(location, 1, glm::value_ptr(val));
}

template<>
void Program::uniformDispatcher(int location, const glm::vec4 &val) {
    glUniform4fv(location, 1, glm::value_ptr(val));
}

template<>
void Program::uniformDispatcher(int location, const glm::mat2 &val) {
    glUniformMatrix2fv(location, 1, GL_FALSE, glm::value_ptr(val));
}

template<>
void Program::uniformDispatcher(int location, const glm::mat3 &val) {
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(val));
}

template<>
void Program::uniformDispatcher(int location, const glm::mat4 &val) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(val));
}

bool Program::bound() const {
    int currentProgram;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return m_location == (GLuint) currentProgram;
}

Texture::Texture(GLenum target) : m_location(0), m_target(target) {
    glGenTextures(1, &m_location);
}

Texture::~Texture() {
    glDeleteTextures(1, &m_location);
}

void Texture::bind() const {
    glBindTexture(m_target, m_location);
}

void Texture::unbind() const {
    glBindTexture(m_target, 0);
}

template<>
void Texture::setData<GLubyte>(const Image<GLubyte> &image, bool mipmaps) const {
    bind();
    GLenum format_color;
    switch (image.channels) {
        case 1:
            format_color = GL_RED;
            break;
        case 2:
            format_color = GL_RG;
            break;
        case 3:
            format_color = GL_RGB;
            break;
        case 4:
            format_color = GL_RGBA;
            break;
        default:
            throw "Invalid format of color";
    }

    if (m_target == GL_TEXTURE_2D) {
        glTexImage2D(GL_TEXTURE_2D, 0, format_color, image.width, image.height, 0, format_color, GL_UNSIGNED_BYTE, image.data);
    } else if (m_target == GL_TEXTURE_3D) {
        glTexImage3D(GL_TEXTURE_3D, 0, format_color, image.width, image.height, image.depth, 0, format_color, GL_UNSIGNED_BYTE, image.data);
    }


    if (mipmaps) {
        glGenerateMipmap(m_target);
    }

    unbind();
}

Sampler::Sampler(int texUnit) : m_location(0), m_texUnit(texUnit) {
    glGenSamplers(1, &m_location);
}

Sampler::~Sampler() {
    glDeleteSamplers(1, &m_location);
}

void Sampler::bind() const {
    glBindSampler(m_texUnit, m_location);
}

void Sampler::unbind() const {
    glBindSampler(m_texUnit, 0);
}

void Sampler::attachToProgram(const Program &prog, const std::string &samplerName, BindOption bindOption) const {
    if (bindOption == BindUnbind) {
        prog.bind();
    }

    prog.setUniform(samplerName, m_texUnit);

    if (bindOption == BindUnbind) {
        prog.unbind();
    }

}

void Sampler::attachTexture(const Texture &texture) const {
     glActiveTexture(GL_TEXTURE0+m_texUnit);
     texture.bind();
}

template<>
void Sampler::setParameter<int>(GLenum paramName, const int &value) const {
     glSamplerParameteri(m_location, paramName, value);
}

template<>
void Sampler::setParameter<float>(GLenum paramName, const float &value) const {
    glSamplerParameterf(m_location, paramName, value);
}

void Sampler::enableAnisotropicFiltering() const {
    float max_anisotropic_filtering = 1.0f;
    glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &max_anisotropic_filtering);
    setParameter(GL_TEXTURE_MAX_ANISOTROPY_EXT, max_anisotropic_filtering);
}
