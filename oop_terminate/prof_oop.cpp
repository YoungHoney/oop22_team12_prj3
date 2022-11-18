#define GL_SILENCE_DEPRECATION

#include "prof_oop.h"



CSphere control_sphere, hit_sphere; // �����ϴ� �� ��, �ݻ�Ǵ� ������
CSphere target_sphere[1200]; // ����� �ϴ� Ÿ�� �Ķ� ���� ���� �ִ� NO_SPHERE ������ŭ ����, NO_SPHERE������ �������� 1200���� ����
CWall g_wall(planeWidth, planeHeight, planeDepth); // �ٴ� ���
CWall boundary_wall[4]; // �����ڸ� ��


// ------------------------------------------------------- v implement of CSphere v -------------------------------------------------------

CSphere::CSphere()
{
	center_x = center_y = center_z = 0.0;
}

void CSphere::init()
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix(); // ���� �𵨺� ����� ����, �׸��� �� ������ �׷����� �͵鿡 ���� ���� �𵨺� ����� ������ �ٽ� �������Ѱ��鼭 ǥ��
	glLoadIdentity(); // ����� ������ķ� �ʱ�ȭ�Ѵ�.
	glGetDoublev(GL_MODELVIEW_MATRIX, m_mRotate); // ��Ʈ���� ����, ���� ���� ��Ʈ����
	glPopMatrix(); // �׵��� �����ߴ� ����� ���ְ� push�� ����� �ٽ� �ҷ��´�.
}

void CSphere::setCenter(float x, float y, float z)
{
	center_x = x;    center_y = y;    center_z = z;
}

void CSphere::setColor(float r, float g, float b)
{
	color_r = r; color_g = g; color_b = b;
}

// �������� ���� �ٸ� ����� hasIntersected, ���� ���� �ʰ� �Ϸ��� �����ؾ���
bool  CSphere::hasIntersected(const CSphere& ball)
{
	float deltaX = center_x - ball.center_x;
	float deltaY = center_y - ball.center_y;
	if (sqrt(deltaX * deltaX + deltaY * deltaY) <= 2 * radius_sphere)
		return true;
	else
		return false;
}


void  CSphere::hitBy(const CSphere hitSphere)
{
	// ���� ������ �浹�� ���� ���� �ذ� �κ�
	// ������ �ε����� ������ �ٲ���µ� ����������, ���� �ӵ� �ݴ� �������� x ���а� y ������ ��ġ�� ������ �ذ�� ������ 0.01�� �ٲ۴�.
	while (hasIntersected(hitSphere)) {
		center_x -= dir_x / sqrt(dir_x * dir_x + dir_y * dir_y) * 0.01;
		center_y -= dir_y / sqrt(dir_x * dir_x + dir_y * dir_y) * 0.01;
	}
	float deltaX = hitSphere.center_x - center_x;
	float deltaY = hitSphere.center_y - center_y;
	float distance = sqrt(deltaX * deltaX + deltaY * deltaY);

	// Rodrigues' rotation formula ���
	// V(rot) = -v + 2(k inner_product v)k
	float k_x = deltaX / distance;
	float k_y = deltaY / distance;
	float v_x = -dir_x;
	float v_y = -dir_y;
	float inner_product = k_x * v_x + k_y * v_y;

	dir_x = -v_x + 2 * inner_product * k_x;
	dir_y = -v_y + 2 * inner_product * k_y;


}
void CSphere::draw()
{
	glLoadIdentity(); //������ķ� �ʱ�ȭ
	glTranslatef(0.0, 0.0, -sdepth); //+Z������ ȭ�鿡�� �츮�� ���¹����̹Ƿ� -���ϸ� ��ҵǴ� ȿ��, wall�� ������ ���߱����� ����ѵ�
	glMultMatrixd(m_mRotate); // ���콺 �̵������� motion�ݹ��Լ����� m_mrotate����� �����Ǵµ� �� mult�Լ��� ������Ŀ� ���Ͽ� m_mrotate��� ȸ���Ѵ�.
	glTranslated(center_x, center_y, center_z); // �߾����� �̵��Ѵ�.
	glColor3f(color_r, color_g, color_b); //�� ����
	glutSolidSphere(radius_sphere, 20, 16); //radius_sphere�� ������ ������ �� ���ڴ� ���� ��Ÿ���� �漱�� ����
}



