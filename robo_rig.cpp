/**************************************************
 *						  *
 * Animated Robot with OpenGL.			  *
 *					 	  *
 *       by					  *
 *					 	  *
 * Vlasis Gogousis [vgogousis@gmail.com]	  *
 * Kiki Hatzistavrou [kikihatzistavrou@gmail.com] *
 *						  *
 * Aristotle University of Thessaloniki, 2014	  *
 *						  *
 **************************************************/
//���������ע�Ͳ��ֶ��Ƕ������Ĵ���
#define _CRT_SECURE_NO_WARNINGS
#include <math.h>
#include <stdio.h>
#include <vector>
#include <GL/glut.h>
#include <stdlib.h>

#define STEP 0.05
#define TORSO_HEIGHT 6.0
#define TORSO_RADIUS 2.0
#define UPPER_ARM_HEIGHT 3.0
#define UPPER_ARM_RADIUS  0.5
#define LOWER_ARM_HEIGHT 2.5
#define LOWER_ARM_RADIUS  0.5
#define UPPER_LEG_HEIGHT 4.0
#define UPPER_LEG_RADIUS  0.6
#define LOWER_LEG_HEIGHT 3.5
#define LOWER_LEG_RADIUS  0.6
#define ELBOW_RADIUS 0.6
#define KNEE_RADIUS 0.7
#define HAND_RADIUS 0.6
#define FOOT_RADIUS 0.7
#define SHOULDER_RADIUS 0.8
#define HEAD_HEIGHT 1.5
#define HEAD_RADIUS 1.0
#define NECK_HEIGHT 1.0
#define NECK_RADIUS 0.4

#define NULL 0

int win_width = 800;
int win_height = 750;
int option = -1;
int animation = -3; //teleytaio animation
int done = 0; //an oloklirwthike to teleytaio animation
double rotate = -20;
double horizontal = 0.03 * STEP;
//�������������ƶ���ǰ���ƶ�
double vertical = -0.03 * STEP;
double backward = 0;

void head();
void torso();
void left_upper_arm();
void right_upper_arm();
void left_upper_leg();
void right_upper_leg();

typedef float point[3];


typedef struct treenode
{
	GLfloat m[16];
	void(*f)();
	struct treenode* sibling;
	struct treenode* child;
}treenode;

//���task����װÿһ����ת����ϸ��
class Task
{
public:
	void(*f)();
	bool isDone;
	Task(void(*f)(), bool isDone) {
		this->f = f;
		this->isDone = isDone;
	}
};

//��װö�٣��򻯲����Ĵ���
enum Position
{
	BODY, NECK, HEAD, LUA, LLA, RUA, RLA, LUL, LLL, RUL, RLL,
};


typedef treenode* t_ptr;

//������С
static GLfloat steps[11] = { STEP, STEP, STEP ,STEP,STEP,STEP,STEP,STEP,STEP,STEP,STEP};

//��ת��Ŀ�����
static GLfloat theta[11] = { 0.0,0.0,0.0,180.0,0.0,180.0,0.0,
180.0,0.0,180.0,0.0};

//�������ά��ת�Ķ���
/*static GLfloat thetas[3][11] = {0.0,0.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0,
								0.0,0.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0,
								0.0,0.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0,180.0,0.0}; */

static GLint angle = 2;

GLUquadricObj* t, * h, * lua, * lla, * rua, * rla, * lll, * rll, * rul, * lul;
GLUquadricObj* relb, * lelb, * rknee, * lknee, * nk, * lhand, * rhand, * lfoot, * rfoot, * rsh, * lsh;

double size = 1.0;

treenode torso_node, head_node, lua_node, rua_node, lll_node, rll_node,
lla_node, rla_node, rul_node, lul_node,
relb_node, lelb_node, rknee_node, lknee_node, nk_node, lhand_node, rhand_node, lfoot_node, rfoot_node,
rsh_node, lsh_node;

void traverse(treenode* root)
{
	if (root == NULL) return;
	glPushMatrix();
	glMultMatrixf(root->m);
	root->f();
	if (root->child != NULL) {
		traverse(root->child);
	}
	glPopMatrix();
	if (root->sibling != NULL) traverse(root->sibling);
}

void torso()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(t, TORSO_RADIUS / 1.3, TORSO_RADIUS, TORSO_HEIGHT, 10, 10);
	glPopMatrix();
}

