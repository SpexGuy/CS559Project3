#include <GL/glew.h>
#include "Window.h"
#include "View.h"
#include "Camera.h"
#include "Drawable.h"
#include "P2Cameras.h"
#include "Function.h"
#include "SpheroidLight.h"
#include "Mesh.h"
#include "Rocket.h"
#include "PointMesh.h"
#include "ILContainer.h"
#include "FrameBuffer.h"
#include "Frame.h"
#include "LinearPathSpawner.h"
#include "FancyPathSpawner.h"
#include "Ribbon.h"
#include <GL/freeglut.h>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


using namespace std;
using namespace glm;



void windowReshape(int x, int y);
void windowClose();
void windowDisplay();
void KeyboardFunc(unsigned char c, int x, int y);
void SpecialFunc(int c, int x, int y);
void TimerFunc(int value);
void MotionFunc(int x, int y);
void PassiveMotionFunc(int x, int y);
void MouseFunc(int button, int state, int x, int y);
void SpecialUpFunc(int c, int x, int y);


class Globals {
public:
	Window *window;
	Frame *view;

	Drawable *mainOverlay;

	PerspectiveProjection *proj;

	SpheroidCamera *vcam;
	BetterCamera *cam;

	SpheroidLight *light[NUM_LIGHTS];

	DrawableGroup *model, *vmodel;
	Drawable *sphere;
	Drawable *sphereCopy;
	Drawable *virtualSphere;
	Drawable *translucentSphere;
	RibbonBuilder *ribbonBuilder;
	FancyPathSpawner *spawner;

	bool lmouse_pressed;
	bool up_pressed;
	bool down_pressed;
	bool right_pressed;
	bool left_pressed;

	Texture *marsTexture;

	vector<Frame *> frames;

	int period;
	bool wireframe;
	int curr_light;

	Globals();
	bool initialize();
	void takeDown();

	virtual ~Globals();

} globals;

Globals::Globals() {}