// ------------------------------------------------------- v implement of CWall v -------------------------------------------------------



CWall::CWall(float w = 0.0, float h = 0.0, float d = 0.0)
{
	width = w; height = h; depth = d;
	color_r = 0.0; color_g = 1.0; color_b = 0.0;


	float coef{};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == 0) coef = w / 2.0;
			if (j == 1) coef = h / 2.0;
			if (j == 2) coef = d / 2.0;
			Verts[i][j] = coef * BoxVerts[i][j]; // BoxVerts�� ������ ��������, coef�� ������ �������Ϳ� �������� ũ��
		}
	}
}

void CWall::init() //initGL���� initrotate�� ȣ���ϰ� �ű⼭ ȣ���
{ // openGL���� ����� mode�� ������ ��, transelate, rotate���� ��ȯ�� ��ġ�µ�, �Ϲ������� ��ȯ�� �� �� LoadIdentity�� �ʱ�ȭ�� ����� �Ѵ�. �ȱ׷��� �����Ǵϱ�.. �ٵ� �� ��� ����
   //������ �̿��ؼ� ���� ��ġ�� push�� ������ �� ������ �� �Ŀ� �׳� pop���� �������� �ִ�.


	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glGetDoublev(GL_MODELVIEW_MATRIX, m_mRotate); //��Ʈ���� ����, ���� ���� ��Ʈ����, m_mRotate�� ��������� ��
	glPopMatrix();
}

void CWall::setSize(float w, float h, float d)
{
	width = w;
	height = h;
	depth = d;


	float coef{};
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 3; j++) {
			if (j == 0) coef = w / 2.0;
			if (j == 1) coef = h / 2.0;
			if (j == 2) coef = d / 2.0;
			Verts[i][j] = coef * BoxVerts[i][j]; // BoxVerts�� ������ ��������, coef�� ������ �������Ϳ� �������� ũ��
		}
	}
}

void CWall::setCenter(float x, float y, float z)
{
	center_x = x;    center_y = y;    center_z = z;
}

void CWall::setColor(float r, float g, float b)
{
	color_r = r; color_g = g; color_b = b;
}

void CWall::draw() //display �ݹ��Լ����� ȣ���
{
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -sdepth); //  �� ����� ������ �𵨺���Ŀ� �������� �����̵��� �Ѵ�.
	//�𵨺���Ŀ� 1 0 0 0.0
	//            0 1 0 0.0
	//            0 0 1 -sdepth
//                              0 0 0   1  �� �������� �ȴ�.
//ȭ�鿡�� �츮���� z���Ͱ� +�� �Ǵ����̹Ƿ� sdepth�� Ŀ������ wall�� �۾����� ȿ���� ���� �� �ִ�.
	glMultMatrixd(m_mRotate); //���� ��Ŀ� rotate��������Ѵ�. m_mRotate�� motion�ݹ��Լ����� ���콺 �����ӿ����� �����ȴ�.
	glTranslatef(center_x, center_y, center_z); //wall�� center_x,y,z��ŭ�� ��ġ�� �����̵�

	glColor3f(color_r, color_g, color_b); //�� ����

	int i;
	int v1, v2, v3, v4;

	for (i = 0; i < 6; i++) {
		v1 = cubeIndices[i][0];
		v2 = cubeIndices[i][1];
		v3 = cubeIndices[i][2];
		v4 = cubeIndices[i][3];

		glBegin(GL_QUADS); //glbegin�� ������ �׸��� �����ߴٴ� ��
		glNormal3f(bNorms[i][0], bNorms[i][1], bNorms[i][2]);
		glVertex3f(Verts[v1][0], Verts[v1][1], Verts[v1][2]);
		glNormal3f(bNorms[i][0], bNorms[i][1], bNorms[i][2]);
		glVertex3f(Verts[v2][0], Verts[v2][1], Verts[v2][2]);
		glNormal3f(bNorms[i][0], bNorms[i][1], bNorms[i][2]);
		glVertex3f(Verts[v3][0], Verts[v3][1], Verts[v3][2]);
		glNormal3f(bNorms[i][0], bNorms[i][1], bNorms[i][2]);
		glVertex3f(Verts[v4][0], Verts[v4][1], Verts[v4][2]);
		glEnd(); //glbegin�� ���������� �����׸��� ��
	}
}

