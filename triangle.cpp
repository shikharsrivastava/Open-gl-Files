#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

using namespace std;
int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
  GLFWwindow *window = glfwCreateWindow(800,600,"Triangles",NULL,NULL);
  if(window ==NULL)
{
  cout<<"CANNOT CREATE WIMDOW\n";
  glfwTerminate();
  return 0;
}
  glfwMakeContextCurrent(window);
  glViewport(0,0,800,600);

 /* Initialising glew for shader compilation*/
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
{
  cout <<"Failed to initialize GLEW"<<endl;
  return 0;
}
  

 
  const GLchar *vertexsource={ "#version 330 core\n"
			       "layout (location=0) in vec3 pos;\n"
			       "void main()\n"
			       "{\n"
			       "gl_Position=vec4(pos.x,pos.y,pos.z,1.0);\n"
			       "}\n\0"};
  const GLchar *fragmentsource={"#version 330 core\n"
				"out vec4 color;\n"
				"void main()\n"
				"{\n"
				"color=vec4(1.0f,0.2f,0.3f,0.4f);\n"
			  	"}\n\0"};

  GLuint vertex_shader,fragment_shader,shader_program,VBO,VAO;
  GLint success_vertex,success_fragment,success_linking;
  char info_vertex[512],info_fragment[512],info_linking[512];
 
  /* Creating vertex shader*/
  vertex_shader=glCreateShader(GL_VERTEX_SHADER);
  /* Binding vertex shadr to source code*/
  glShaderSource(vertex_shader,1,&vertexsource,NULL);
  /* Compiling vertex shader*/
  glCompileShader(vertex_shader);
  /* Checking compilation errors*/
  glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success_vertex);
  if(!success_vertex)
    {
      glGetShaderInfoLog(vertex_shader,512,NULL,info_vertex);
      cout<<"Cannot compile vertex shader : "<<info_vertex<<endl;
      return 1;
    }
  else
    cout<<"Vertex shader compilation successful"<<endl;



  /* Creating fragment shader*/
  fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
  /* Binding fraagment shader to source code*/
  glShaderSource(fragment_shader,1,&fragmentsource,NULL);
  /* Compiling fragment shader*/
  glCompileShader(fragment_shader);
  /* Checking for compilation errors*/
  glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&success_fragment);
  if(!success_fragment)
    {
      glGetShaderInfoLog(fragment_shader,512,NULL,info_fragment);
      cout<<"Cannot compile fragment shader : "<<info_fragment<<endl;
    }
  else
    cout<<"Fragment shader cmpilation successful"<<endl;

 
  /* Attaching both shaders together in shader program*/
  GLuint shaderprogram;
  shaderprogram=glCreateProgram();
  glAttachShader(shaderprogram,vertex_shader);
  glAttachShader(shaderprogram,fragment_shader);
  glLinkProgram(shaderprogram);

  /* Checking for linking errors*/
  glGetProgramiv(shaderprogram,GL_LINK_STATUS,&success_linking);
  if(!success_linking)
    {
      glGetProgramInfoLog(shaderprogram,512,NULL,info_linking);
      cout<<"Cannot link the shaders : "<<info_linking<<endl;
      return -1;
    }
  else
    cout<<"Linking successful"<<endl;


  GLfloat vertices[]={
    -0.5f,-0.5f,0.0f,
    0.0f,0.5f,0.0f,
    0.5f,-0.5f,0.0f};

  /* Creating vertex array object and vertex buffer object in gpu ram*/
  glGenBuffers(1,&VBO);
  glGenVertexArrays(1,&VAO);
  /* Binding the vbo with arraybuffer*/
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBindVertexArray(VAO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  glBindVertexArray(0);

 /* The game loop*/
  while(!glfwWindowShouldClose(window))
    {
      glfwPollEvents();
      glfwSwapBuffers(window);
      glUseProgram(shaderprogram);
      glBindVertexArray(VAO);
      glDrawArrays(GL_TRIANGLES,0,3);
      glBindVertexArray(0);
    }
  glfwTerminate();
  return 0;

}