void head()
{
	glPushMatrix();
	glTranslatef(0.0, HEAD_HEIGHT, 0.0);
	glScalef(HEAD_RADIUS, HEAD_HEIGHT, HEAD_RADIUS);
	gluSphere(h, HEAD_RADIUS, 10, 10);
	//glasses
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	glTranslatef(0.0f, -0.4f, -0.15f);
	gluCylinder(h, 0.9 * HEAD_RADIUS, 0.9 * HEAD_RADIUS, HEAD_HEIGHT / 5, 10, 10);
	glPopMatrix();
}

void neck()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(nk, NECK_RADIUS, NECK_RADIUS, NECK_HEIGHT, 10, 10);
	glPopMatrix();
}

void rightShoulder()
{
	glPushMatrix();
	gluSphere(relb, SHOULDER_RADIUS, 10, 10);
	glPopMatrix();
}

void leftShoulder()
{
	glPushMatrix();
	gluSphere(lelb, SHOULDER_RADIUS, 10, 10);
	glPopMatrix();
}

void rightElbow()
{
	glPushMatrix();
	gluSphere(relb, ELBOW_RADIUS, 10, 10);
	glPopMatrix();
}

void leftElbow()
{
	glPushMatrix();
	gluSphere(lelb, ELBOW_RADIUS, 10, 10);
	glPopMatrix();
}

void rightKnee()
{
	glPushMatrix();
	gluSphere(rknee, KNEE_RADIUS, 10, 10);
	glPopMatrix();
}

void leftKnee()
{
	glPushMatrix();
	gluSphere(lknee, KNEE_RADIUS, 10, 10);
	glPopMatrix();
}

void leftFoot()
{
	glPushMatrix();
	gluSphere(lknee, FOOT_RADIUS, 10, 10);
	glPopMatrix();
}

void rightFoot()
{
	glPushMatrix();
	gluSphere(lknee, FOOT_RADIUS, 10, 10);
	glPopMatrix();
}

void rightHand()
{
	glPushMatrix();
	gluSphere(lknee, HAND_RADIUS, 10, 10);
	glPopMatrix();
}

void leftHand()
{
	glPushMatrix();
	gluSphere(lknee, HAND_RADIUS, 10, 10);
	glPopMatrix();

}

void left_upper_arm()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_arm()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lla, LOWER_ARM_RADIUS - 0.1, LOWER_ARM_RADIUS - 0.15, LOWER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_arm()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rua, UPPER_ARM_RADIUS, UPPER_ARM_RADIUS - 0.1, UPPER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_arm()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rla, LOWER_ARM_RADIUS - 0.1, LOWER_ARM_RADIUS - 0.15, LOWER_ARM_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_upper_leg()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void left_lower_leg()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(lll, LOWER_LEG_RADIUS - 0.1, LOWER_LEG_RADIUS - 0.2, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_upper_leg()
{

	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rul, UPPER_LEG_RADIUS, UPPER_LEG_RADIUS - 0.1, UPPER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void right_lower_leg()
{
	glPushMatrix();
	glRotatef(-90.0, 1.0, 0.0, 0.0);
	gluCylinder(rll, LOWER_LEG_RADIUS - 0.1, LOWER_LEG_RADIUS - 0.2, LOWER_LEG_HEIGHT, 10, 10);
	glPopMatrix();
}

void display(void)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-15, 15, -15, 15, -15, 15);
	glRotatef(rotate, 0, 1, 0);
	glMatrixMode(GL_MODELVIEW);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glColor3f(0.4, 0.4, 0.4);
	traverse(&torso_node);

	glutSwapBuffers();
}

// ���øú�����ʹ���������˶�
void act(enum Position pos ,double x, double y, double z) {
	glPushMatrix();
	switch (pos)
	{
	case RUL:
		glRotatef(steps[9], x, y, z);
		glMultMatrixf(rul_node.m);
		theta[9] += steps[9];
		glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);
		break;
	case RLL:
		glRotatef(steps[10], x, y, z);
		glMultMatrixf(rll_node.m);
		theta[10] += steps[10];
		glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);
		break;
	case RUA:
		glRotatef(steps[5], x, y, z);
		glMultMatrixf(rua_node.m);
		theta[5] += steps[5];
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
		break;
	case RLA:
		glRotatef(steps[6], x, y, z);
		glMultMatrixf(rla_node.m);
		theta[6] += steps[6];
		glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);
		break;
	case LUA:
		glRotatef(steps[3], x, y, z);
		glMultMatrixf(lua_node.m);
		theta[3] += steps[3];
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
		break;
	case LLA:
		glRotatef(steps[4], x, y, z);
		glMultMatrixf(lla_node.m);
		theta[4] += steps[4];
		glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);
		break;
	case LUL:
		glRotatef(steps[7], x, y, z);
		glMultMatrixf(lul_node.m);
		theta[7] += steps[7];
		glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);
		break;
	case LLL:
		glRotatef(steps[8], x, y, z);
		glMultMatrixf(lll_node.m);
		theta[8] += steps[8];
		glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);
		break;
	default:
		break;
	}
	glPopMatrix();
}