// �� ���� ���� �浹 ����
bool CWall::hasUpIntersected(CSphere* sphere)
{
	if (sphere->center_y + radius_sphere >= planeHeight / 2)
		return (true);
	return (false);
}

// �Ʒ� ���� ���� �浹 ����
bool CWall::hasDownIntersected(CSphere* sphere, CSphere* control_sphere) {
	if (sphere->center_y - radius_sphere <= -1 * planeHeight / 2)
	{
		Life -= 1; //�Ʒ� ���� ������ Life�� ����
		statecode = LIFE_DECREASE;
		sphere->center_x = hit_sphere_init_x;
		sphere->center_y = hit_sphere_init_y;
		sphere->center_z = 0.0;

		sphere->dir_x = 0.0;
		sphere->dir_y = 0.0;
		sphere->dir_z = 0.0;

		control_sphere->setCenter(control_sphere_init_x, control_sphere_init_y, 0.0); //�Ͼ��
		return (true);
	}
	return (false);
}

// ���� ���� ���� �浹 ����
bool CWall::hasLeftIntersected(CSphere* sphere) {
	if (sphere->center_x - radius_sphere <= -1 * planeWidth / 2)
		return (true);
	return (false);
}

// ������ ���� ���� �浹 ����
bool CWall::hasRightIntersected(CSphere* sphere)
{
	if (sphere->center_x + radius_sphere >= planeWidth / 2)
		return (true);
	return (false);
}


void CWall::hitBy(CSphere* sphere, CSphere* control_sphere)
{
	if (hasUpIntersected(sphere))
	{
		sphere->dir_y = -(sphere->dir_y);

		// ���� ������ �浹�� ���� ���� �ذ� �κ�
		// ���� ���� �ε����� ���� ������ �ٲ���µ� ����������, �ݻ� �������� x ���а� y ������ ��ġ�� ������ �ذ�� ������ 0.1�� �ٲ۴�.
		while (hasUpIntersected(sphere)) {
			sphere->center_y -= 0.1;
		}
	}
	else if (hasDownIntersected(sphere, control_sphere)) {
		sphere->dir_y = -(sphere->dir_y);

		// ���� ������ �浹�� ���� ���� �ذ� �κ�
		// ���� ���� �ε����� ���� ������ �ٲ���µ� ����������, �ݻ� �������� x ���а� y ������ ��ġ�� ������ �ذ�� ������ 0.1�� �ٲ۴�.
		while (hasDownIntersected(sphere, control_sphere)) {
			sphere->center_y += 0.1;

		}


	}
	else if (hasLeftIntersected(sphere))
	{
		sphere->dir_x = -(sphere->dir_x);

		// ���� ������ �浹�� ���� ���� �ذ� �κ�
		// ���� ���� �ε����� ���� ������ �ٲ���µ� ����������, �ݻ� �������� x ���а� y ������ ��ġ�� ������ �ذ�� ������ 0.1�� �ٲ۴�.
		while (hasLeftIntersected(sphere)) {
			sphere->center_x += 0.1;
		}
	}
	else if (hasRightIntersected(sphere))
	{
		sphere->dir_x = -(sphere->dir_x);

		// ���� ������ �浹�� ���� ���� �ذ� �κ�
		// ���� ���� �ε����� ���� ������ �ٲ���µ� ����������, �ݻ� �������� x ���а� y ������ ��ġ�� ������ �ذ�� ������ 0.1�� �ٲ۴�.
		while (hasRightIntersected(sphere)) {
			sphere->center_x -= 0.1;
		}
	}
}




