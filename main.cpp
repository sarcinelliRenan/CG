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

float cur_time, last_time;

void GameOver()
{
	glColor3f(1,0,0);
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

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();

	if (win == -1){
		GameOver();
		PrintTime(0,-0.5);
	}else if(win == 1){
		YouWin();
		PrintTime(0,-0.5);
	}else{	

	glViewport(0,0,500,500);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1, 0.01, 3000);
	glMatrixMode(GL_MODELVIEW);
	//PrintTime(lane[OUT].cx+lane[OUT].radius-82,lane[OUT].cy+lane[OUT].radius-12);	
		
	   /* gluLookAt(	player.get_x()-(player.get_size()/12)*cos(player.get_theta()*M_PI/180),
					player.get_y()-(player.get_size()/12)*sin(player.get_theta()*M_PI/180),
					player.get_size()/2,
					player.get_x()-100*sin(player.get_theta()*M_PI/180),
					player.get_y()+100*cos(player.get_theta()*M_PI/180),
					player.get_size()/2,
					0, 0, 1.0);
*/
		float cannon_x = (1.79612187838555+7.93621087778903)*0.00234324850820599-0.001;
		float cannon_z = (36.7102507390688-35.6207264253114)*0.00234324850820599;
		float cannon_y = (162.187749878032-88.1413474731246)*0.00234324850820599;

/*	glPushMatrix();
		glTranslatef(player.get_x(),player.get_y(),player.get_size()/5.3);
		glRotatef(player.get_theta(),0,0,1);
		glRotatef(0,0,0,1);
		glRotatef(90,1,0,0);
		glScalef(player.get_size(),player.get_size(),player.get_size());

		gluLookAt(	cannon_x-0.02*sin(player.get_theta()*M_PI/180)*sin(0*M_PI/180) , cannon_y+0.02*cos(0*M_PI/180), cannon_z+0.02*sin(0*M_PI/180)*cos(player.get_theta()*M_PI/180),
						cannon_x+sin(player.get_theta()*M_PI/180)*cos(0*M_PI/180), cannon_y+sin(0*M_PI/180),cannon_z-cos(player.get_theta()*M_PI/180)*cos(0*M_PI/180),
						0, 1, 0);
		glPopMatrix();
/*
		/*gluLookAt(player.get_x() + (cannon_x-0.02*sin(player.get_theta()*M_PI/180))*player.get_size(),
				  player.get_y() + (cannon_z + 0.02*cos(player.get_theta()*M_PI/180))*player.get_size(),	
				  player.get_size()*(12/30) + (cannon_y + 0.02)*player.get_size(), 
				  (cannon_x+sin(player.get_theta()*M_PI/180))*100*player.get_size(), 
				  cannon_y*100*player.get_size(),
				  (cannon_z-cos(player.get_theta()*M_PI/180))*player.get_size(),
				  0, 0, 1.0);

		gluLookAt(	player.get_x()-(player.get_size()/12)*cos(player.get_theta()*M_PI/180),
					player.get_y()-(player.get_size()/12)*sin(player.get_theta()*M_PI/180),
					player.get_size()/2,
					player.get_x()-100*sin(player.get_theta()*M_PI/180),
					player.get_y()+100*cos(player.get_theta()*M_PI/180),
					player.get_size()/2,
					0, 0, 1.0); 

	    gluLookAt(	player.get_x()+(player.get_size())*sin(player.get_theta()*M_PI/180),
			player.get_y()-(player.get_size())*cos(player.get_theta()*M_PI/180),
			player.get_size()/2,
			player.get_x()-100*sin(player.get_theta()*M_PI/180),
			player.get_y()+100*cos(player.get_theta()*M_PI/180),
			player.get_size()/2,
			0, 0, 1.0);


		


*/
	float x = player.get_x() + (cannon_x*cos(player.get_theta()*M_PI/180.0)-cannon_z*sin(player.get_theta()*M_PI/180.0))*player.get_size();
	float y = player.get_y() + (cannon_x*sin(player.get_theta()*M_PI/180.0)+cannon_z*cos(player.get_theta()*M_PI/180.0))*player.get_size();
	float z = player.get_size()*(12.0/30.0) + (cannon_y +0.02)*player.get_size();
//	printf("%f, %f, %f\n", x-(sin((player.get_theta()+player.get_cannon_theta())*M_PI/180)), y+(cos((player.get_theta()+player.get_cannon_theta())*M_PI/180)), z);

	gluLookAt(		  x,
				  y,	
				  z, 
				  x-(sin((player.get_theta()+player.get_cannon_theta())*M_PI/180)), 
				  y+(cos((player.get_theta()+player.get_cannon_theta())*M_PI/180)),
				  z,
				  0, 0, 1.0);
/*
		glPushMatrix();
		glDisable(GL_TEXTURE_2D);
		glPushMatrix();
			glColor3f(0,0.9,0);
			glTranslatef(x, y,z);
			glutSolidSphere(1.8,10,10);
			glColor3f(0,0.9,0);
			
			glColor3f(0,0.9,0.9);
			glRotatef(player.get_theta()+player.get_cannon_theta(),0,0,1);
			glTranslatef(0,10,0);
			glutSolidSphere(1,10,10);
		glPopMatrix();
		glPushMatrix();
			glColor3f(0.9,0,0);
			glTranslatef(x-(10*sin((player.get_theta()+player.get_cannon_theta())*M_PI/180)), 
				  y+(10*cos((player.get_theta()+player.get_cannon_theta())*M_PI/180)),
				  z);
			glutSolidSphere(1,10,10);
		glPopMatrix();
*/		
		glEnable(GL_TEXTURE_2D);
		
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

		draw_arena();
	}
	  glutSwapBuffers();
}