// ���øú�����ʹ���������˶�
void bodyAct(int x=0, int y=0, int z=0) {
	glPushMatrix();
	glTranslatef(horizontal, vertical, backward);
	glRotatef(steps[0], x, y, z);
	glMultMatrixf(torso_node.m);
	theta[0] += steps[0];
	glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);
	glPopMatrix();
}

// ���ò�����Ŀ��Ƕ�
void resetAngle() {
	for (int i = 0; i < 11; i++) {
		theta[i] = 0;
		steps[i] = STEP;
	}
	steps[0] = 0;
	horizontal = 0.03 * STEP;
	//�������������ƶ���ǰ���ƶ�
	vertical = -0.02 * STEP;
	backward = 0;
}

void inverseKinematics()
{
	switch (animation)
	{
	case 0: //an to teleytaio animation einai to 0 antistrepse to
		if (theta[5] < 180.0)
		{
			theta[5] += STEP;
			theta[3] += STEP;
			theta[1] -= 0.2 * STEP;
		}
		else animation = option; //an exei antistrafei tote eimaste stin arxiki thesi kai to neo animation einai to option

		glPushMatrix();

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[5], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[3], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

		glLoadIdentity();
		glTranslatef(0.0, TORSO_HEIGHT - 0.25 * NECK_HEIGHT, 0.0);
		glRotatef(theta[1], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, nk_node.m);

		glPopMatrix();
		break;
	case 1:
		if (theta[9] < 180.0)
		{
			theta[9] += STEP;
			theta[10] -= STEP;
		}
		else animation = option;

		glPushMatrix();

		glLoadIdentity();
		glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
		glRotatef(theta[9], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

		glLoadIdentity();
		glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rknee_node.m);

		glLoadIdentity();
		glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
		glRotatef(theta[10], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

		glPopMatrix();
		break;
	case 6: case -1:
		if (theta[9] < 180.0)
		{
			theta[9] += STEP;
			theta[10] -= STEP;
			theta[7] += STEP;
			theta[8] -= STEP;
			theta[5] -= 1.3 * STEP;
			theta[6] += STEP;
			theta[3] += STEP;
			theta[4] -= STEP;
			horizontal -= 0.03 * STEP;
		}
		else animation = option;

		glPushMatrix();

		glLoadIdentity();
		glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
		glRotatef(theta[9], 1.0, 0.0, -1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

		glLoadIdentity();
		glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
		glRotatef(theta[10], 1.0, 0.0, -1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

		glLoadIdentity();
		glTranslatef(0, horizontal, 0);
		glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[5], -1.0, 0.0, -1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[6], 1.0, 1.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[3], 1.0, 0.0, -1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[4], -1.0, -1.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);

		glPopMatrix();
		break;
	case 7: case -2:
		if (theta[9] < 180.0)
		{
			theta[9] += STEP;
			theta[10] -= STEP;
			theta[7] += STEP;
			theta[8] -= STEP;
			theta[5] -= STEP;
			theta[6] += STEP;
			theta[3] += STEP;
			theta[4] -= STEP;
			horizontal -= 0.03 * STEP;
		}
		else animation = option;

		glPushMatrix();

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[5], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[6], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[3], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

		glLoadIdentity();
		glTranslatef(0, 0, 0.0);
		glRotatef(theta[4], 0.0, 0.0, 1.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);

		glLoadIdentity();
		glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
		glRotatef(theta[9], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

		glLoadIdentity();
		glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
		glRotatef(theta[10], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

		glLoadIdentity();
		glTranslatef(-TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
		glRotatef(theta[7], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

		glLoadIdentity();
		glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
		glRotatef(theta[8], 1.0, 0.0, 0.0);
		glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

		glLoadIdentity();
		glTranslatef(0, -horizontal, -horizontal);
		glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

		glPopMatrix();
		break;
	default: animation = option;
	}
}

//����
// һ��ʼ���Ȳ��˶�
Task qishi0 = Task([]() {
		//����̧��
		//�˶��Ƕ������Լ��ؽ��˶��ٶ�
		//Ϊ�˼���Դ���룬���Ǵ�180�ȿ�ʼ
		steps[9] = -STEP;
		steps[10] = 1.25 * STEP;
		if (theta[9] <= 140.0)
		{
			resetAngle();
			qishi0.isDone = true;
		}
		//�˶���λ����
		act(RUL, 1, 0, 0);
		act(RLL, 1, 0, 0);
	}, false);
Task qishi1 = Task([]() {
	//��������
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[10] = 0.8 * STEP;
	if (theta[9] >= 25.0)
	{
		resetAngle();
		qishi1.isDone = true;
	}
	//�˶���λ����
	act(RUL, 1, 0, 1);
	act(RLL, -1, 0, 0);
	}, false);
Task qishi2 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[10] = 1.2 * STEP;
	vertical = -0.01 * STEP;
	if (theta[9] >= 25.0)
	{
		resetAngle();
		qishi2.isDone = true;
	}
	//�˶���λ����
	act(RUL, 1, 0, 0);
	act(RLL, -1, 0, 0);
	}, false);
Task qishi3 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	if (theta[7] >= 15.0)
	{
		resetAngle();
		qishi3.isDone = true;
	}
	//�˶���λ����
	act(LUL, 0, 0, -1);
	bodyAct();
	}, false);
// ��������ʼ�����ֱ��˶�
Task qishi4 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	if (theta[3] >= 80.0)
	{
		resetAngle();
		qishi4.isDone = true;
	}
	//�˶���λ����
	act(LUA, -1, 0, 0);
	act(RUA, -1, 0, 0);
	}, false);