// ------------------------------------------------------- v implement of CallbackFunc v -------------------------------------------------------

void InitObjects()
{
	// specify initial colors and center positions of each spheres
	hit_sphere.setColor(0.8, 0.2, 0.2); hit_sphere.setCenter(hit_sphere_init_x, hit_sphere_init_y, 0.0); //������
	control_sphere.setColor(0.8, 0.8, 0.8); control_sphere.setCenter(control_sphere_init_x, control_sphere_init_y, 0.0); //�Ͼ��

	// �Ķ��� target_sphere �� ��ġ
	cnt_placed_sphere = 0;
	for (int i = 0; i < planeHeight; i++) {
		for (int j = 0; j < planeWidth; j++) {
			if (sphere_place[i][j] != '.') {
				target_sphere[cnt_placed_sphere].dir_x = 1;
				target_sphere[cnt_placed_sphere].setColor(0.2, 0.2, 0.8);
				target_sphere[cnt_placed_sphere++].setCenter(-planeWidth / 2 + j + radius_sphere, planeHeight / 2 - i - radius_sphere, 0);
			}
		}
	}

	// specify initial colors and center positions of a wall
	g_wall.setColor(0.0, 1.0, 0.0); g_wall.setCenter(0.0, 0.0, -0.6);

	boundary_wall[0].setSize(planeWidth, 0.1, 3);
	boundary_wall[0].setColor(0.5882, 0.2941, 0.0);
	boundary_wall[1].setSize(planeWidth, 0.1, 3);
	boundary_wall[1].setColor(0.5882, 0.2941, 0.0);
	boundary_wall[2].setSize(0.1, planeHeight, 3);
	boundary_wall[2].setColor(0.5882, 0.2941, 0.0);
	boundary_wall[3].setSize(0.1, planeHeight, 3);
	boundary_wall[3].setColor(0.5882, 0.2941, 0.0);

	boundary_wall[0].setCenter(0.0, planeHeight / 2, 0.0); // ���� �����ڸ� ��
	boundary_wall[1].setCenter(0.0, -(planeHeight / 2), 0.0); // �Ʒ��� �����ڸ� ��
	boundary_wall[2].setCenter(planeWidth / 2, 0.0, 0.0); // ������ �����ڸ� ��
	boundary_wall[3].setCenter(-(planeWidth / 2), 0.0, 0.0); // ���� �����ڸ� ��

}



