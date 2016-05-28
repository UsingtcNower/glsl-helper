#include<GL/glut.h>
void Init(void){
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0f,1.0f,-1.0f,1.0f,2.0f,100.0f);//为透视投影定义一个矩阵//
   glTranslatef(0.0f,0.0f,-4.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(-90.0, -90, -150.0, 1.0);
   glEnable(GL_DEPTH_TEST);//启用深度缓存//
   glDepthFunc(GL_LEQUAL);//所作测试深度的类型//
}
void Reshape(GLsizei w,GLsizei h)
{
   glViewport(0,0,w,h);
}
void Display(void){
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glBegin(GL_TRIANGLE_FAN);//连接成扇形的三角形系列//
   glColor3f(1.0f,0.0f,0.0f);glVertex3f(0.0f,1.0f,0.0f);
   glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f,-1.0f,-1.0f);
   glColor3f(0.0f,0.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);
   glColor3f(0.0f,1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);
   glColor3f(0.0f,0.0f,1.0f);glVertex3f(-1.0f,-1.0f,1.0f);
   glColor3f(0.0f,1.0f,0.0f);glVertex3f(-1.0f,-1.0f,-1.0f);
   glEnd();
   glBegin(GL_QUADS);
   glColor3f(1.0f,1.0f,0.0f);glVertex3f(-1.0f,-1.0f,-1.0f);
   glColor3f(0.0f,0.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);
   glColor3f(0.0f,1.0f,0.0f);glVertex3f(1.0f,-1.0f,1.0f);
   glColor3f(0.0f,1.0f,1.0f);glVertex3f(-1.0f,-1.0f,1.0f);
   glEnd();
   glRotatef(0.05f,0.0f,0.05f,0.0f);
}
void Idle(void)
{
   Display();
   glFlush();
   glutSwapBuffers();//请求双缓存模式，是一个glut函数非opengl函数???交换的过程涉及与窗口系统的交互必须在初始化阶段启用双缓存模式//
}
int main(int argc,char**argv)
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
   glutInitWindowSize(600,600);
   glutInitWindowPosition(100,100);
   glutCreateWindow("OpenGL四棱锥");
   Init();
   glutDisplayFunc(Display);
   glutReshapeFunc(Reshape);
   glutIdleFunc(Idle);//空闲回调函数,当事件队列为空时才得到执行//
   glutMainLoop();
   return 0;
}