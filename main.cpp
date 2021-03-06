#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdlib.h>
#include <time.h>

#include "tinyxml2.h"

#include "Car.h"
#include "Shot.h"
#include "GameObject.h"

#define IN  0
#define OUT 1

using namespace tinyxml2;

GLuint carTex;
GLuint gunTex;
GLuint gndTex;
GLuint walTex;
GLuint strTex;
GLuint losTex;
GLuint winTex;

int keystatus[256];
float last_x,last_y;
std::vector<Car> enemies;
std::vector<Shot> player_bullets;
std::vector<Shot> enemy_bullets;
float enemy_speed;
float enemy_shoot_speed;
float enemy_shot_freq;
float enemy_last_shot = 0;
Circle lane[2];
Car player;
Rectangle startMark;
float decision_time=0;
int decision;
int win = 0;
int lap = 0;
int camera = 1;

bool rotating = false;
float rot_phi = 90;
float rot_theta = 0;
float x_press,y_press;

float cur_time, last_time;

void GameOver()
{
	glMatrixMode(GL_PROJECTION);
	glColor3f(1,0,0);
	glLoadIdentity();
    //Create a string to be printed
	static char str[10];
    char *tmpStr;
    sprintf(str, "GAME OVER");
    //Define the position to start printing
    glRasterPos2f(-.1, 0);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = str;
    //Print each of the other Char at time
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmpStr);
        tmpStr++;
    }

}

void YouWin()
{
	glColor3f(0,1,0);
    //Create a string to be printed
	static char str[10];
    char *tmpStr;
    sprintf(str, "YOU WIN");
    //Define the position to start printing
    glRasterPos2f(-.1, 0);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = str;
    //Print each of the other Char at time
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *tmpStr);
        tmpStr++;
    }

}

void PrintTime(GLfloat x, GLfloat y)
{
    //Create a string to be printed
    char *tmpStr;
	static char str[10];
	
    sprintf(str, "%02d:%02d:%03d",(int)cur_time/60000,((int)cur_time/1000)%60,(int)cur_time%1000);
    //Define the position to start printing
    glRasterPos2f(x, y);
    //Print  the first Char with a certain font
    //glutBitmapLength(font,(unsigned char*)str);
    tmpStr = str;
    //Print each of the other Char at time
    while( *tmpStr ){
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *tmpStr);
        tmpStr++;
    }

}

