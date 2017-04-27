#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

void key_press(GLFWwindow* window,int key,int scancode,int action,int mode)
{
   if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
   glfwSetWindowShouldClose(window,GL_TRUE);
   
}

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

 /*Initialising glew for shader compilation*/
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
{
  cout <<"Failed to initialize GLEW"<<endl;
  return 0;
}
  /*Registering key callback function with glfw*/
  glfwSetKeyCallback(window,key_press);
  

 
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

  /* Declaratio  of all opengl object id's*/
  GLuint vertex_shader,fragment_shader,shader_program,VBO,VAO,EBO;
  GLint success_vertex,success_fragment,success_linking;
  char info_vertex[512],info_fragment[512],info_linking[512];
 
  /*Creating vertex shader*/
  vertex_shader=glCreateShader(GL_VERTEX_SHADER);
  /*Binding vertex shadr to source code*/
  glShaderSource(vertex_shader,1,&vertexsource,NULL);
  /*Compiling vertex shader*/
  glCompileShader(vertex_shader);
  /*Checking compilation errors*/
  glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success_vertex);
  if(!success_vertex)
    {
      glGetShaderInfoLog(vertex_shader,512,NULL,info_vertex);
      cout<<"Cannot compile vertex shader : "<<info_vertex<<endl;
      return 1;
    }
  else
    cout<<"Vertex shader compilation successful"<<endl;



  /*Creating fragment shader*/
  fragment_shader=glCreateShader(GL_FRAGMENT_SHADER);
  /*Binding fraagment shader to source code*/
  glShaderSource(fragment_shader,1,&fragmentsource,NULL);
  /*Compiling fragment shader*/
  glCompileShader(fragment_shader);
  /*Checking for compilation errors*/
  glGetShaderiv(fragment_shader,GL_COMPILE_STATUS,&success_fragment);
  if(!success_fragment)
    {
      glGetShaderInfoLog(fragment_shader,512,NULL,info_fragment);
      cout<<"Cannot compile fragment shader : "<<info_fragment<<endl;
    }
  else
    cout<<"Fragment shader cmpilation successful"<<endl;

 
  /*Attaching both shaders together in shader program*/
  GLuint shaderprogram;
  shaderprogram=glCreateProgram();
  glAttachShader(shaderprogram,vertex_shader);
  glAttachShader(shaderprogram,fragment_shader);
  glLinkProgram(shaderprogram);

  /*Checking for linking errors*/
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
    -0.5f,0.5f,0.0f,
    -0.5f,-0.5f,0.0f,
     0.5f,0.5f,0.0f,
     0.5f,-0.5f,0.0f};

  GLuint indices[]={
    0,1,3,
    0,2,3};
     

  /*Creating vertex array object,vertex buffer object,element buffer object in gpu ram*/
  glGenBuffers(1,&VBO);
  glGenVertexArrays(1,&VAO);
  glGenBuffers(1,&EBO);
  /*Binding the vbo with arraybuffer*/
  /*Binding vertex array to store element_array_buffer , array_buffer and attrribute pointer state info*/
  glBindVertexArray(VAO);
  /*Binding VBO with global array_buffer and copying data*/
  glBindBuffer(GL_ARRAY_BUFFER,VBO);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_DYNAMIC_DRAW);
  /*Binding EBO with global element_array_buffer and copying data*/
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_DYNAMIC_DRAW); 
  /*Attribute pointer to instruct shader how to use data from buffer*/
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  /*Unbinding vertex array to store state information*/
  glBindVertexArray(0);


  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
 /*The game loop*/
  while(!glfwWindowShouldClose(window))
    {

      /*Checking for our polling events and calling callbacks*/
      glfwPollEvents();
      glfwSwapBuffers(window);
      /*Making the gpu's graphics pipeline use our created shader progeam*/
      glUseProgram(shaderprogram);
      /*Binding the VAO to use predefined state information*/
      glBindVertexArray(VAO);
      /*Drawing elements for preventing overlap*/
      glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
      glBindVertexArray(0);
    }
  glfwTerminate();
  return 0;

}
