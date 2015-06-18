#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define COUNT1 10
#define COUNT2 10
#define PI 3.1415926534

float angle = 0.0;
float j1bcz[10][3];
float j2bcz[10][3];
float bcz1[3] = {0}, bcz2[3] = {0};

void initPointState(){
	srand(time(0));
	//我们要控制靠近旗杆的点不变，所以
	for(int i=0;i<10;i++){
		for(int j=0;j<3;j++){
			j1bcz[i][j] = (rand() % 2) / 100.0f - 0.005;
			j2bcz[i][j] = (rand() % 2) / 100.0f - 0.005;
		}
	}
}


/*保持旗帜的一一个边不动，给旗帜一点补偿值*/
void getJaoBcz(){
	for(int i=0;i<3;i++){
		bcz1[i] = 0;
		bcz2[i] = 0;
	}

	for(int i=0;i< 9;i++){
		for(int j=0;j<3;j++){
			bcz1[j] += j1bcz[i][j] * (10 - i) / 10.0f;
			j1bcz[i][j] = j1bcz[i+1][j];
			bcz2[j] += j2bcz[i][j] * (10 - i) / 10.0f;
			j2bcz[i][j] = j2bcz[i+1][j];
		}
		
	}
	for(int i=0;i<3;i++){
		j1bcz[9][i] = (rand() %2) / 100.0f - 0.005;
		j2bcz[9][i] = (rand() %2) / 100.0f - 0.005;
		if(bcz1[i] > 0.1f || bcz1[i] < -0.1f){
			bcz1[i] = bcz1[i] > 0 ? 0.1 : -0.01;
		}
		if(bcz2[i] > 0.1f || bcz2[i] < -0.1f){
			bcz2[i] = bcz2[i] > 0 ? 0.1 : -0.01;
		}
	}
}

float CalValue(int x, int y, int z){
	if(x == 0){
		return 0;
	}
	//x 的补偿值
	float bx = 0.01 * sin(2 * angle + 2 * PI * x / COUNT1);
	//x 的补偿值
	float by = 0.01 * sin(2 * angle + 2 * PI * x / COUNT1);
	//x 的补偿值
	float bz = 0.01 * sin(2 * angle + 2 * PI * x / COUNT1);
	
	float reslut = 0.05 * sin(2 * angle + 2 * PI * x / COUNT1) + 0.01 * cos(5 * angle + 2 * PI * x / COUNT1) + bx + by + bz;

	return reslut;
}

void changeNewCtrl(GLfloat ctrlPoints[COUNT1][COUNT2][3]){
	for(int i=0;i<COUNT1;i++){
		for(int j=0;j<COUNT2;j++){
			ctrlPoints[i][j][0] = CalValue(i,j,0) + ctrlPoints[i][j][0];
			ctrlPoints[i][j][1] = CalValue(i,j,1) + ctrlPoints[i][j][1];
			ctrlPoints[i][j][2] = CalValue(i,j,2) + ctrlPoints[i][j][2];
		}
	}
	getJaoBcz();
	for(int i=0;i<3;i++){
		ctrlPoints[9][0][i] += bcz1[i];
		ctrlPoints[9][9][i] += bcz2[i];
	}
	angle = angle + PI/ 20;	
}