void init (void){
	GLfloat light_diffuse[] = {1.0,1.0, 1.0, 1.0};  
	GLfloat light_position[] = {lane[OUT].cx, lane[OUT].cy , player.get_size()*10, 1.00};

	glClearColor(1,1,1,0.0);
	glClearDepth(10.0f);                   // Set background depth to farthest
	glLoadIdentity();
	
	glEnable(GL_DEPTH_TEST);
//	glOrtho(lane[OUT].cx-lane[OUT].radius,lane[OUT].cx+lane[OUT].radius,lane[OUT].cy-lane[OUT].radius,lane[OUT].cy+lane[OUT].radius,-10.0,10.0);
	//glViewport(0,0,500,500);
	glMatrixMode(GL_PROJECTION);
	gluPerspective(60.0f, 1, 0.01, 3000);
	glMatrixMode(GL_MODELVIEW);
//	glLightfv(GL_LIGHT0, GL_AMBIENT, light_diffuse);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
   	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	
	//glLightfv(GL_LIGHT0, GL_SPECULAR, light_diffuse);
	//glMaterialf(GL_FRONT, GL_SHININESS, 255.0f);
   	glEnable(GL_LIGHT0);
//  	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_NORMALIZE);
	carTex = LoadTextureRAW("models/chassi/Tex_0020_1.png");
	gunTex = LoadTextureRAW("models/gun/Tex_0019_1.png");
	gndTex = LoadTextureRAW("models/ground.png");
	walTex = LoadTextureRAW("models/fence.png");
	strTex = LoadTextureRAW("models/start.png");
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
//			enemy_bullets.push_back(bullet);
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

void mouse(int button, int state, int x, int y){
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		Shot bullet = player.fire();
		bullet.set_max_distance (lane[OUT].radius*2);     	
		player_bullets.push_back(bullet); 
	}
	
	glutPostRedisplay();			
}

int main (int argc, char** argv){

	std::string path = "config.xml";

	if (argc > 1)
		path = std::string(argv[1])+"/"+path;
	
	glutInit(&argc,argv);

	parser(path.c_str());

	glutInitWindowSize(500,700);
	glutCreateWindow("TC2");
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB| GLUT_DEPTH);
	glutInitWindowPosition(0,0);
	init();
	glutDisplayFunc(display);
	glutKeyboardUpFunc(keyup);
	glutKeyboardFunc(keydown);
	glutMouseFunc(mouse);
	glutPassiveMotionFunc(mouseMove);
	glutIdleFunc(idle);

	srand (time(NULL));

	glutMainLoop();

	return 0;
}
