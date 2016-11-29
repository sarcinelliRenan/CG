#include "std_draw.h"
#include "lodepng.h"

void draw_circ(float radius,float r,float g,float b,float points){
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
		for (int i=0; i < 360; i+=360/points)
		{
			float degInRad = i*DEG2RAD;
			glVertex3f(cos(degInRad)*radius,sin(degInRad)*radius,0);
		}
	glEnd(); 
}

void draw_rect(float height, float width,float r,float g,float b){
	glColor3f(r,g,b);
	glBegin(GL_POLYGON);
		glVertex3f(height/2		,width/2	,0.0);
		glVertex3f(height/2		,-width/2	,0.0);
		glVertex3f(-height/2	,-width/2	,0.0);
		glVertex3f(-height/2	,width/2	,0.0);
	glEnd();
}

void draw_circle (Circle circ)
{ 
	glColor3f(circ.r,circ.g,circ.b);
	glBegin(GL_POLYGON);
		for (int i=0; i < 360; i+=4)
		{
			float degInRad = i*DEG2RAD;
			glVertex3f(cos(degInRad)*circ.radius+circ.cx,sin(degInRad)*circ.radius+circ.cy,0);
		}
	glEnd(); 
}

void draw_rectangle (Rectangle rect)
{ 
	glColor3f(rect.r,rect.g,rect.b);
	glBegin(GL_POLYGON);
		glVertex3f(rect.x			,rect.y			,0.0);
		glVertex3f(rect.x+rect.w	,rect.y			,0.0);
		glVertex3f(rect.x+rect.w	,rect.y+rect.h	,0.0);
		glVertex3f(rect.x			,rect.y+rect.h	,0.0);
	glEnd();
}

GLuint LoadTextureRAW( const char * filename )
{

    GLuint texture;
    
    unsigned error;
  	unsigned char* image;
  	unsigned width, height;

  	error = lodepng_decode32_file(&image, &width, &height, filename);
  	if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
	
	glEnable(GL_TEXTURE_2D);
    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR );
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                             0,                            //0 for now
                             GL_RGB,                       //Format OpenGL uses for image
                             width, height,  //Width and height
                             0,                            //The border of the image
                             GL_RGBA, //GL_RGB, because pixels are stored in RGB format
                             GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                                               //as unsigned numbers
                             image);               //The actual pixel data

    return texture;
}
