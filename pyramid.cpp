#include<GL/glut.h>
void Init(void){
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-1.0f,1.0f,-1.0f,1.0f,2.0f,100.0f);//Ϊ͸��ͶӰ����һ������//
   glTranslatef(0.0f,0.0f,-4.0f);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
   glRotatef(-90.0, -90, -150.0, 1.0);
   glEnable(GL_DEPTH_TEST);//������Ȼ���//
   glDepthFunc(GL_LEQUAL);//����������ȵ�����//
}
void Reshape(GLsizei w,GLsizei h)
{
   glViewport(0,0,w,h);
}
void Display(void){
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glBegin(GL_TRIANGLE_FAN);//���ӳ����ε�������ϵ��//
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
   glutSwapBuffers();//����˫����ģʽ����һ��glut������opengl����???�����Ĺ����漰�봰��ϵͳ�Ľ��������ڳ�ʼ���׶�����˫����ģʽ//
}
int main(int argc,char**argv)
{
   glutInit(&argc,argv);
   glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
   glutInitWindowSize(600,600);
   glutInitWindowPosition(100,100);
   glutCreateWindow("OpenGL����׶");
   Init();
   glutDisplayFunc(Display);
   glutReshapeFunc(Reshape);
   glutIdleFunc(Idle);//���лص�����,���¼�����Ϊ��ʱ�ŵõ�ִ��//
   glutMainLoop();
   return 0;
}