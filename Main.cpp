#include <GL/freeglut.h>
#include "Data.h"
#include "Vector3.h"
#include <stdio.h>

Vector3 mEye;
Vector3 mCen;
Vector3 mUp;

GLint mFloorLineID;

enum class MouseState {
	LEFT_DOWN,
	RIGHT_DOWN,
	IDLE
};
enum class EditMode {
	CAMERA_TRANSFORM,
	OBJECT_SCALE,
	OBJECT_ROTATE,
	OBJECT_TRANSLATE
};

MouseState mMouse = MouseState::IDLE;
EditMode mEditMode = EditMode::CAMERA_TRANSFORM;
int mMousePosX;
int mMousePosY;

Vector3 mTranslation;
Vector3 mRotation;
Vector3 mScale;

void glsDraw(){

}

void specialFunc(int key, int x, int y) {
	const GLfloat speed = (GLfloat)0.01 * (mEye - mCen).magnitude() * (GLfloat)(glutGetModifiers() == GLUT_ACTIVE_SHIFT ? 2 : 1);
	switch (key) {
	case GLUT_KEY_LEFT:
		mEye = mEye - (mCen - mEye).crossProduct(mUp).normalize() * speed;
		mCen = mCen - (mCen - mEye).crossProduct(mUp).normalize() * speed;
		break;
	case GLUT_KEY_RIGHT:
		mEye = mEye + (mCen - mEye).crossProduct(mUp).normalize() * speed;
		mCen = mCen + (mCen - mEye).crossProduct(mUp).normalize() * speed;
		break;
	case GLUT_KEY_UP:
		mEye = mEye + (mCen - mEye).normalize() * speed;
		mCen = mCen + (mCen - mEye).normalize() * speed;
		break;
	case GLUT_KEY_DOWN:
		mEye = mEye - (mCen - mEye).normalize() * speed;
		mCen = mCen - (mCen - mEye).normalize() * speed;
		break;
	}
	glutPostRedisplay();
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key) {
	case 'q':
		mEditMode = EditMode::CAMERA_TRANSFORM;
		break;
	case 'w':
		mEditMode = EditMode::OBJECT_TRANSLATE;
		break;
	case 'e':
		mEditMode = EditMode::OBJECT_ROTATE;
		break;
	case 'r':
		mEditMode = EditMode::OBJECT_SCALE;
		break;
	}
}

void motionFunc(int x, int y) {
	const GLfloat speed = (GLfloat)0.001 * (mEye - mCen).magnitude() * (GLfloat)(glutGetModifiers() == GLUT_ACTIVE_SHIFT ? 2 : 1);
	switch (mEditMode) {
		case EditMode::CAMERA_TRANSFORM :
			switch (mMouse) {
			case MouseState::LEFT_DOWN:
				mEye = mEye + (mCen - mEye).crossProduct(mUp).normalize() * (GLfloat)(mMousePosX - x) * speed + mUp.normalize() * (GLfloat)(y - mMousePosY) * speed;
				mCen = mCen + (mCen - mEye).crossProduct(mUp).normalize() * (GLfloat)(mMousePosX - x) * speed + mUp.normalize() * (GLfloat)(y - mMousePosY) * speed;
				break;
			case MouseState::RIGHT_DOWN:
				mCen = mCen + (mCen - mEye).crossProduct(mUp).normalize() * (GLfloat)(x - mMousePosX) * speed + mUp.normalize() * (GLfloat)(mMousePosY - y) * speed;
				break;
			}
			break;
		case EditMode::OBJECT_TRANSLATE:
			break;
		case EditMode::OBJECT_ROTATE:
			break;
		case EditMode::OBJECT_SCALE:
			break;
	}
	mMousePosX = x;
	mMousePosY = y;

	glutPostRedisplay();
}

void mouseFunc(int button, int state, int x, int y) {
	mMousePosX = x;
	mMousePosY = y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		mMouse = MouseState::LEFT_DOWN;
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
		mMouse = MouseState::RIGHT_DOWN;
	else if (state == GLUT_UP)
		mMouse = MouseState::IDLE;
}

void mouseWheelFunc(int wheel, int direction, int x, int y) {
	if (direction == 0)
		return;
	Vector3 vec = (mEye - mCen);
	float mag = vec.magnitude();
	if (direction > 0)			// Zoom in
		mEye = mCen + vec.normalize() * mag * (GLfloat)0.95;
	else if (direction < 0) 	// Zoom out
		mEye = mCen + vec.normalize() * mag * (GLfloat)1.05;

	glutPostRedisplay();
}

