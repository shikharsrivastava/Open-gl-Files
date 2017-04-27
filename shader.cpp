#include<iostream>
#include<LG/glew>
#include<GLFW/glfw3>
#include<fstream>
#include<sstream>

shader(GLchar* vertexsource,GLchar* fragmentsource)
{
 std::string vertexcode;
 std::string fragmentcode;
 std::ifstream vertexfile;
 std::ifstream fragmentfile;

 vertexfile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
 fragmentfile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
 try
{
 vertexfile.open(vertexsource);
 fragmentfile.open(fragmentsource);
 std:: stringstream vertexstream,fragmentstream;
 vertexstream<<vertexfile.rdbuf();
 fragmentstream<<fragmentfile.rdbuf();

 vertexfile.close();
 fragmentfile.close();
 vertexcode=vertexstream.str();
 fragmentcode=fragmentstram.str();
}

catch(std::ifstream::failure e)
{
std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::
endl;
}

const GLchar* vShaderCode = vertexcode.c_str();
const GLchar* fShaderCode = fragmentcode.c_str();

}