Task qishi5 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[3] = 0.2 * STEP;
	steps[5] = 0.2 * STEP;
	if (theta[4] >= 50.0)
	{
		resetAngle();
		qishi5.isDone = true;
	}
	//�˶���λ����
	act(LLA, -1, 0, 0);
	act(RLA, -1, 0, 0);
	act(LUA, 1, 0, 0);
	act(RUA, 1, 0, 0);
	}, false);
Task qishi6 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[4] = 0.8 * STEP;
	steps[6] = 0.8 * STEP;
	backward = 0.00001 * STEP;
	horizontal = 0;
	if (theta[7] >= 35.0) {
		steps[7] = 0;
		steps[8] = 0;
		steps[9] = 0;
		steps[10] = 0;
		vertical = 0;
		backward = 0.00001 * STEP;
		horizontal = 0;
	}
	if (theta[4] >= 35.0) {
		steps[4] = 0;
		steps[6] = 0;
	}
	if (theta[3] >= 60.0)
	{
		resetAngle();
		qishi6.isDone = true;
	}
	//�˶���λ����
	act(LUA, 1, 0, 0);
	act(RUA, 1, 0, 0);
	act(LLA, 1, 0, 0);
	act(RLA, 1, 0, 0);
	act(LUL, -1, 0, 0);
	act(RUL, -1, 0, 0);
	act(LLL, 1, 0, 0);
	act(RLL, 1, 0, 0);
	bodyAct();
	}, false);

//Ұ�����
Task yema0 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	//steps[0] = 0.3 * STEP;
	steps[3] = 1.5 * STEP;
	steps[4] = 0.6 *STEP;
	steps[5] = 0.8 * STEP;
	steps[6] = 0.15 * STEP;
	steps[7] = STEP;
	steps[8] = 0.8* STEP;
	steps[9] = STEP;
	steps[10] =  STEP;
	horizontal = -0.03 * STEP;
	vertical = 0.001 * STEP;
	if (theta[7] > 35) {
		steps[7] = 0;
		steps[8] = 0;
		steps[9] = 0;
		steps[10] = 0;
		horizontal = 0;
		vertical = 0;
	}
	if (theta[5] >= 35) {
		steps[5] = 0;
		steps[6] = 0;
	}
	if (theta[3] >= 90.0)
	{
		resetAngle();
		yema0.isDone = true;
	}
	//�˶���λ����
	act(LUA, -0.7, 0, -1);
	act(LLA, 0, 0, -1);
	act(RUA, 0.8, 0, 1);
	act(RLA, 0, 0, -1);
	act(LUL, 1, 0, 1);
	act(LLL, 0, 0, 1);
	act(RUL, 1, 0, 1);
	act(RLL, -1, 0, 1);
	bodyAct(0,1,0);
	}, false);
