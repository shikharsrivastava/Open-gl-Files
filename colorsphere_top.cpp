#include<iostream>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include<cmath>
#include<cstdlib>
using namespace std; 

//int no_sphere;
//int m[5];
 
class Sphere
{
GLfloat* vertices;
GLuint *indices;
float xcenter,ycenter,zcenter,r;
int x,y;
public:
int sphere();
void value(GLfloat* v,GLuint* i,float xc,float yc,float zc,float rad,int plane,int zplane)
{
vertices=v;
indices=i;
xcenter=xc;
ycenter=yc;
zcenter=zc;
r=rad;
x=plane;
y=zplane;
}
};


void key_press(GLFWwindow* window,int key,int scancode,int action,int mode)
{
   if(key==GLFW_KEY_ESCAPE && action==GLFW_PRESS)
   glfwSetWindowShouldClose(window,GL_TRUE);
   
}


int main()
{

  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
  glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE,GL_TRUE);
  GLFWwindow *window = glfwCreateWindow(1280,720,"Triangles",NULL,NULL);
  if(window ==NULL)
{
  cout<<"CANNOT CREATE WIMDOW\n";
  glfwTerminate();
  return 0;
}
  glfwMakeContextCurrent(window);
  glViewport(0,0,1280,720);

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
				"out vec4 color;uniform vec4 colour;\n"
				"void main()\n"
				"{\n"
				"color=colour;\n"
			  	"}\n\0"};


  /* Declaration  of all opengl object id's*/  
  GLuint vertex_shader,fragment_shader,shader_program;
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
	
	int no_sphere;
        cout<<"Enter the number of spheres\n";
	cin>>no_sphere;
	GLuint VBO[no_sphere],VAO[no_sphere],EBO[no_sphere];
        int point_per_plane[no_sphere],no_of_plane[no_sphere],indice_count[no_sphere],m[no_sphere];
	float x[no_sphere],y[no_sphere],z[no_sphere],radius[no_sphere];
	GLfloat vertices[no_sphere][100000];//vertice buffer
        GLuint indices[no_sphere][100000];//indices buffer
	Sphere sp[no_sphere];
	for(int i=0;i<no_sphere;i++)
        {
	cout<<"For sphere "<<i+1<<endl;
	cout<<"Enter point per plane and number of planes\n";
	cin>>point_per_plane[i]>>no_of_plane[i];
	cout<<"Enter the center of sphere and radius\n";
	cin>>x[i]>>y[i]>>z[i]>>radius[i];
	sp[i].value(vertices[i],indices[i],x[i],y[i],z[i],radius[i],point_per_plane[i],no_of_plane[i]);
	indice_count[i]=sp[i].sphere();
	m[i]=3;
	}
	


  /*Creating vertex array object,vertex buffer object,element buffer object in gpu ram*/
  glGenBuffers(no_sphere,VBO);
  glGenVertexArrays(no_sphere,VAO);
  glGenBuffers(no_sphere,EBO);
for(int i=0;i<no_sphere;i++)
{
  /*Binding the vbo with arraybuffer*/
  /*Binding vertex array to store element_array_buffer , array_buffer and attrribute pointer state info*/
  glBindVertexArray(VAO[i]);
  /*Binding VBO with global array_buffer and copying data*/
  glBindBuffer(GL_ARRAY_BUFFER,VBO[i]);
  glBufferData(GL_ARRAY_BUFFER,sizeof(vertices[i]),vertices[i],GL_DYNAMIC_DRAW);
  /*Binding EBO with global element_array_buffer and copying data*/
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO[i]);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices[i]),indices[i],GL_DYNAMIC_DRAW); 
  /*Attribute pointer to instruct shader how to use data from buffer*/
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(GL_FLOAT),(GLvoid*)0);
  glEnableVertexAttribArray(0);
  /*Unbinding vertex array to store state information*/
  glBindVertexArray(0);
}



  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

 /*The game loop*/
  while(!glfwWindowShouldClose(window))
    {

      /*Checking for our polling events and calling callbacks*/
      glfwPollEvents();
      glfwSwapBuffers(window);
      /*Making the gpu's graphics pipeline use our created shader progeam*/
      glUseProgram(shaderprogram);

	
      GLfloat timeValue = glfwGetTime();
      GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
      GLfloat redValue = (cos(timeValue) / 2) + 0.5;
      GLint vertexColorLocation = glGetUniformLocation(shaderprogram,"colour");
      glUniform4f(vertexColorLocation, redValue, greenValue, 0.0f, 1.0f);     

	for(int i=0;i<no_sphere;i++)
{
	
      /*Binding the VAO to use predefined state information*/
      glBindVertexArray(VAO[i]);
      /*Drawing elements for preventing overlap*/
 	glDrawElements(GL_TRIANGLES,m[i],GL_UNSIGNED_INT,0);      
	glBindVertexArray(0);
	if(m[i]<=indice_count[i])
	m[i]+=3;
}

    

    }
  glfwTerminate();
  return 0;

}

int Sphere::sphere()
{
int index=0,index2=0,l,n,i,j;
float xcord,ycord,zcord,theta=0,phi=0,dtheta,dphi;

dtheta=360/x;
dphi=90/y;
for(phi=-90;phi<=90;phi+=dphi)
{
for(theta=0;theta<360;theta+=dtheta)
{
zcord=zcenter+r*sin((phi/180)*3.14);
xcord=xcenter+r*cos((phi/180)*3.14)*cos((theta/180)*3.14);
ycord=ycenter+r*cos((phi/180)*3.14)*sin((theta/180)*3.14);

vertices[index++]=xcord;
vertices[index++]=ycord;
vertices[index++]=zcord;
}
}

//wrapping the base of hemisphere

// wrapping the base line (one pole) with half triangles
for(i=0;i<x;i+=2)
{
l=n=1;
if(i==0)
{
n=1-x;
}
indices[index2++]=i;
indices[index2++]=i+l;
indices[index2++]=i+x;

indices[index2++]=i;
indices[index2++]=i-n;
indices[index2++]=i+x;
}

// full scale hemisphere wrapping
for(i=x+1,j=1;i<=x*y*2;i+=2,j++)
{

l=n=1;
if(j==x+1)
{
i++;
j=1;
}
if(j==x/2+1)
{
i--;
}
//checking boundary conditions
if((i+1)%x==0)
{
l=1-x;
n=1;
}
else if(i%x==0)
{
l=1;
n=1-x;
}
indices[index2++]=i;
indices[index2++]=i+l;
indices[index2++]=i+x;

indices[index2++]=i;
indices[index2++]=i+l;
indices[index2++]=i-x;

indices[index2++]=i;
indices[index2++]=i-n;
indices[index2++]=i+x;

indices[index2++]=i;
indices[index2++]=i-n;
indices[index2++]=i-x;
}

return index2-1;
}