void ReshapeCallback(int width, int height)
{

	xsize = width;
	ysize = height;
	aspect = (float)xsize / (float)ysize;

	glViewport(0, 0, xsize, ysize); //ó�� �����Ǵ� ������� InitGL�� glutInitWindowsize���� ���������� ����Ǵ� ���α׷� ��ü�� ���� �� �Լ��� �ػ󵵿� ���� �����ȴ�. ���Ҹ��� ������
	// ���� 1920,1080���� ������ ���� �˱⽬���
	glMatrixMode(GL_PROJECTION); //openGL�� �𵨸��� GL_MODELVIEW, GL_PROJECTION, GL_TEXTURE 3���� ��ķ� ȭ�鿡ǥ���Ѵ�. ���� (1,1,1)�� �� �׸��ٸ� �ش� ��ǥ�� GL_MODELVIEW ��Ʈ������
	// ���ؼ� �������� ��ġ�� �����Ѵ�. ���� �������� ��ġ������ �ϳ����� GL_MODELVIEW ��Ʈ������ ����ȴٸ� ���� �ٸ� ��ġ�� �׷�����. �̷��� �׷��� ����
	   //GL_PROJECTION ��İ� ������ ȭ��� �����ȴ�. �� GL_MODELVIEW�� �𵨸��� ���� ���� ��ġ��� GL_PROJECTION�� �ش� �𵨸��� ���������� ��� ǥ������ �����Ѵ�.

 /*
 GL_MODELVIEW�� ����: Ư����ǥ�� ������ �׸��ٸ� GL_MODELVIEW�� ���� �������� ��ġ�� ����, ���� �̰� ����ȴٸ� �����ٸ� ��ǥ�� �׷���
 glTranslatef, glScalef, glRotatef���� ���� ��ü�� ��ġ�� ��ȯ �Ǵ� ������ �̸� �𵨸� ��ȯ �̶�� ��,�𵨺�����̶� ��ǥ�� ��ȯ�� ���� ��ü�� ��ġ�� ������ �����ϱ� ����
 4x4��İ� ���ؽ����� �ϳ��� ���� ������ ��ķ� ��������� �𵨺���İ� ������

 GL_PROJECTION�� ����: GL_MODELVIEW�� �׷��� ������ ��ġ��� ������ GL_PROJECTION�� ��İ� ������ ���������� ��� ȭ�鿡 �Ѹ����ΰ��� ����Ѵ�.
 �������� glOrtho �������� gluPerspectivef


 */


	glLoadIdentity(); // ������ķ� �ʱ�ȭ�Ѵ�.
	gluPerspective(64.0, aspect, zNear, zFar); //��������

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity(); // ������ķ� �ʱ�ȭ�Ѵ�.   ������ �����ߴ� ȸ������ �����ϰ� �ٽ� �����ϱ����� ������ �ؾ��Ѵ�. �� �ڵ尡 ������ ȸ���� ��� ��������ȴ�.
	//�̿Ͱ��� glMatrixMode(GL_MODELVIEW)���Ŀ� glLoadIdentity()�� �����°� �̷��� �����̴�.

	glutPostRedisplay();
}

void renderBitmapCharacter(float x, float y, float z, void* font, char* string)
{
	char* c;
	glRasterPos3f(x, y, z);
	for (c = string; *c != '\0'; c++)
	{
		glutBitmapCharacter(font, *c);
	}
}

void DisplayCallback(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //glclear�� ���۵��� �̸������� ������ �ٲ۴�.
	//GL_COLOR_BUFFER_BIT�� initgl�� glclearcolor���� ������ ���̴�.
	//���� 3������ 2������ �׸������� depth��� ������ �ִ�. ���� x,y�� depth���̿� ���� �׷����� ���� ��������.
	//�� �� ��ü�� �ϳ��� ���뱸 ����ϰ� ���̴µ�, 3�����׸� ��ü�� ����� ������ �Ѵ�.
	glMatrixMode(GL_MODELVIEW);



	for (i = 0; i < cnt_placed_sphere; i++) {

		target_sphere[i].draw(); //�� �׸���

	}

	control_sphere.draw(); // �Ͼ� �� ���� �׸���
	hit_sphere.draw(); // ���� �� �׸���
	g_wall.draw(); // �� �׸���
	for (int i = 0; i < 4; i++) if (i != 1) boundary_wall[i].draw(); // �Ʒ� �� �����ϰ� boundary_wall �׸���

	renderBitmapCharacter(-10, 3.8, -5, GLUT_BITMAP_TIMES_ROMAN_24, (char*)((("Score : ") + std::to_string(Score)).c_str()));
	renderBitmapCharacter(45, 3.8, -5, GLUT_BITMAP_TIMES_ROMAN_24, (char*)((("Life : ") + std::to_string(Life)).c_str()));
	if (statecode == GAME_START || statecode == LIFE_DECREASE) {
		renderBitmapCharacter(17, -8, 10, GLUT_BITMAP_TIMES_ROMAN_24, (char*)"Space To Start"); //���� ������ space ������ �����  �����ϴ°Ŷ� ������ �ִµ� �� ��� ���ָ� ��������?
	}

	if (Life == 4) { //Life�� 0�̵Ǹ� gameover
		renderBitmapCharacter(17, -8, 5, GLUT_BITMAP_TIMES_ROMAN_24, (char*)"YOU FAILED 'esc' to get out");
		statecode = GAME_OVER;
	}

	if (Score >= cnt_placed_sphere - 120) { //Score�� cnt_placed_sphere==127 ���� ��� �������� ��, �׽�Ʈ�Ҷ��� 20�� ���µ� ū ���� �����ҵ�
		renderBitmapCharacter(17, -8, 5, GLUT_BITMAP_TIMES_ROMAN_24, (char*)"YOU WIN 'r' to regame");

		statecode = GAME_CLEAR;
	}

	glutSwapBuffers(); // front���ۿ� back���۸� swapping �ϱ� ���Ѱ�, ����Ʈ���۳����� ȭ�鿡 �ѷ����� ���� ���ο� ������ ����ۿ� ���̰� ����ۿ� ����� �� �Ǹ� ����Ʈ�� ���� �ٲ��.
	//����ۿ� �׸��� �� �׷����� ������ۿ� ��°�� ��ü�Ѵ�. ����� �ĸ��� �Ͻÿ� ��ü�ǹǷ� ����ۿ� �̸� �غ��ص� �׸��� ��Ÿ����.
 //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); ���� ������µ�?
}