void reshapeFunc(int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, (GLdouble)w / h, _CAMERA_INIT_NEAR_, _CAMERA_INIT_FAR_);
	glViewport(0, 0, w, h);
}

void drawCoord() {
	const float size = 20;

	glColor3f(0,0.6f,0);
	glutWireSphere(0.1, 10, 10);
	glColor3f(1, 0, 0);
	glPushMatrix();
		glTranslatef(0.05 * size, 0, 0);
		glScalef(size, 1, 1);
		glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(0, 1, 0);
	glPushMatrix();
		glTranslatef(0, 0.05 * size, 0);
		glScalef(1, size, 1);
		glutSolidCube(0.1);
	glPopMatrix();

	glColor3f(0, 0, 1);
	glPushMatrix();
		glTranslatef(0, 0, 0.05*size);
		glScalef(1, 1, size);
		glutSolidCube(0.1);
	glPopMatrix();
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(mEye.x,mEye.y,mEye.z, mCen.x, mCen.y, mCen.z, mUp.x, mUp.y, mUp.z);
	drawCoord();
	glCallList(mFloorLineID);

	glTranslatef(mTranslation.x,mTranslation.y,mTranslation.z);
	glRotatef(mRotation.z, 0, 0, 1);
	glRotatef(mRotation.y, 0, 1, 0);
	glRotatef(mRotation.x, 1, 0, 0);
	glScalef(mScale.x, mScale.y, mScale.z);

	glsDraw();

	glutSwapBuffers();
}


void glsFloorLine() {
	mFloorLineID = glGenLists(1);
	const int _size = 100;
	glNewList(mFloorLineID, GL_COMPILE);
	glColor3b(0, 0, 0);
	glBegin(GL_LINES);
	for (int i = -_size; i <= _size; i++) {
		glVertex3f((GLfloat)i, 0, (GLfloat)-_size);
		glVertex3f((GLfloat)i, 0, (GLfloat)_size);
		glVertex3f((GLfloat)-_size, 0, (GLfloat)i);
		glVertex3f((GLfloat)_size, 0, (GLfloat)i);
	}
	glEnd();
	glEndList();
}


void glsInitLook() {
	mEye = Vector3(_LOOK_INIT_EYE_X_, _LOOK_INIT_EYE_Y_, _LOOK_INIT_EYE_Z_);
	mCen = Vector3(_LOOK_INIT_CEN_X_, _LOOK_INIT_CEN_Y_, _LOOK_INIT_CEN_Z_);
	mUp = Vector3(_LOOK_INIT_UP_X_, _LOOK_INIT_UP_Y_, _LOOK_INIT_UP_Z_);
}

void glsInitTransform() {
	mTranslation = Vector3(0, 0, 0);
	mRotation = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
}

void glsSetting() {
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(_WINDOW_INIT_WIDTH_, _WINDOW_INIT_HEIGHT_);
	glutInitWindowPosition(_WINDOW_INIT_POS_X_, _WINDOW_INIT_POS_Y_);
	glutCreateWindow("Object Editor");
	glutDisplayFunc(displayFunc);
	glutReshapeFunc(reshapeFunc);
	glutMouseFunc(mouseFunc);
	glutMotionFunc(motionFunc);
	glutMouseWheelFunc(mouseWheelFunc);
	glutKeyboardFunc(keyboardFunc);
	glutSpecialFunc(specialFunc);
	
	glEnable(GL_DEPTH_TEST);		// 깊이
	glEnable(GL_CULL_FACE);			// 후면 제거
	glCullFace(GL_BACK);			// 호출 필수
	glFrontFace(GL_CCW);			// GL_CCW가 반시계로 기본값
	glEnable(GL_BLEND);				// 안티 애일리싱 블랜더 필수
	glEnable(GL_POINT_SMOOTH);		// 안티 애일리싱 필수
	glEnable(GL_LINE_SMOOTH);		// 안티 애일리싱 필수
	glEnable(GL_POLYGON_SMOOTH);	// 안티 애일리싱 필수
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LIGHTING); // 조명

	glClearColor(1, 1, 1, 1);

	glsInitLook();
	glsFloorLine();
}
 

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glsSetting();

	glutMainLoop();
	return 0;
}