/************************************************************************/
/* 
这个项目给规定死了，必须是旗帜，10 * 10 的分割
不要想太多的东西，没有用的。

*/
/************************************************************************/
#include <GL/glut.h>
#include <gl/glext.h>
#include <gl/gltools.h>
#include <windows.h>
#include "ChangeState.h"
#include "math3d.h"
#define DIVIDE_PART 10

GLfloat ctrlPoints[10][10][3];
GLuint background_texture = NULL;

float RotX = 0;
float RotY = 0;
float RotZ = 0;

bool LoadTexture(LPSTR szFileName, GLuint &texid);

void myInit(void)
{
	for(int i=0;i<10;i++){
		for(int j=0;j<10;j++){
			ctrlPoints[i][j][0] = -4 + i*0.8;  //[-4, 3.6]
			ctrlPoints[i][j][1] = -4 + j*0.8;	//[-4, 3.6]
			ctrlPoints[i][j][2] = 0;
		}
	}

	initPointState();

	//光照
	GLfloat ambientLight[] = {0.9, 0.9, 0.9, 1.0};
	GLfloat diffuseLight[] = {0.7, 0.7, 0.7, 1.0};
	GLfloat specular[]     = {0.1, 0.1, 0.1, .1};
	GLfloat lightPos[] = {20.0f, 20.0f, 20, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT ,ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	

	LoadTexture("background.bmp", background_texture);
	glEnable(GL_TEXTURE_2D);
	glClearColor(0.0,1.0,1.0,1.0);//设置背景色
	glEnable(GL_MAP2_VERTEX_3);
	glFrontFace(GL_CW);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}

void drawGan(){
	GLUquadricObj *quadratic;
	quadratic=gluNewQuadric();
	glColor3f(1.0,1.0,0);
	gluCylinder(quadratic,0.30f,0.30f,20.0f,32,32);
}

void drawPoints(){
	glPointSize(1.0);
	glColor3f(1.0,0,0);
	glBegin(GL_POINTS);{
		for(int i=0;i<10;i++){
			for(int j=0;j<10;j++){
				glVertex3f(ctrlPoints[i][j][0], ctrlPoints[i][j][1], ctrlPoints[i][j][2]);
			}
		}
	}
}

/*绘制曲面*/
void myDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glMap2f(GL_MAP2_VERTEX_3,	// Type of data generated
		0.0f,						// Lower u range
		DIVIDE_PART,						// Upper u range
		3,							// Distance between points in the data
		10,							// Dimension in u direction (order)
		0.0f,						// Lover v range
		DIVIDE_PART,						// Upper v range
		30,							// Distance between points in the data
		10,							// Dimension in v direction (order)
		&ctrlPoints[0][0][0]			// array of control points
	);		

	glRotatef(RotX, 1.0,0,0);
	glRotatef(RotY, 0,1.0,0);
	glRotatef(RotZ, 0,0,1.0);
	
	//现在终于可以安心的贴图了

	glBindTexture(GL_TEXTURE_2D,background_texture);
	glBegin(GL_TRIANGLES);{
		glColor3f(1.0,1.0,1.0);
		for (int i=0;i<DIVIDE_PART - 1;i++)
		{
			//我想我是贴图的时候把图片给搞反掉了
			for(int j=0;j<DIVIDE_PART - 1;j++){
				//上三角
				glTexCoord2f(i/(DIVIDE_PART - 1.0), j/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)(i),(float)j);
				glTexCoord2f((i+1)/(DIVIDE_PART - 1.0), j/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)(i+1),(float)j);
				glTexCoord2f(i/(DIVIDE_PART - 1.0), (j+1)/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)i,(float)(j+1));

				//下三角
				glTexCoord2f(i/(DIVIDE_PART - 1.0), (j+1)/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)(i),(float)(j+1));
				glTexCoord2f((i+1)/(DIVIDE_PART - 1.0), j/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)(i+1),(float)j);
				glTexCoord2f((i+1)/(DIVIDE_PART - 1.0), (j+1)/(DIVIDE_PART - 1.0)); glEvalCoord2d((float)(i+1),(float)(j+1));
			}
		}
	}glEnd();
	//画一个杆子
	glTranslatef(-4.0,3.2,0);
	glRotatef(90, 1.0f,0,0);
	drawGan();
	glRotatef(-90, 1.0f,0,0);
	glTranslatef(4.0,-3.2,0);

	glRotatef(-RotX, 1.0,0,0);
	glRotatef(-RotY, 0,1.0,0);
	glRotatef(-RotZ, 0,0,1.0);
	glPopMatrix();
	glutSwapBuffers();
}


void changeSize(int w, int h){
	float VIEW_SIZE = 6;
	if(h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE, -VIEW_SIZE, VIEW_SIZE);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void timeFun(int value){
	changeNewCtrl(ctrlPoints);
	glutPostRedisplay();
	glutTimerFunc(100, timeFun, 1);
}


// Respond to arrow keys by moving the camera frame of reference
void SpecialKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'x':
		RotX = RotX + 1;
		break;
	case 'y':
		RotY = RotY + 1;
		break;
	case 'z':
		RotZ = RotZ + 1;
		break;
	default:
		break;
	}
	printf("\n the pos x: %f y: %f z: %f", RotX, RotY, RotZ);
	// Refresh the Window
	glutPostRedisplay();
}


int main(int argc,char ** argv)
{
	/*初始化*/
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(600,400);
	glutInitWindowPosition(200,200);
	glutCreateWindow("NURBS surface");
	glutTimerFunc(20, timeFun, 1);
	myInit();
	glutKeyboardFunc(SpecialKeys);
	glutReshapeFunc(changeSize);
	glutDisplayFunc(myDisplay);

	/*进入GLUT事件处理循环*/
	glutMainLoop();
	return(0);
}



//辅助函数
bool LoadTexture(LPSTR szFileName, GLuint &texid)      // Creates Texture From A Bitmap File
{
	HBITMAP hBMP;              // Handle Of The Bitmap
	BITMAP BMP;              // Bitmap Structure
	glGenTextures(1, &texid);           // Create The Texture
	hBMP=(HBITMAP)LoadImage(GetModuleHandle(NULL), szFileName, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE );
	if (!hBMP)               // Does The Bitmap Exist?
		return FALSE;             // If Not Return False
	GetObject(hBMP, sizeof(BMP), &BMP);         // Get The Object
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);        // Pixel Storage Mode (Word Alignment / 4 Bytes)
	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, texid);         // Bind To The Texture ID
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // Linear Min Filter
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Linear Mag Filter
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	DeleteObject(hBMP);             // Delete The Object
	return TRUE;              // Loading Was Successful
}