bool Globals::initialize() {

	period = 1000 / 120;

	proj = new PerspectiveProjection(45.0f);
	proj->setPlanes(0.01f, 100.0f);

	vcam = new SpheroidCamera();
	vcam->setRadius(3.0f);

	//This is kinda deprecated??? 
	vector<char*> text;
	text.push_back("Image credit: http://openuniverse.sourceforge.net/");
	text.push_back("");
	text.push_back("Left/Right UP/DOWN PgUP/PgDn - Camera Controls");
	text.push_back("O/L ;/K - Light Controls");
	text.push_back("W - wireframe");
	text.push_back("P - Pause");
	text.push_back("N - Normals");
	text.push_back("F11 - Full Screen");
	text.push_back("F1 - Cycle Camera/Scene");
	text.push_back("");
	text.push_back("MARS MODE");

	mainOverlay = (new HudOverlay(text))
		->inColor(WHITE)
		->pushDecorator(new ShaderUse(SHADER_SOLID));

	vmodel = new DrawableGroup();

	marsTexture = new ILContainer("earth_texture.jpg");
	virtualSphere = Mesh::newSphere(10, 10, 1.0f, marsTexture)
		->rotated(vec3(1,0,0), 15)
		->animateRotation(vec3(0, 1, 0),
			new LinearTimeFunction(16.0f/1000.0f, 0))
		->inColor(RED)
		->inMaterial(1.0f, 0.0f, 50)
		->useShader(SHADER_TEXTURE);

	Frame *f = (new Frame(ivec2(1024, 1024)))
		->renderStuff(proj, vcam, vmodel, mainOverlay)
		->postProcess(PPO_SEPIA);

	model = new DrawableGroup();

	sphere = Mesh::newSphere(30, 30, 0.05f, f->getFBO())
		//->translated(vec3(-1.0f, 0.0f, 0.0f))
		->inMaterial(0.3f, 1.0f, 50)
		->useShader(SHADER_TEXTURE);
	sphereCopy = sphere->copyStack()
		->translated(vec3(-1.0f, 0.0f, 0.0f));

	ribbonBuilder = new RibbonBuilder(model, vec3(0.0f, 0.0f, -10.0f), 0.25f/1000.0f, 3, 0.5, 50, vec3(1.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	translucentSphere = Mesh::newSphere(5, 10, 0.05f);
	spawner = new FancyPathSpawner(translucentSphere, 2.0f/1000.0f, 0.0f, 0.0f, 1.0f/period, model, 2.0f);

	//set up lights!
	light[0] = new SpheroidLight(0, WHITE);
	light[0]->setAngle(90);
	light[0]->setAxisAngle(90);
	light[0]->setRadius(10);

	light[1] = new SpheroidLight(1, RED);
	light[1]->setAngle(0);
	light[1]->setAxisAngle(90);
	light[1]->setRadius(10);

	light[2] = new SpheroidLight(2, GREEN);
	light[2]->setAngle(90);
	light[2]->setAxisAngle(0);
	light[2]->setRadius(10);

	light[3] = new SpheroidLight(3, BLUE);
	light[3]->setAngle(180);
	light[3]->setAxisAngle(90);
	light[3]->setRadius(10);

	light[4] = new SpheroidLight(4, WHITE);
	light[4]->setAngle(0);
	light[4]->setAxisAngle(180);
	light[4]->setRadius(10);

	//Building the models


	model->addLight(light[0]->animateRotation(vec3(0.0f,0.0f,1.0f), new LinearTimeFunction(16.0f/1000.0f, 0.0f)));
	model->addLight(light[1]->animateRotation(vec3(0.0f,0.0f,1.0f), new LinearTimeFunction(16.0f/1000.0f, 0.0f)));
	model->addLight(light[2]->animateRotation(vec3(0.0f,0.0f,1.0f), new LinearTimeFunction(16.0f/1000.0f, 0.0f)));
	model->addLight(light[3]->animateRotation(vec3(0.0f,0.0f,1.0f), new LinearTimeFunction(16.0f/1000.0f, 0.0f)));
	model->addLight(light[4]->animateRotation(vec3(0.0f,0.0f,1.0f), new LinearTimeFunction(16.0f/1000.0f, 0.0f)));
	model->addElement(sphere);
	model->addElement(sphereCopy);
	vmodel->addLight(light[0]);
	vmodel->addElement(virtualSphere);
	//Building the cameras
	cam = new BetterCamera(vec3(0.0f,0.0f,2.0f));

	view = (new Frame(ivec2(1,1)))
		->postProcess(PPO_PLASMA)
		->renderStuff(proj, cam, model, mainOverlay)
		->postProcess(PPO_STATIC_NOISE);

	window = new SingleViewportWindow(view);

	wireframe = false;

	//setup GL
	if (!window->initialize("Project 3"))
		return false;
	glutReshapeFunc(windowReshape);
	glutCloseFunc(windowClose);
	glutDisplayFunc(windowDisplay);
	glutKeyboardFunc(KeyboardFunc);
	glutSpecialFunc(SpecialFunc);
	glutSpecialUpFunc(SpecialUpFunc);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutMotionFunc(MotionFunc);
	glutMouseFunc(MouseFunc);
	glutTimerFunc(period, TimerFunc, 0);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		return false;
	}

	if (!ShaderFlyweight::inst()->initialize())
		return false;
	if (!Graphics::inst()->initialize())
		return false;
	for(int i = 0; i < NUM_LIGHTS; i++) {
		if (light[i] != NULL && !light[i]->initialize())
			return false;
	}
	if (!sphere->initialize())
		return false;
	if (!sphereCopy->initialize())
		return false;
	if (!virtualSphere->initialize())
		return false;
	if (!mainOverlay->initialize())
		return false;
	if (!marsTexture->initialize())
		return false;
	if (!view->initialize())
		return false;
	if (!f->initialize())
		return false;
	if(!translucentSphere->initialize())
		return false;
	if (!ribbonBuilder->initialize())
		return false;

	frames.push_back(f);

	return true;
}

void Globals::takeDown() {
	for(int i = 0; i < NUM_LIGHTS; i++){
		if(light[i] != NULL)
			light[i]->takeDown();
	}
	Graphics::inst()->takeDown();
	ShaderFlyweight::inst()->takeDown();
	for (uint c = 0; c < frames.size(); c++) {
		frames[c]->takeDown();
	}
	view->takeDown();
}

Globals::~Globals() {

	delete mainOverlay;

	delete view;

	delete window;
	 
	delete proj;

	delete cam;

	delete model;
}

void CloseFunc() {
	glutLeaveMainLoop();
	globals.takeDown();
}

void KeyboardFunc(unsigned char c, int x, int y) {

	switch (c) {
	//toggle wireframe
	case 'w':
		globals.wireframe = !globals.wireframe;
		Graphics::inst()->setWireframe(!Graphics::inst()->isWireframe());
		break;

	//light controls
	case ';':
		if(globals.light[globals.curr_light] != NULL)
		globals.light[globals.curr_light]->addAngle(1);
		break;
	case 'k':
		if(globals.light[globals.curr_light] != NULL)
		globals.light[globals.curr_light]->addAngle(-1);
		break;
	case 'o':
		if(globals.light[globals.curr_light] != NULL)
		globals.light[globals.curr_light]->addAxisAngle(-1);
		break;
	case 'l':
		if(globals.light[globals.curr_light] != NULL)
		globals.light[globals.curr_light]->addAxisAngle(1);
		break;
	case '1':
		globals.curr_light = 0;
		break;
	case '2':
		globals.curr_light = 1;
		break;
	case '3':
		globals.curr_light = 2;
		break;
	case '4':
		globals.curr_light = 3;
		break;
	case '5':
		globals.curr_light = 4;
		break;
	case '+':
		Graphics::inst()->setTimeScale(Graphics::inst()->getTimeScale() + 0.5f);
		break;
	case '-':
		Graphics::inst()->setTimeScale(Graphics::inst()->getTimeScale() - 0.5f);
		break;

	case 's':
		globals.spawner->spawn();
		break;

	//quit
	case 'x':
	case 27:
		glutLeaveMainLoop();
		return;
	}
}

void SpecialFunc(int c, int x, int y) {

	switch (c) {
		case GLUT_KEY_F11:
			globals.window->toggleFullscreen();
			break;

		//------ Camera Controls ------
		case GLUT_KEY_PAGE_UP:
			globals.proj->addFov(1.0f);
			break;
		case GLUT_KEY_PAGE_DOWN:
			globals.proj->addFov(-1.0f);
			break;
		case GLUT_KEY_UP:
			globals.up_pressed = true;
			break;
		case GLUT_KEY_DOWN:
			globals.down_pressed = true;
			break;
		case GLUT_KEY_LEFT:
			globals.left_pressed = true;
			break;
		case GLUT_KEY_RIGHT:
			globals.right_pressed = true;
			break;
	}
}

void SpecialUpFunc(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_UP:
		globals.up_pressed = false;
		break;
	case GLUT_KEY_DOWN:
		globals.down_pressed = false;
		break;
	case GLUT_KEY_LEFT:
		globals.left_pressed = false;
		break;
	case GLUT_KEY_RIGHT:
		globals.right_pressed = false;
		break;
	}
}