void draw_arena(void){

		if (keystatus['t']||keystatus['T'])
			glDisable(GL_TEXTURE_2D);

		glColor3f(1,1,1);

		draw_ground(lane[OUT],gndTex);
		draw_inner_wall(lane[IN],walTex, player.get_size()*2);
		draw_out_wall(lane[OUT],walTex, player.get_size()*2);
		draw_start_mark(startMark,strTex);

		for(std::vector<Car>::iterator it = enemies.begin() ; it != enemies.end(); ++it){
			it->draw();
		} 

		player.draw();

		for(std::vector<Shot>::iterator it = enemy_bullets.begin() ; it != enemy_bullets.end(); ++it){
			it->draw();
		} 

		for(std::vector<Shot>::iterator it = player_bullets.begin() ; it != player_bullets.end(); ++it){
			it->draw();
		}

}
void initLighting()
{

    glEnable(GL_LIGHTING);

	GLfloat qaAmbientLight[]    = {0.1, 0.1, 0.1, 1.0};
	GLfloat qaDiffuseLight[]    = {1, 1, 1, 1.0};
	GLfloat qaSpecularLight[]   = {1.0, 1.0, 1.0, 1.0};
	GLfloat qaLightPosition[]   = {lane[OUT].cx, lane[OUT].cy, player.get_size()*10};
	if (keystatus['l']||keystatus['L'])
    	glDisable(GL_LIGHT0);
	else
		glEnable(GL_LIGHT0);
    // Set lighting intensity and color
    glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT0, GL_POSITION, qaLightPosition);
    glLightfv(GL_LIGHT0, GL_SPECULAR, qaSpecularLight);
    ////////////////////////CAR//////////////////////////
	/*
	GLfloat farol1_position[]  = {-24.64616, -12.31065, -237.37874};
	GLfloat farol2_position[] = {-2.0064  , -12.31065, -237.37874};
	GLfloat farol3_position[] = {17.87886 , -12.31065, -237.37874};
	GLfloat farol4_position[] = {39.91863 , -12.31065, -237.37874};
	
	GLfloat farol1_direction[] = {0,0, - 1};
	GLfloat teste[]  = {lane[OUT].cx, lane[OUT].cy, player.get_size()*3};
 	glEnable(GL_LIGHT1);
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight);
    glLightfv(GL_LIGHT1, GL_POSITION, teste);
    glLightfv(GL_LIGHT1, GL_SPECULAR, qaSpecularLight);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);// set cutoff angle
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, farol1_direction); 
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 1); // set focusing strength */
	
}
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (win == -1){
		glClearColor(0,0,0,0.0);
		glColor3f(1,1,1);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glViewport(0,0,500,500);	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		PrintTime(-0.2,1.0);
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0,0,1,0,0,0,0,1,0);
		Circle c;
		c.cx = c.cy = 0;
		c.radius = 1;
		draw_ground(c,losTex);
	}else if(win == 1){
		glClearColor(0,0,0,0.0);
		glColor3f(1,1,1);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
		glViewport(0,0,500,500);	
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glDisable(GL_TEXTURE_2D);
		PrintTime(-0.2,1.0);
		glEnable(GL_TEXTURE_2D);
		glMatrixMode(GL_MODELVIEW);
		gluLookAt(0,0,1,0,0,0,0,1,0);
		Circle c;
		c.cx = c.cy = 0;
		c.radius = 1;
		draw_ground(c,winTex);
	}else{	

		glViewport(0,0,500,500);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		PrintTime(-0.2,1.0);
		gluPerspective(60.0f, 1, 0.01, 3000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		float cannon_x = (1.79612187838555+7.93621087778903)*0.00234324850820599-0.001;
		float cannon_z = (36.7102507390688-35.6207264253114)*0.00234324850820599;
		float cannon_y = (162.187749878032-88.1413474731246)*0.00234324850820599;
		float x = player.get_x() + (cannon_x*cos(player.get_theta()*M_PI/180.0)-cannon_z*sin(player.get_theta()*M_PI/180.0))*player.get_size();
		float y = player.get_y() + (cannon_x*sin(player.get_theta()*M_PI/180.0)+cannon_z*cos(player.get_theta()*M_PI/180.0))*player.get_size();
		float z = player.get_size()/5.3 + (cannon_y)*player.get_size();

		if(camera == 1)	{
			gluLookAt(  player.get_x()-(player.get_size()/12)*cos(player.get_theta()*M_PI/180),
						player.get_y()-(player.get_size()/12)*sin(player.get_theta()*M_PI/180),
						player.get_size()/4,
						player.get_x()-100*sin(player.get_theta()*M_PI/180),
						player.get_y()+100*cos(player.get_theta()*M_PI/180),
						player.get_size()/4,
						0, 0, 1.0);
		}
		else if(camera == 2) {
			gluLookAt(	x+0.02*sin(player.get_cannon_phi()*M_PI/180)*sin(player.get_cannon_theta()*M_PI/180)*player.get_size(), 
				  		y-0.02*sin(player.get_cannon_phi()*M_PI/180)*cos(player.get_cannon_theta()*M_PI/180)*player.get_size(),
				  		z+0.02*cos(player.get_cannon_phi()*M_PI/180)*player.get_size(),
						x-(15*sin((player.get_theta()+player.get_cannon_theta())*M_PI/180)*cos(player.get_cannon_phi()*M_PI/180)), 
				  		y+(15*cos((player.get_theta()+player.get_cannon_theta())*M_PI/180)*cos(player.get_cannon_phi()*M_PI/180)),
				  		z+15*sin(player.get_cannon_phi()*M_PI/180),
						0,0,1);
		}
		else { 
			if (rot_phi==90){
				gluLookAt(	player.get_x() + (player.get_size())*sin((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_y() - (player.get_size())*cos((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_size()/6 + (player.get_size())*sin(rot_phi*M_PI/180),
							player.get_x(),
							player.get_y(),
							player.get_size()/6	,
							0, 1, 0);
			}
			else if (rot_phi<90){
				gluLookAt(	player.get_x() + (player.get_size())*sin((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_y() - (player.get_size())*cos((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_size()/6 + (player.get_size())*sin(rot_phi*M_PI/180),
							player.get_x(),
							player.get_y(),
							player.get_size()/6	,
							0, 0, 1.0);
			}
			else{
				gluLookAt(	player.get_x() + (player.get_size())*sin((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_y() - (player.get_size())*cos((player.get_theta()+rot_theta)*M_PI/180)*cos(rot_phi*M_PI/180),
							player.get_size()/6 + (player.get_size())*sin(rot_phi*M_PI/180),
							player.get_x(),
							player.get_y(),
							player.get_size()/6	,
							0, 0, -1.0); 
			}
		}
		
		initLighting();

		if (keystatus[' ']){
			glDisable(GL_TEXTURE_2D);
			glPushMatrix();
				glColor3f(0.9,0,0);
				glTranslatef(x-(15*sin((player.get_theta()+player.get_cannon_theta())*M_PI/180)*cos(player.get_cannon_phi()*M_PI/180)), 
					  y+(15*cos((player.get_theta()+player.get_cannon_theta())*M_PI/180)*cos(player.get_cannon_phi()*M_PI/180)),
					  z+15*sin(player.get_cannon_phi()*M_PI/180));
				glutSolidSphere(1,10,10);
			glPopMatrix();

			glPushMatrix();
				glColor3f(0,0.9,0);
				glTranslatef(x+0.05*sin(player.get_cannon_phi()*M_PI/180)*sin(player.get_cannon_theta()*M_PI/180)*player.get_size(), 
					  y-0.05*sin(player.get_cannon_phi()*M_PI/180)*cos(player.get_cannon_theta()*M_PI/180)*player.get_size(),
					  z+0.05*cos(player.get_cannon_phi()*M_PI/180)*player.get_size());
				glutSolidSphere(1,10,10);
			glPopMatrix();
			glEnable(GL_TEXTURE_2D);
		}
		
		if(win ==0)
			draw_arena();

		glViewport(0,500,500,200);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		gluPerspective(60.0f, 2.5, 0.01, 3000);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		gluLookAt(	player.get_x(),
					player.get_y(),
					player.get_size()/2.5,
					player.get_x()-100*sin((player.get_theta()+180)*M_PI/180),
					player.get_y()+100*cos((player.get_theta()+180)*M_PI/180),
					player.get_size()/2.5,
					0, 0, 1.0);
		initLighting();
			draw_arena();
	}
	 glutSwapBuffers();
}

void init (void){

	glClearColor(1,1,1,0.0);
	glClearDepth(10.0f);                   // Set background depth to farthest
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, 1, 0.01, 3000);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	carTex = LoadTextureRAW("models/chassi/Tex_0020_1.png");
	gunTex = LoadTextureRAW("models/gun/Tex_0019_1.png");
	gndTex = LoadTextureRAW("models/ground.png");
	walTex = LoadTextureRAW("models/fence.png");
	strTex = LoadTextureRAW("models/start.png");
	losTex = LoadTextureRAW("models/lose.png");
	winTex = LoadTextureRAW("models/win.png");	
   	glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   	glShadeModel(GL_SMOOTH);   // Enable smooth shading
}

void svgParser(const char* path){
	XMLDocument xml_doc;
	xml_doc.LoadFile(path);

	XMLNode* root = xml_doc.FirstChild();

	bool lane_first = true;
	for(XMLElement* e = root->FirstChildElement("circle"); e != NULL; e = e->NextSiblingElement("circle"))
	{
		std::string id = e->Attribute("id");
		Circle c;
		c.cx = e->FloatAttribute("cx");
		c.cy = -e->FloatAttribute("cy");
		c.radius = e->FloatAttribute("r");
		std::string color = e->Attribute("fill");
		if (color == "red"){
			c.r = 1;
			c.g = c.b = 0;
		}else if(color == "blue"){
			c.b = 1;
			c.g = c.r = 0;
		}else if(color == "green"){
			c.g = 1;
			c.r = c.b = 0;
		}else if(color == "black"){
			c.r = c.g = c.b = 0;
		}else if(color == "white"){
			c.r = c.g = c.b = 1;
		}

		if (id == "Inimigo"){
			Car enemy;
			enemy.set_color(c.r,c.g,c.b);
			enemy.set_size(c.radius*2);
			enemy.set_pos(c.cx,c.cy,0);
			enemy.set_speed(enemy_speed);
			enemy.set_bullet_speed(enemy_shoot_speed);
			enemies.push_back(enemy);
		}else if (id == "Jogador"){
			player.set_color(c.r,c.g,c.b);
			player.set_size(c.radius*2);
			player.set_pos(c.cx,c.cy,0);
		}else if (id == "Pista"){
			if (lane_first){
				lane[IN] = c;
				lane_first = false;
			}else{
				if (lane[IN].radius<c.radius){
					lane[OUT] = c;
				}else{
					lane[OUT] = lane[IN];
					lane[IN] = c;
				}	
			}
		}	
	}

	XMLElement* e = root->FirstChildElement("rect");
	Rectangle r;
	r.x = e->FloatAttribute("x");
	r.y = -e->FloatAttribute("y");
	r.w = e->FloatAttribute("width");
	r.h = -e->FloatAttribute("height");
	std::string color = e->Attribute("fill");
	if (color == "red"){
		r.r = 1;
		r.g = r.b = 0;
	}else if(color == "blue"){
		r.b = 1;
		r.g = r.r = 0;
	}else if(color == "green"){
		r.g = 1;
		r.r = r.b = 0;
	}else if(color == "black"){
		r.r = r.g = r.b = 0;
	}else if(color == "white"){
		r.r = r.g = r.b = 1;
	}
	startMark = r;


}

void parser(const char* path){
	XMLDocument xml_doc;
	xml_doc.LoadFile(path);

	XMLNode* root = xml_doc.FirstChild();
   
	XMLElement* carElem = root->FirstChildElement("carro");
	
	player.set_speed(carElem->FloatAttribute("velCarro"));
	player.set_bullet_speed(carElem->FloatAttribute("velTiro"));

	carElem = root->FirstChildElement("carroInimigo");
	
	enemy_speed = carElem->FloatAttribute("velCarro");
	enemy_shoot_speed = carElem->FloatAttribute("velTiro");
	enemy_shot_freq = carElem->FloatAttribute("freqTiro");

	XMLElement* arenaElem = root->FirstChildElement("arquivoDaArena");	

	std::string arenaPath;
	arenaPath = std::string(arenaElem->Attribute("caminho"));

	arenaPath += '/' + std::string(arenaElem->Attribute("nome"));
	
	arenaPath += '.' + std::string(arenaElem->Attribute("tipo"));

	svgParser(arenaPath.c_str());
}

void keyup(unsigned char key,int x,int y){
	keystatus[key] = 0;
}

void keydown(unsigned char key,int x,int y){
	keystatus[key] = 1;
}

void idle(void)
{
	
	if (lap > 0 && win == 0){
		win = 1;
		glClearColor(0,0,0,0.0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-1,1,-1,1,-1.0,1.0);
	}

	if(win == 0)
		cur_time = glutGet(GLUT_ELAPSED_TIME);
	float delta = cur_time-last_time;
	last_time = cur_time;
	Car old_player = player;	
	if (keystatus['w']||keystatus['W']){
		player.forward(delta);
	}
	else if (keystatus['s']||keystatus['S']){
		player.back(delta);
	}
	if (keystatus['a']||keystatus['A']){
		player.right(delta);
	}
	else if (keystatus['d']||keystatus['D']){
		player.left(delta);
	}

	if(keystatus['1']){
		camera = 1;
	}

	if(keystatus['2']){
		camera = 2;
	}	

	if(keystatus['3']){
		camera = 3;
	}

	if (!player.on_lane(lane[IN],lane[OUT]))
		player = old_player;
	else{
		for(std::vector<Car>::iterator it = enemies.begin() ; it != enemies.end(); ++it){
			GameObject* enemy = &(*it);
			
			if (player.colide(enemy)){
				player = old_player;
				break;
			}
		}
	}

	if (player.get_x()>startMark.x && player.get_x()<startMark.x+startMark.w){
		if(old_player.get_y()<startMark.y+startMark.h && player.get_y()>=startMark.y+startMark.h)
			lap ++;
		if(player.get_y()<startMark.y+startMark.h && old_player.get_y()>=startMark.y+startMark.h)
			lap --;
	}

//Enemy bullets
	for(int i=0; i<enemy_bullets.size(); i++){
		if (!enemy_bullets.at(i).refresh_pos(delta)){
			enemy_bullets.erase(enemy_bullets.begin()+i);
		}else{
			if (enemy_bullets.at(i).get_z()>player.get_size())
				break;
			GameObject* enemy_b = &enemy_bullets.at(i);
			if (player.colide(enemy_b)){
				enemy_bullets.erase(enemy_bullets.begin()+i);
				win = -1;
				glClearColor(0,0,0,0.0);

				glMatrixMode(GL_PROJECTION);
				glLoadIdentity();
				glOrtho(-1,1,-1,1,-1.0,1.0);
			}
		}
	}
//Enemy cannon
	for (int j=0; j < enemies.size(); j++){
		float diff_angle = (float)atan2(player.get_x()-enemies.at(j).get_x(),player.get_y()-enemies.at(j).get_y())*180.0/M_PI;
		enemies.at(j).set_cannon_angle((-enemies.at(j).get_theta()-diff_angle));
		if (enemy_last_shot < cur_time){
			Shot bullet = enemies.at(j).fire();
			bullet.set_max_distance (lane[OUT].radius*2);     	
			enemy_bullets.push_back(bullet);
		}
	}
	if (enemy_last_shot < cur_time){
			enemy_last_shot = cur_time + 1.0/enemy_shot_freq;
	}
//Enemy move
	if (decision_time < cur_time){
		decision = rand() ;
		decision_time = cur_time+decision%10*100;
	}
	for (int j=0; j < enemies.size(); j++){
		int enmemy_dec = (decision+25*j)%101;
		Car old_enemy = enemies.at(j);	
		if (enmemy_dec < 20 ){
			enemies.at(j).back(delta);
		}else if (enmemy_dec < 30){
			enemies.at(j).right(delta);
		}else if (enmemy_dec < 50){
			enemies.at(j).right(delta);
			enemies.at(j).forward(delta);
		}else if (enmemy_dec > 90){
			enemies.at(j).left(delta);
		}else if (enmemy_dec > 70){
			enemies.at(j).left(delta);
			enemies.at(j).forward(delta);
		}else{
			enemies.at(j).forward(delta);
		}
		if (!enemies.at(j).on_lane(lane[IN],lane[OUT])){
			enemies.at(j) = old_enemy;
		}
		GameObject* enemy = &enemies.at(j);
		if (player.colide(enemy)){
			enemies.at(j) = old_enemy;
		}
		for (int i = 0; i < enemies.size();i++){
			if (i==j)
				continue;

			enemy = &enemies.at(i);
			
			if (enemies.at(j).colide(enemy)){
				enemies.at(j) = old_enemy;
			}
		}
	}
//Player Bullets
	for(int i=0; i<player_bullets.size(); i++){
		if (!player_bullets.at(i).refresh_pos(delta)){
			player_bullets.erase(player_bullets.begin()+i);
		}else{
			for (int j=0; j < enemies.size(); j++){
				GameObject* enemy = &enemies.at(j);
				if (player_bullets.at(i).get_z()>player.get_size()*0.5)
					break;
				if (player_bullets.at(i).colide(enemy)){
					player_bullets.erase(player_bullets.begin()+i);
					enemies.erase(enemies.begin()+j);
					break;
				}
			}
		} 
	}
	
	glutPostRedisplay();
}

void mouseMove(int x, int y)
{
	player.inc_cannon_angle((last_x-x)/4.0);
	last_x = x;
	player.inc_cannon_phi((last_y-y)/4.0);
	last_y = y;
	glutPostRedisplay();
}

void mouseMovePressed(int x, int y)
{
	if(camera == 3 && rotating){
		rot_theta -= ((float)x-x_press)/4;
		if (rot_theta > 180)
			rot_theta = 180;
		if (rot_theta < -180)
			rot_theta = -180;
		x_press = x;
		rot_phi += ((float)y-y_press)/4;
		if (rot_phi > 180)
			rot_phi = 180;
		if (rot_phi < 0)
			rot_phi = 0;
		y_press = y;
	}
}

void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Shot bullet = player.fire();
		bullet.set_max_distance (lane[OUT].radius*2);     	
		player_bullets.push_back(bullet); 
	}
	if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rotating = true;
		x_press = x;
		y_press = y;
	}else
		rotating = false;
	
	glutPostRedisplay();			
}

int main (int argc, char** argv){

	std::string path = "config.xml";

	if (argc > 1)
		path = std::string(argv[1])+"/"+path;
	
	glutInit(&argc,argv);

	parser(path.c_str());

	glutInitWindowSize(500,700);
	glutCreateWindow("Tank war");
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB| GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	init();
	glutDisplayFunc(display);
	glutKeyboardUpFunc(keyup);
	glutKeyboardFunc(keydown);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMovePressed);
	glutPassiveMotionFunc(mouseMove);
	glutIdleFunc(idle);

	srand (time(NULL));

	glutMainLoop();

	return 0;
}