void KeyboardCallback(unsigned char ch, int x, int y)
{
	switch (ch)
	{

	case 'r': {
		if (statecode == GAME_CLEAR) {
			statecode = GAME_START;
			Life = 5;
			Score = 0;
			InitObjects();

		}
		break;
	}


	case 32: {//�����̽���

		switch (statecode) {
		case GAME_START: {

			hit_sphere.dir_x = 0.0; //sphere[0]�� �����̽��� ������ �����̴� ���� ��
			hit_sphere.dir_y = 3.0;
			hit_sphere.dir_z = 0.0;
			statecode = GAME_PLAYING;
			break;
		}
		case LIFE_DECREASE: {
			hit_sphere.dir_x = 0.0; //sphere[0]�� �����̽��� ������ �����̴� ���� ��
			hit_sphere.dir_y = 3.0;
			hit_sphere.dir_z = 0.0;
			statecode = GAME_PLAYING;
			break;
		}
		default: {
			//doing nothing
		}


		}
		break;
	}
	case 27: {//ESCŰ
		exit(0);
		break;
	}


	}

	glutPostRedisplay();
}



void SpecialCallback(int key, int x, int y) {

	if (GLUT_KEY_LEFT == key) {
		leftPressed = true;
	}
	if (GLUT_KEY_RIGHT == key) {
		rightPressed = true;
	}


}

void SpecialUpCallback(int key, int x, int y) {
	if (GLUT_KEY_LEFT == key) {
		leftPressed = false;
	}
	if (GLUT_KEY_RIGHT == key) {
		rightPressed = false;
	}
}



void initRotate() { // ������ ��¦ �ٸ� initGL���� ȣ��

	for (int i = 0; i < NO_SPHERE; i++)
	{
		target_sphere[i].init();
	}
	hit_sphere.init();
	control_sphere.init();

	g_wall.init();
	for (int i = 0; i < 4; i++) boundary_wall[i].init();

}

