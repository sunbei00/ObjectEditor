#include <GL/freeglut.h>
#include "Data.h"
#include "Vector3.h"
#include "LoadBMP.h"
#include <stdio.h>



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
enum class CoordSel  {
	XYZ,
	X,
	Y,
	Z
};

MouseState mMouse = MouseState::IDLE;
EditMode mEditMode = EditMode::CAMERA_TRANSFORM;
CoordSel mCoordSel = CoordSel::XYZ;
int mMousePosX;
int mMousePosY;

Vector3 mTranslation;
Vector3 mRotation;
Vector3 mScale;

Vector3 mEye;
Vector3 mCen;
Vector3 mUp;

GLint mFloorLineID;
GLuint mTextureID[100];


bool glsLoadGLTextures(char* path, GLuint* TextureObject);
void glsInitTransform();

void glsInitTexture() {
	char* path = (char*)".\\test.bmp";
	glsLoadGLTextures(path, &mTextureID[0]);

}

void glsDrawNotEdit() {
	glutSolidCube(1);

}

void glsDrawEdit(){
	GLfloat light[] = {1,1,1,0};
	glLightfv(GL_LIGHT0, GL_AMBIENT_AND_DIFFUSE, light);
	glEnable(GL_LIGHT0);

	glColor4f(0.7, 0.3, 0.5, 0);
	glBindTexture(GL_TEXTURE_2D, mTextureID[0]);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_OBJECT_LINEAR);
	GLfloat MyPlaneS[] = { 0.5,0,0,0.5 }; // texCoord = params[0] * objX + params[1] * objY + params[2] * objZ + params[3]
	GLfloat MyPlaneT[] = { 0,0.5,0,0.5 }; // Where objX, objY, objZ are the vertex position in object space.   ex> (x,y) : (-1,-1) ~ (1,1) 
	glTexGenfv(GL_S, GL_OBJECT_PLANE, MyPlaneS);
	glTexGenfv(GL_T, GL_OBJECT_PLANE, MyPlaneT);

	glutSolidCube(2);
	glBindTexture(GL_TEXTURE_2D,0); // NULL
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
	case 'Q':
		mEditMode = EditMode::CAMERA_TRANSFORM;
		break;
	case 'w':
	case 'W':
		mCoordSel = CoordSel::XYZ;
		mEditMode = EditMode::OBJECT_TRANSLATE;
		break;
	case 'e':
	case 'E':
		mCoordSel = CoordSel::XYZ;
		mEditMode = EditMode::OBJECT_ROTATE;
		break;
	case 'r':
	case 'R':
		mCoordSel = CoordSel::XYZ;
		mEditMode = EditMode::OBJECT_SCALE;
		break;
	case 'x':
	case 'X':
		mCoordSel = CoordSel::X;
		break;
	case 'y':
	case 'Y':
		mCoordSel = CoordSel::Y;
		break;
	case 'z':
	case 'Z':
		mCoordSel = CoordSel::Z;
		break;
	case 'a':
	case 'A':
		mCoordSel = CoordSel::XYZ;
		break;
	case 'c':
	case 'C':
		printf("============== Capture ================\n");
		printf("Translation - x : %f , y : %f , z : %f\n",mTranslation.x,mTranslation.y,mTranslation.z);
		printf("Rotation - x : %f , y : %f , z : %f\n",mRotation.x, mRotation.y, mRotation.z);
		printf("Scale - x : %f , y : %f , z : %f\n", mScale.x, mScale.y, mScale.z);
		printf("=======================================\n");
		break;
	case 'b':
	case 'B':
		glsInitTransform();
		glutPostRedisplay();
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
			if (mMouse == MouseState::LEFT_DOWN) {
				switch (mCoordSel) {
				case CoordSel::X:
					mTranslation = mTranslation + Vector3(speed * 10,0,0) * ( ((mCen - mEye).crossProduct(Vector3(0,1,0))*Vector3(1,0,0)).x > 0 ? 1 : -1)  * (x - mMousePosX >= 0 ? 1 : -1);
					break;
				case CoordSel::Y:
					mTranslation = mTranslation + Vector3(0,speed * 10, 0) * (mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::Z:
					mTranslation = mTranslation + Vector3(0,0,speed * 10) * (((mCen - mEye).crossProduct(Vector3(0, 1, 0)) * Vector3(0, 0, 1)).z > 0 ? 1 : -1) * (x - mMousePosX >= 0 ? 1 : -1);
					break;
				case CoordSel::XYZ:
					mTranslation = mTranslation + (mCen - mEye).crossProduct(mUp).normalize() * (GLfloat)(x - mMousePosX) * speed + mUp.normalize() * (GLfloat)(mMousePosY - y) * speed;
					break;
				}
			}
			break;
		case EditMode::OBJECT_ROTATE:
			if (mMouse == MouseState::LEFT_DOWN) {
				switch (mCoordSel) {
				case CoordSel::X:
					mRotation = mRotation + Vector3(speed*100, 0, 0) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::Y:
					mRotation = mRotation + Vector3(0, speed * 100, 0) * (x - mMousePosX >= 0 ? 1 : -1);
					break;
				case CoordSel::Z:
					mRotation = mRotation + Vector3(0, 0, speed * 100) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::XYZ:
					mRotation = mRotation + Vector3(speed * 100, speed * 100, speed * 100) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				}
			}
			break;
		case EditMode::OBJECT_SCALE:
			if (mMouse == MouseState::LEFT_DOWN) {
				switch (mCoordSel) {
				case CoordSel::X:
					mScale = mScale + Vector3(speed, 0, 0) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::Y:
					mScale = mScale + Vector3(0, speed, 0) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::Z:
					mScale = mScale + Vector3(0, 0, speed) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				case CoordSel::XYZ:
					mScale = mScale + Vector3(speed, speed, speed) * (x - mMousePosX + mMousePosY - y >= 0 ? 1 : -1);
					break;
				}
			}
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

	glColor4f(0,0.6f,0,1);
	glutWireSphere(0.1, 10, 10);
	glColor4f(1, 0, 0, 1);
	glPushMatrix();
		glTranslatef(0.05 * size, 0, 0);
		glScalef(size, 1, 1);
		glutSolidCube(0.1);
	glPopMatrix();

	glColor4f(0, 1, 0, 1);
	glPushMatrix();
		glTranslatef(0, 0.05 * size, 0);
		glScalef(1, size, 1);
		glutSolidCube(0.1);
	glPopMatrix();

	glColor4f(0, 0, 1, 1);
	glPushMatrix();
		glTranslatef(0, 0, 0.05*size);
		glScalef(1, 1, size);
		glutSolidCube(0.1);
	glPopMatrix();
}