Task yema1 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[3] = 1 * STEP;
	steps[4] = 1.2 * STEP;
	steps[5] = 0.5*STEP;
	steps[6] = 0.3*STEP;
	steps[9] = -0.2 * STEP;
	steps[10] = STEP;
	if (theta[10] >= 70.0)
	{
		resetAngle();
		yema1.isDone = true;
	}
	//�˶���λ����
	act(LUA, -0.3, 0, 1);
	act(LLA, 0.3, 0, -1);
	act(RUA, -0.6, 0, -1);
	act(RLA, 0, 0, 1);
	act(RUL, 1, 0, 0);
	act(RLL, 0, 0, 1);
	}, false);
Task yema2 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[0] = 0.8 * STEP;
	steps[3] = 1 * STEP;
	steps[4] = 1.2 * STEP;
	steps[5] = 0.5 * STEP;
	steps[6] = 0.5 * STEP;
	steps[7] = 0.2 * STEP;
	steps[8] = 0.6 * STEP;
	steps[9] = 0.2 * STEP;
	steps[10] = STEP;
	vertical = -0.012 * STEP;
	horizontal = 0;
	if (theta[10] >= 90.0)
	{
		resetAngle();
		yema2.isDone = true;
	}
	//�˶���λ����
	//act(LUA, -0.3, 0, 1);
	//act(LLA, 0.3, 0, -1);
	act(RUA, -1, 0, 0);
	act(RLA, 1, 0, 1);
	act(LUL, -1, 0, 0);
	act(LLL, 1, 0, 0);
	act(RUL, -1, 0, -1);
	act(RLL, -0.5, 0, -1);
	bodyAct(0, 1, 0);
	}, false);
Task yema3 = Task([]() {
	//�˶��Ƕ������Լ��ؽ��˶��ٶ�
	steps[0] = 0;
	steps[3] = 1 * STEP;
	steps[4] = 1.2 * STEP;
	steps[5] = 0.6 * STEP;
	steps[6] = 1 * STEP;
	steps[7] = 1 * STEP;
	steps[8] = 1.2 * STEP;
	steps[9] = 0.5 * STEP;
	steps[10] = STEP;
	vertical = 0;
	horizontal = 0;
	backward = -0.03 * STEP;
	if (theta[10] >= 70.0)
	{
		resetAngle();
		yema3.isDone = true;
	}
	//�˶���λ����
	act(LUA, 1, 0, -0.3);
	act(LLA, -0.3, 0, 1);
	act(RUA, -0.7, 0, 1);
	act(RLA, -0.5, 0, -1);
	act(LUL, 1, 0, -0.2);
	act(LLL, -1, 0, -0.4);
	act(RUL, -1, 0, -0.5);
	act(RLL, 1, 0, 0);
	bodyAct(0, 0, 0);
	}, false);