void InitGL() {
	//opengl�� �⺻ ������
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ���̹��۰� �ְ� rgb������۰� �ִ� ���߹���â�� ��带 ����
	glutInitWindowSize(1200, 800);
	glutCreateWindow("OOP_TEAM12_PRJ3_ARKANOID"); // Displaymode���� ������ ���۴�� â�� �����
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glClearColor(0.7, 0.7, 0.7, 0.0);
	glPolygonOffset(1.0, 1.0);
	glDisable(GL_CULL_FACE);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glLightfv(GL_LIGHT0, GL_POSITION, light0Position);
	glEnable(GL_LIGHT0);
	initRotate(); //���� wall�� initȣ��

	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);



	//���� opengl�� �̺�Ʈ ó���κ�
	//�ݹ��Լ���: �̺�Ʈ ó������ �Ѵ�.w�� ������ ������ ����, esc�� ������ �����°ſ� ���� � ���������� ����� ��Ÿ���� �Լ��ε�, ���÷����ݹ��Լ��� ��� �����Ӹ��� ȣ��ɰ��̴�.

	glutIdleFunc(renderScene); // ���ø����̼��� �޸�(idle)�ð��� ȣ��Ǵ� �Լ�(renderscene)�� ȣ���ϵ����Ѵ�. �� �ִϸ��̼��� �ȴ�.
	glutReshapeFunc(ReshapeCallback); // GLUT�� ó�� �����츦 ����, ������ ��ġ�� �ű涧, ������ ũ�⸦ �����Ҷ� reshapeevent�� �߻��Ѱɷ� ����. �Ű��������� �Լ��� ���ϴ� �Լ�ȿ���� �Ҽ��ִ�.
	// reshapeevent �� �߻��ϸ� ��ȣ���� �Ķ���͸����� ����� �������� ���� ���̸� �ݹ��Լ��� �����Ѵ�. �׷��� width�� height�� �׳� �޾Ƽ� ���µ�.
	glutDisplayFunc(DisplayCallback); // �� �Լ��� DisplayCallback�̶�� �Լ��� ���÷����̺�Ʈ�� ���� �ݹ��Լ��� ����ϴ� �Լ�, �Ű������� ������ �Լ��� ���÷����̺�Ʈ���� ȣ��ȴ�.
	glutKeyboardFunc(KeyboardCallback); // Ű���尡 ��������� �۵��ϴ� �ݹ��Լ�

	glutSpecialFunc(SpecialCallback); //����Ű, F1~F12�� ���� Ư���� Ű�� ���� ��� �۵��ϴ� �ݹ��Լ�
	glutSpecialUpFunc(SpecialUpCallback); //����Ű�� ������ �۵��ϴ� �ݹ��Լ�, �ε巯�� �������� ���� �߰�
}



