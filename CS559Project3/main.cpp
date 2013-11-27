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
void PassiveMotionFunc(int x, int y);



class Globals {
public:
	Window *window;
	View *view;

	Drawable *mainOverlay;

	PerspectiveProjection *proj;

	SpheroidCamera *cam;

	SpheroidLight *light[NUM_LIGHTS];

	DrawableGroup *model;
	Drawable * sphere;
	Drawable * virtualSphere;

	Texture *marsTexture;

	FrameBufferObject *genTexture;

	TimeFunction<float> *sphereRotation;

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

	marsTexture = new ILContainer("earth_texture.jpg");
	genTexture = new FrameBufferObject(ivec2(1024, 1024), 1, true);

	//Creates the models which will hold the meshes for a given Scene.
	model = new DrawableGroup();

	sphereRotation = new LinearTimeFunction(16.0f/1000.0f, 0);

	virtualSphere = Mesh::newSphere(10, 10, 1.0f, marsTexture)
		//->rotated(vec3(1,0,1), 15)
		//->animateRotation(vec3(0, 1, 0), sphereRotation)
		//->inColor(RED)
		->inMaterial(0.3f, vec4(1.0f), 50)
		->pushDecorator(new ShaderUse(SHADER_TEXTURE));

	sphere = Mesh::newSphere(10, 10, 1.0f, genTexture)
		->rotated(vec3(1,0,1), 15)
		->animateRotation(vec3(0, 1, 0), sphereRotation)
		->inColor(RED)	
		->inMaterial(0.3f, vec4(1.0f), 50)
		->pushDecorator(new ShaderUse(SHADER_TEXTURE));

	light[0] = new SpheroidLight(0, WHITE);
	light[0]->setAngle(90);
	light[0]->setAxisAngle(90);
	light[0]->setRadius(15);

	light[1] = new SpheroidLight(1, RED);
	light[1]->setAngle(0);
	light[1]->setAxisAngle(90);
	light[1]->setRadius(15);

	//clear out the lights we don't use
	for(int i = 2; i < NUM_LIGHTS; i++)
		light[i] = NULL;

	//Building the models
	model->addLight(light[0]);
	model->addLight(light[1]);
	model->addElement(sphere);
	//Building the cameras
	cam = new SpheroidCamera();
	cam->setRadius(3.0f);

	//Building the overlays
	//Setting the hud overlays to display the associated text.
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

	view = new View(proj, cam, model, mainOverlay);

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
	glutTimerFunc(period, TimerFunc, 0);
	glutPassiveMotionFunc(PassiveMotionFunc);
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glEnable(GL_CULL_FACE);
	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	
	if (glewInit() != GLEW_OK) {
		cerr << "GLEW failed to initialize." << endl;
		return false;
	}

	if (!ShaderFlyweight::inst()->initialize())
		return false;
	if (!Graphics::inst()->initialize())
		return false;
	for(int i = 0; i < NUM_LIGHTS; i++)
	{
		if (light[i] != NULL && !light[i]->initialize())
			return false;
	}
	sphere->initialize();
	virtualSphere->initialize();
	mainOverlay->initialize();
	marsTexture->initialize();
	genTexture->initialize();

	genTexture->bindDraw();
	Graphics::inst()->setProjection(proj->generateProjectionMatrix());
	Graphics::inst()->setView(cam->generateViewMatrix());
	Graphics::inst()->setWireframe(true);
	for(int i = 0; i < NUM_LIGHTS; i++){
		if(light[i] != NULL)
			light[i]->draw(mat4(1.0f));
	}
	virtualSphere->draw(mat4(1.0f));
	genTexture->unbindDraw();
	Graphics::inst()->setWireframe(false);

	return true;
}

void Globals::takeDown() {
	for(int i = 0; i < NUM_LIGHTS; i++){
		if(light[i] != NULL)
			light[i]->takeDown();
	}
	Graphics::inst()->takeDown();
	ShaderFlyweight::inst()->takeDown();
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

/** this function is adapted from a post by Steven Canfield
 * on StackOverflow.com:
 * http://stackoverflow.com/questions/728049/glutpassivemotionfunc-and-glutwarpmousepointer
 */
int lastX = 512;
int lastY = 256;
void PassiveMotionFunc(int x, int y) {
	////short-circuit return if not in free-fly mode
	//if(globals.Scenes[globals.currentScene] != globals.marsScene
	//		|| globals.marsScene->getCurrentCameraIndex() != 1)
		return;

	//int deltaX = x - lastX;
	//int deltaY = y - lastY;

	////short-circuit return if no movement
	//if (deltaX == 0 && deltaY == 0)
	//	return;

	//lastX = x;
	//lastY = y;

	//ivec2 size = Graphics::inst()->getSize();
	//int centerx = size.x/2;
	//int centery = size.y/2;
	//
	//globals.flyCam->addAngle(-deltaX/10.0f);
	//globals.flyCam->addAxisAngle(deltaY/10.0f);

	////warping the pointer triggers PassiveMotionFunc, so should be done sparingly.
	////we choose to do it when the cursor is within 10 pixels of the edge of the screen.
	//if(x <= 10 || (y) <= 10 || x >= size.x-10 || y >= size.y-10) {
	//	lastX = centerx;
	//	lastY = centery;
	//	glutWarpPointer( lastX, lastY );
	//}
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
		case GLUT_KEY_CTRL_R:
			globals.cam->moveForward(1.0f);
			break;
		case GLUT_KEY_CTRL_L:
			globals.cam->moveForward(-1.0f);
			break;
		case GLUT_KEY_LEFT:
			globals.cam->moveRight(-1.0f);
			break;
		case GLUT_KEY_RIGHT:
			globals.cam->moveRight(1.0f);
			break;
		case GLUT_KEY_UP:
			globals.cam->moveUp(1.0f);
			break;
		case GLUT_KEY_DOWN:
			globals.cam->moveUp(-1.0f);
			break;
		case GLUT_KEY_PAGE_UP:
			globals.proj->addFov(1.0f);
			break;
		case GLUT_KEY_PAGE_DOWN:
			globals.proj->addFov(-1.0f);
			break;
	}
}

void TimerFunc(int value) {
	if (!globals.window->isClosed()) {
		//keep state in Graphics
		Graphics::inst()->update();
		//request next call
		glutTimerFunc(globals.period, TimerFunc, value);
		//redraw window
		globals.window->update();
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