void displayFunc() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(mEye.x,mEye.y,mEye.z, mCen.x, mCen.y, mCen.z, mUp.x, mUp.y, mUp.z);
	drawCoord();
	glCallList(mFloorLineID);

	glsDrawNotEdit();

	glTranslatef(mTranslation.x,mTranslation.y,mTranslation.z);
	glRotatef(mRotation.z, 0, 0, 1);
	glRotatef(mRotation.y, 0, 1, 0);
	glRotatef(mRotation.x, 1, 0, 0);
	glScalef(mScale.x, mScale.y, mScale.z);
	glsDrawEdit();

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

bool glsLoadGLTextures(char* path, GLuint* TextureObject) {
	LoadBMP loadBMP(path);
	if (!(loadBMP).loadData()) {
		printf("%s", (loadBMP).getMSG());
		return false;
	}

	glGenTextures(1, TextureObject);
	glBindTexture(GL_TEXTURE_2D, *TextureObject);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, loadBMP.getWidth(), loadBMP.getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, loadBMP.getData());
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glBindTexture(GL_TEXTURE_2D, 0); // NULL

	return true;
}

void glsSetting() {
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
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
	glAlphaFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR); // 왜 안되는거야 ㅓㅐ로ㅜㅏㅓㅁ뉴ㅜ라ㅓㅁㄴ우ㅠ라
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_LIGHTING); // 조명
	glEnable(GL_COLOR_MATERIAL);

	glClearColor(1, 1, 1, 1);

	glsInitLook();
	glsFloorLine();
	glsInitTransform();
	glsInitTexture();
}
 

int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glsSetting();

	glutMainLoop();
	return 0;
}