int currentTime, previousTime = -1; // �ű⿣ ����
void renderScene() // ���� �ٸ�, ���ø����̼��� �޸�ð��� ȣ��Ǵ� �Լ�, glutidleFunc()���� ȣ��ȴ�.
{

	int timeDelta;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	if (previousTime == -1) timeDelta = 0;
	else timeDelta = (currentTime - previousTime) / 2;

	float x = hit_sphere.center_x;
	float y = hit_sphere.center_y;
	float z = hit_sphere.center_z;



	if (leftPressed) {

		if (statecode == GAME_START || statecode == LIFE_DECREASE) //���ӽ��۴ܰ�� �������� ���� �ܰ迡���� �Ͼ���� �������� ���� ������
		{
			hit_sphere.setCenter(hit_sphere.center_x - timeDelta * 0.05, hit_sphere.center_y, 0);
			control_sphere.setCenter(control_sphere.center_x - timeDelta * 0.05, control_sphere.center_y, 0);
		}
		else //�ٸ� �ܰ迡���� �Ͼ���� ������
		{
			control_sphere.setCenter(control_sphere.center_x - timeDelta * 0.05, control_sphere.center_y, 0);
		}

	}

	if (rightPressed) {

		if (statecode == GAME_START || statecode == LIFE_DECREASE) //���ӽ��۴ܰ�� �������� ���� �ܰ迡���� �Ͼ���� �������� ���� ������
		{
			hit_sphere.setCenter(hit_sphere.center_x + timeDelta * 0.05, hit_sphere.center_y, 0);
			control_sphere.setCenter(control_sphere.center_x + timeDelta * 0.05, control_sphere.center_y, 0);
		}
		else //�ٸ� �ܰ迡���� �Ͼ���� ������
		{
			control_sphere.setCenter(control_sphere.center_x + timeDelta * 0.05, control_sphere.center_y, 0);
		}

	}



	if (statecode == GAME_PLAYING) {
		x = hit_sphere.center_x;
		y = hit_sphere.center_y;
		z = hit_sphere.center_z;

		hit_sphere.setCenter(
			x + timeDelta * 0.008 * hit_sphere.dir_x, // �ӵ��� ������ 1�϶�, ���� timeDelta �� 0.002��ŭ �����δ�.
			y + timeDelta * 0.008 * hit_sphere.dir_y,
			z + timeDelta * 0.008 * hit_sphere.dir_z);

		for (int i = 0; i < cnt_placed_sphere; i++) {
			x = target_sphere[i].center_x;
			y = target_sphere[i].center_y;
			z = target_sphere[i].center_z;
			target_sphere[i].setCenter(
				x + timeDelta * 0.008 * target_sphere[i].dir_x,
				y + timeDelta * 0.008 * target_sphere[i].dir_y,
				z + timeDelta * 0.008 * target_sphere[i].dir_z);
		}
	}
	glutPostRedisplay(); // �����츦 �ٽñ׸����� ��û, �ٷ� ���÷����ݹ��Լ�(renderscene)�� ȣ����� �ʰ� ���η���(�Ƹ� glutMainloop?)���� ȣ������� �����Ѵ�. �̰� ������ ������ ���ڿ�������

	// renderScene���� �� ������ �浹�� ó���ϴ� �κ�
	// ���� ��� ������ �˻��ϰ�, ����� ��� �ݻ縦 ����
	if (hit_sphere.hasIntersected(control_sphere) == true) // hit_sphere�� �������� control_sphere�� ������� �浹 ����
	{
		hit_sphere.hitBy(control_sphere);
	}

	for (int i = 0; i < cnt_placed_sphere; i++) {
		if (hit_sphere.hasIntersected(target_sphere[i]) == true) // hit_sphere�� �������� target_sphere[]�� �Ķ�������� �浹 ����
		{
			hit_sphere.hitBy(target_sphere[i]); // �켱�� ������ ��� �浹�� ó�� ���߿� �ٸ� ���鵵 �ֱ�
			target_sphere[i].setCenter(0, 500, 500); //���� ���� �ָ� ���躸��
			target_sphere[i].dir_x = 0;
			Score += 1;
			break;
		}
	}

	// ���� ���� �ݻ� ����, �ٸ� ��� �ڵ� �״�� ������
	g_wall.hitBy(&hit_sphere, &control_sphere);
	g_wall.hitBy(&control_sphere, &control_sphere);

	for (int i = 0; i < cnt_placed_sphere; i++) {
		if (g_wall.hasLeftIntersected(&target_sphere[i]) || g_wall.hasRightIntersected(&target_sphere[i])) {

			while (g_wall.hasLeftIntersected(&target_sphere[i])) {
				for (int j = 0; j < cnt_placed_sphere; j++)
					target_sphere[j].center_x += 0.1;
			}
			while (g_wall.hasRightIntersected(&target_sphere[i])) {
				for (int j = 0; j < cnt_placed_sphere; j++)
					target_sphere[j].center_x -= 0.1;
			}
			for (int i = 0; i < cnt_placed_sphere; i++) {
				target_sphere[i].dir_x = -target_sphere[i].dir_x;
			}
			break;
		}
	}

	previousTime = currentTime;
}

// �߰��ؾ��� using namespace std;
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // glut ���� �ʱ�ȭ, ���� �׷��ȸ����µ��� �Ѵٰų� �ϸ� ���� ������
	InitObjects(); // �ʱ�ȭ��, �� ���� ��ġ�� ����
	InitGL(); // opengl ���� �ʱ�ȭ
	glutMainLoop(); // �̺�Ʈ ������ �����°�, �̺�Ʈ���� �ݹ��Լ� ����� �������� �̺�Ʈ ������ �����϶�.
	return 0;
}