void idle()
{
	switch (option)
	{
	case 9: {
		//����
		std::vector<Task*> taskList;
		taskList.push_back(&qishi0);
		taskList.push_back(&qishi1);
		taskList.push_back(&qishi2);
		taskList.push_back(&qishi3);
		taskList.push_back(&qishi4);
		taskList.push_back(&qishi5);
		taskList.push_back(&qishi6);
		Task* undone = nullptr;
		//�ӵ�һ��δ��ɵ�����ʼִ��
		for (auto t : taskList) {
			if (!t->isDone) {
				undone = t;
				break;
			}
		}
		if (undone != nullptr) undone->f();
		break; }
	case 10: {
		//Ұ�����
		std::vector<Task*> taskList;
		taskList.push_back(&yema0);
		taskList.push_back(&yema1);
		taskList.push_back(&yema2);
		taskList.push_back(&yema3);
		Task* undone = nullptr;
		//�ӵ�һ��δ��ɵ�����ʼִ��
		for (auto t : taskList) {
			if (!t->isDone) {
				undone = t;
				break;
			}
		}
		if (undone != nullptr) undone->f();
		break;
	}
	case 0:
		if (animation != option) inverseKinematics(); //an to teleytaio animation einai diaforetiko antistrepse to
		else if (!done) //kane animation mexri na oloklirwthei
		{
			if (theta[5] > 30.0)
			{
				theta[5] -= STEP;
				theta[3] -= STEP;
				theta[1] += 0.2 * STEP;
			}
			else done = 1;

			glPushMatrix();

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[5], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[3], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

			glLoadIdentity();
			glTranslatef(0.0, TORSO_HEIGHT - 0.25 * NECK_HEIGHT, 0.0);
			glRotatef(theta[1], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, nk_node.m);

			glPopMatrix();
		}
		break;
	case 1:
		if (animation != option) inverseKinematics();
		else if (!done)
		{
			if (theta[9] > 110.0)
			{
				theta[9] -= STEP;
				theta[10] += STEP;
			}
			else done = 1;

			glPushMatrix();

			glLoadIdentity();
			glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
			glRotatef(theta[9], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

			glLoadIdentity();
			glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
			glRotatef(theta[10], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

			glLoadIdentity();
			glTranslatef(0, -horizontal, -horizontal);
			glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

			glPopMatrix();
		}
		break;
	case 2:
		inverseKinematics();
		break;
	case 6:
		if (animation != option) inverseKinematics();
		else
		{
			if (theta[9] > 130.0)
			{
				theta[9] -= STEP;
				theta[10] += STEP;
				theta[7] -= STEP;
				theta[8] += STEP;
				theta[5] += 1.3 * STEP;
				theta[6] -= STEP;
				theta[3] -= STEP;
				theta[4] += STEP;
				horizontal += 0.03 * STEP;
			}
			else animation = -1;

			glPushMatrix();

			glLoadIdentity();
			glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
			glRotatef(theta[9], 1.0, 0.0, -1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

			glLoadIdentity();
			glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
			glRotatef(theta[10], 1.0, 0.0, -1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

			glLoadIdentity();
			glTranslatef(0, horizontal, 0);
			glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[5], -1.0, 0.0, -1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[6], 1.0, 1.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[3], 1.0, 0.0, -1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[4], -1.0, -1.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);

			glPopMatrix();
		}
		break;
	case 7:
		if (animation != option) inverseKinematics();
		else
		{
			if (theta[9] > 130.0)
			{
				theta[9] -= STEP;
				theta[10] += STEP;
				theta[7] -= STEP;
				theta[8] += STEP;
				theta[5] += STEP;
				theta[6] -= STEP;
				theta[3] -= STEP;
				theta[4] += STEP;
				horizontal += 0.03 * STEP;
			}
			else animation = -2;

			glPushMatrix();

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[5], 0.0, 0.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[6], 0.0, 0.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[3], 0.0, 0.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);

			glLoadIdentity();
			glTranslatef(0, 0, 0.0);
			glRotatef(theta[4], 0.0, 0.0, 1.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);

			glLoadIdentity();
			glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
			glRotatef(theta[9], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);

			glLoadIdentity();
			glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
			glRotatef(theta[10], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);

			glLoadIdentity();
			glTranslatef(-TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
			glRotatef(theta[7], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);

			glLoadIdentity();
			glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
			glRotatef(theta[8], 1.0, 0.0, 0.0);
			glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);

			glLoadIdentity();
			glTranslatef(0, -horizontal, -horizontal);
			glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

			glPopMatrix();
		}
		break;
	case 8:
		//�������
		if (!done)
		{
			//ֱ���ϹؽڵĽǶȵ�110��
			if (theta[3] > 110.0)
			{
				theta[3] -= STEP;
				theta[4] += STEP;
			}
			else done = 1;

			glPushMatrix();

			//����act�����Ƿ���ã��Լ����Թؽڵ��˶���
			act(LUL, 1, 0, 0);
			act(LLL, 1, 0, 0);

			glLoadIdentity();
			glTranslatef(0, -horizontal, -horizontal);
			glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);

			glPopMatrix();
		}
		break;
	}
	glutPostRedisplay();
}

void menu(int id)
{
	option = id;
	done = 0;
	if (id == 3) rotate = -90;
	if (id == 4) rotate = 0;
	if (id == 5) rotate = -30;
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-10.0, 10.0, -10.0 * (GLfloat)h / (GLfloat)w,
			10.0 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-10.0 * (GLfloat)w / (GLfloat)h,
			10.0 * (GLfloat)w / (GLfloat)h, 0.0, 10.0, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void myinit()
{
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess = { 100.0 };
	GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 10.0, 10.0, 10.0, 0.0 };

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialf(GL_FRONT, GL_SHININESS, mat_shininess);

	glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(1.0, 0.0, 0.0);

	/* allocate quadrics with filled drawing style */

	h = gluNewQuadric();
	gluQuadricDrawStyle(h, GLU_FILL);
	t = gluNewQuadric();
	gluQuadricDrawStyle(t, GLU_FILL);
	lua = gluNewQuadric();
	gluQuadricDrawStyle(lua, GLU_FILL);
	///////////////////////////////
	lelb = gluNewQuadric();
	gluQuadricDrawStyle(lelb, GLU_FILL);
	lla = gluNewQuadric();
	gluQuadricDrawStyle(lla, GLU_FILL);
	rua = gluNewQuadric();
	gluQuadricDrawStyle(rua, GLU_FILL);
	rla = gluNewQuadric();
	gluQuadricDrawStyle(rla, GLU_FILL);
	lul = gluNewQuadric();
	gluQuadricDrawStyle(lul, GLU_FILL);
	lll = gluNewQuadric();
	gluQuadricDrawStyle(lll, GLU_FILL);
	rul = gluNewQuadric();
	gluQuadricDrawStyle(rul, GLU_FILL);
	rll = gluNewQuadric();
	gluQuadricDrawStyle(rll, GLU_FILL);
	rknee = gluNewQuadric();
	gluQuadricDrawStyle(rknee, GLU_FILL);
	lknee = gluNewQuadric();
	gluQuadricDrawStyle(lknee, GLU_FILL);
	relb = gluNewQuadric();
	gluQuadricDrawStyle(relb, GLU_FILL);
	nk = gluNewQuadric();
	gluQuadricDrawStyle(nk, GLU_FILL);
	rhand = gluNewQuadric();
	gluQuadricDrawStyle(rhand, GLU_FILL);
	lhand = gluNewQuadric();
	gluQuadricDrawStyle(lhand, GLU_FILL);
	lfoot = gluNewQuadric();
	gluQuadricDrawStyle(lfoot, GLU_FILL);
	rfoot = gluNewQuadric();
	gluQuadricDrawStyle(rfoot, GLU_FILL);
	rsh = gluNewQuadric();
	gluQuadricDrawStyle(rsh, GLU_FILL);
	lsh = gluNewQuadric();
	gluQuadricDrawStyle(lsh, GLU_FILL);

	/* Set up tree */

	glLoadIdentity();
	glRotatef(theta[0], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, torso_node.m);
	torso_node.f = torso;
	torso_node.sibling = NULL;
	torso_node.child = &nk_node;

	glLoadIdentity();
	glTranslatef(0.0, TORSO_HEIGHT - 0.25 * NECK_HEIGHT, 0.0);
	glRotatef(theta[1], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, nk_node.m);
	nk_node.f = neck;
	nk_node.sibling = &lsh_node;
	nk_node.child = &head_node;

	glLoadIdentity();
	glTranslatef(-(TORSO_RADIUS + UPPER_ARM_RADIUS), 0.9 * TORSO_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lsh_node.m);
	lsh_node.f = leftShoulder;
	lsh_node.sibling = &rsh_node;
	lsh_node.child = &lua_node;

	glLoadIdentity();
	glTranslatef(TORSO_RADIUS + UPPER_ARM_RADIUS, 0.9 * TORSO_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rsh_node.m);
	rsh_node.f = rightShoulder;
	rsh_node.sibling = &lul_node;
	rsh_node.child = &rua_node;

	glLoadIdentity();
	glTranslatef(0.0, 0.75 * NECK_HEIGHT, 0.0);
	glRotatef(theta[2], 0.0, 1.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, head_node.m);
	head_node.f = head;
	head_node.sibling = NULL;
	head_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0, 0, 0.0);
	glRotatef(theta[3], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lua_node.m);
	lua_node.f = left_upper_arm;
	lua_node.sibling = NULL;
	lua_node.child = &lelb_node;

	glLoadIdentity();
	glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
	glRotatef(theta[4], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lelb_node.m);
	lelb_node.f = leftElbow;
	lelb_node.sibling = NULL;
	lelb_node.child = &lla_node;

	glLoadIdentity();
	glTranslatef(0, 0, 0.0);
	glRotatef(theta[5], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rua_node.m);
	rua_node.f = right_upper_arm;
	rua_node.sibling = NULL;
	rua_node.child = &relb_node;

	glLoadIdentity();
	glTranslatef(0.0, UPPER_ARM_HEIGHT, 0.0);
	glRotatef(theta[4], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, relb_node.m);
	relb_node.f = rightElbow;
	relb_node.sibling = NULL;
	relb_node.child = &rla_node;

	glLoadIdentity();
	glTranslatef(-TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
	glRotatef(theta[7], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lul_node.m);
	lul_node.f = left_upper_leg;
	lul_node.sibling = &rul_node;
	lul_node.child = &lknee_node;

	glLoadIdentity();
	glTranslatef(TORSO_RADIUS / 2, 0.1 * UPPER_LEG_HEIGHT, 0.0);
	glRotatef(theta[9], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rul_node.m);
	rul_node.f = right_upper_leg;
	rul_node.sibling = NULL;
	rul_node.child = &rknee_node;

	glLoadIdentity();
	glTranslatef(0.0, ELBOW_RADIUS / 2, 0.0);
	glRotatef(theta[4], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lla_node.m);
	lla_node.f = left_lower_arm;
	lla_node.sibling = NULL;
	lla_node.child = &lhand_node;

	glLoadIdentity();
	glTranslatef(0.0, ELBOW_RADIUS / 2, 0.0);
	glRotatef(theta[4], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rla_node.m);
	rla_node.f = right_lower_arm;
	rla_node.sibling = NULL;
	rla_node.child = &rhand_node;

	glLoadIdentity();
	glTranslatef(0.0, LOWER_ARM_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rhand_node.m);
	rhand_node.f = rightHand;
	rhand_node.sibling = NULL;
	rhand_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0.0, LOWER_ARM_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lhand_node.m);
	lhand_node.f = leftHand;
	lhand_node.sibling = NULL;
	lhand_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(theta[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lknee_node.m);
	lknee_node.f = leftKnee;
	lknee_node.sibling = NULL;
	lknee_node.child = &lll_node;

	glLoadIdentity();
	glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
	glRotatef(theta[8], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lll_node.m);
	lll_node.f = left_lower_leg;
	lll_node.sibling = NULL;
	lll_node.child = &lfoot_node;

	glLoadIdentity();
	glTranslatef(0.0, LOWER_LEG_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, lfoot_node.m);
	lfoot_node.f = leftFoot;
	lfoot_node.sibling = NULL;
	lfoot_node.child = NULL;

	glLoadIdentity();
	glTranslatef(0.0, UPPER_LEG_HEIGHT, 0.0);
	glRotatef(theta[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rknee_node.m);
	rknee_node.f = rightKnee;
	rknee_node.sibling = NULL;
	rknee_node.child = &rll_node;

	glLoadIdentity();
	glTranslatef(0.0, KNEE_RADIUS / 2, 0.0);
	glRotatef(theta[10], 1.0, 0.0, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rll_node.m);
	rll_node.f = right_lower_leg;
	rll_node.sibling = NULL;
	rll_node.child = &rfoot_node;

	glLoadIdentity();
	glTranslatef(0.0, LOWER_LEG_HEIGHT, 0.0);
	glGetFloatv(GL_MODELVIEW_MATRIX, rfoot_node.m);
	rfoot_node.f = rightFoot;
	rfoot_node.sibling = NULL;
	rfoot_node.child = NULL;

	glLoadIdentity();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(win_width, win_height);
	glutCreateWindow("Taichi Robot");
	myinit();
	glutReshapeFunc(myReshape);
	glutIdleFunc(idle);
	glutDisplayFunc(display);

	glutCreateMenu(menu);
	glutAddMenuEntry(" Raise Hands ", 0);
	glutAddMenuEntry(" Raise Leg ", 1);
	glutAddMenuEntry(" Reset ", 2);
	glutAddMenuEntry(" Left View ", 3);
	glutAddMenuEntry(" Front View ", 4);
	glutAddMenuEntry(" 3/4 View ", 5);
	glutAddMenuEntry(" Gangnam Style Dance ", 6);
	glutAddMenuEntry(" Dance ", 7);
	glutAddMenuEntry(" ���� ", 8);
	glutAddMenuEntry(" ���� ", 9);
	glutAddMenuEntry(" Ұ����� ", 10);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	glutMainLoop();
	return 0;
        }