void MouseFunc(int button, int state, int x, int y) {
	//if not left mouse, clear the mouse delta stuff
	if(button == GLUT_LEFT_BUTTON)
		globals.lmouse_pressed = true;
	else
		globals.lmouse_pressed = false;
}
void MotionFunc(int x, int y) {
	//update camera if lmouse pressed
	if(globals.lmouse_pressed)
		globals.cam->mouseUpdate(ivec2(x,y));
	//update previous mouse position otherwise - need it for delta!
	else
		globals.cam->mouseClear(ivec2(x,y));
}
void PassiveMotionFunc(int x, int y) {
	//clear the mouse delta when no buttons pressed
	globals.lmouse_pressed = false;
	//also update the mouse delta stuff
	globals.cam->mouseClear(ivec2(x,y));
}

void TimerFunc(int value) {
	if (!globals.window->isClosed()) {
		//keep state in Graphics
		Graphics::inst()->update();
		//request next call
		glutTimerFunc(globals.period, TimerFunc, value);
		//redraw window
		globals.window->update();

		//update the camera position!
		if(globals.down_pressed)
			globals.cam->moveUp(-0.01f);
		if(globals.up_pressed)
			globals.cam->moveUp(0.01f);
		if(globals.right_pressed)
			globals.cam->moveRight(0.01f);
		if(globals.left_pressed)
			globals.cam->moveRight(-0.01f);
	}
}

//boilerplate global functions to pass directly to window
void windowReshape(int x, int y) {
	globals.window->reshape(x, y);
}
void windowClose() {
	globals.window->onClose();
	CloseFunc();
}
void windowDisplay() {
	for (uint c = 0; c < globals.frames.size(); c++) {
		globals.frames[c]->render();
	}
	globals.spawner->update();
	globals.ribbonBuilder->update();
	globals.window->render();
}



int main(int argc, char * argv[]) {
	//initialize libraries
	glutInit(&argc, argv);
	ilInit();
	iluInit();
	ilutInit();

	//init global arguments
	glutInitWindowSize(1024, 512);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);

	//init globals
	if (!globals.initialize())
		return -1;

	//start program
	glutMainLoop();
}
