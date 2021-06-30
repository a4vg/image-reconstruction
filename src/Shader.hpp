#ifndef SHADER_HPP
#define SHADER_HPP

class Shader {
public:
  static GLuint load(const char* vertex_file_path, const char* fragment_file_path);

  Shader(){};
  ~Shader(){};
};

#endif // SHADERHCPP