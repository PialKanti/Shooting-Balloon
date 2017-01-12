#include<bits/stdc++.h>
#include<stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>

#define PI 3.14159265359

using namespace std;


int num_balloon = 5;
float balloon_z[] = {2,4,6,8,10};
float balloon_y[] = {2.5,3.5,1.5,4.5,2.5};
float balloon_speed[] = {0.001,0.002,0.005,0.001,0.002};
float balloon_top_height = 20;
float balloon_bottom_start = -5.5;
float screen_right = 12;
float arrow_z = -10;
float arrow_y = -5.5;
int score = 0;
int miss = 10;
int high_score = 0;
int game_on = 0;
int first = 1;

bool is_arrow_up = true;

float camera_angle = 0;
float camera_x = -3;
float camera_y = 0;
float camera_height = 6;
float camera_rotate_speed = 0.003;
float camera_distance = 40;

float eye_x = 0;
float eye_y = 0;
float eye_z = 0;

float camera_x_angle = 0;
float camera_y_angle = 0;
float mouse_old_pos_x = 0;
float mouse_old_pos_y = 0;


GLuint texture[2];


GLfloat no_light[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat light_ambient[]  = {1.0, 1.0, 1.0, 1.0};
GLfloat light_diffuse[]  = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_position[] = { 2.0, 25.0, 3.0, 1.0 };

GLfloat balloon_no_mat[] = {0.0,0.0,0.0,1.0};
GLfloat balloon_ambient[] = {0.9, 0.2, 0.2, 1.0};
GLfloat balloon_diffuse[] = {0.9, 0.35, 0.183, 1.0};
GLfloat balloon_specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat balloon_shiness[] = {13};

GLfloat scene_no_mat[] = {0.0,0.0,0.0,1.0};
GLfloat scene_ambient[] = {1.0, 1.0, 1.0, 1.0};
GLfloat scene_diffuse[] = {0.4, 0.9, 0.5, 1.0};
GLfloat scene_specular[] = { 1.0, 1.0, 1.0, 1.0};
GLfloat scene_shiness[] = {13};


void lighting(GLfloat mat_ambient[4],GLfloat mat_diffuse[4],GLfloat mat_specular[4],GLfloat shiness[1])
{
    glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
    glMaterialfv(GL_FRONT,GL_SHININESS,shiness);
}


struct Image
{
    unsigned long sizeX;
    unsigned long sizeY;
    char *data;
};


int ImageLoad(char *filename, Image *image)
{
    FILE *file;
    unsigned long size;
    unsigned long i;
    unsigned short int planes;
    unsigned short int bpp;
    char temp;


    file = fopen(filename, "rb");
    if(file==NULL)
    {
        printf("Can't open file!");
    }


    fseek(file,18,SEEK_CUR);


    i = fread(&image->sizeX,4,1,file);

    i = fread(&image->sizeY,4,1,file);

    size = image->sizeX * image->sizeY * 3;

    fread(&planes, 2, 1, file);

    fseek(file, 24, SEEK_CUR);

    image->data = (char*)malloc(size);

    i=fread(image->data, size, 1, file);

    for(i=0; i<size; i+=3)
    {
        temp = image->data[i];
        image->data[i] = image->data[i+1];
        image->data[i+1] = temp;

    }

    fclose(file);

    return 1;

}

void loadTexture(char* file_name, int tex_id)
{
    Image *image1;

    image1 = (Image*) malloc(sizeof(Image));

    if(!ImageLoad(file_name,image1))
    {
        exit(1);
    }

    texture[tex_id] = tex_id;

    glBindTexture(GL_TEXTURE_2D,tex_id);

    glTexImage2D(GL_TEXTURE_2D,0,3,image1->sizeX,image1->sizeY,0,GL_RGB,GL_UNSIGNED_BYTE,image1->data);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);


    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}

void draw_solid_cube(GLfloat size)
{
    float x1,y1,z1;
    float x2,y2,z2;
    float x3,y3,z3;
    float x4,y4,z4;

    x1 = -size/2;
    y1 = -size/2;
    z1 = -size/2;

    x2 = size/2;
    y2 = -size/2;
    z2 = -size/2;

    x3 = size/2;
    y3 = -size/2;
    z3 = size/2;

    x4 = -size/2;
    y4 = -size/2;
    z4 = size/2;

    float x5,y5,z5;
    float x6,y6,z6;
    float x7,y7,z7;
    float x8,y8,z8;

    x5 = -size/2;
    y5 = size/2;
    z5 = -size/2;

    x6 = size/2;
    y6 = size/2;
    z6 = -size/2;

    x7 = size/2;
    y7 = size/2;
    z7 = size/2;

    x8 = -size/2;
    y8 = size/2;
    z8 = size/2;

    float n1_x = 0, n1_y = -1, n1_z = 0; //face that faces towards bottom
    float n2_x = 0, n2_y = 1, n2_z = 0; //up
    float n3_x = 0, n3_y = 0, n3_z = -1; //front
    float n4_x = 0, n4_y = 0, n4_z = 1; //back
    float n5_x = 1, n5_y = 0, n5_z = 0; //right
    float n6_x = -1, n6_y = 0, n6_z = 0; //left

    glBegin(GL_QUADS);
    //Quad 1

    glNormal3f(n1_x,n1_y,n1_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x4,y4,z4);

    //Quad 2

    glNormal3f(n2_x,n2_y,n2_z);

    glTexCoord2f(0,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);
    //Quad 3

    glNormal3f(n3_x,n3_y,n3_z);

    glTexCoord2f(0,0);
    glVertex3f(x5,y5,z5);

    glTexCoord2f(1,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,1);
    glVertex3f(x2,y2,z2);

    glTexCoord2f(0,1);
    glVertex3f(x1,y1,z1);
    //Quad 4


    glNormal3f(n4_x,n4_y,n4_z);

    glTexCoord2f(0,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,0);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(1,1);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(0,1);
    glVertex3f(x3,y3,z3);
    //Quad 5

    glNormal3f(n5_x,n5_y,n5_z);

    glTexCoord2f(0,0);
    glVertex3f(x6,y6,z6);

    glTexCoord2f(1,0);
    glVertex3f(x7,y7,z7);

    glTexCoord2f(1,1);
    glVertex3f(x3,y3,z3);

    glTexCoord2f(0,1);
    glVertex3f(x2,y2,z2);

    //Quad 6

    glNormal3f(n6_x,n6_y,n6_z);

    glTexCoord2f(0,0);
    glVertex3f(x1,y1,z1);

    glTexCoord2f(1,0);
    glVertex3f(x4,y4,z4);

    glTexCoord2f(1,1);
    glVertex3f(x8,y8,z8);

    glTexCoord2f(0,1);
    glVertex3f(x5,y5,z5);


    glEnd();
}

void draw_scene()
{
    lighting(scene_ambient,scene_diffuse,scene_specular,scene_shiness);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glPushMatrix();
    glTranslatef(25,10,0);
    glScalef(0.2,40,50);
    draw_solid_cube(1);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


int rand_int_range(int min_v, int max_v)
{
    return min_v + (rand() % (max_v - min_v + 1));
}

void showText(string str,float x,float y,float z)
{
    glColor3f(0,0,0);
    glDisable(GL_LIGHTING);
    glRasterPos3f(x,y,z);
    string s = str;
    void * font = GLUT_BITMAP_TIMES_ROMAN_24;

    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
    {
        char c = *i;
        glutBitmapCharacter(font, c);
    }
    glEnable(GL_LIGHTING);
}

void draw_arrow()
{
    if (is_arrow_up == true)
    {
        arrow_y = arrow_y + 0.04;
        if (arrow_y > balloon_top_height)
        {
            arrow_y = -5.5;
        }
    }
    else if (is_arrow_up == false)
    {
        arrow_z = arrow_z + 0.05;
        if (arrow_z > screen_right)
        {
            arrow_z = -10;
            arrow_y = -5.5;
            is_arrow_up = true;
        }

        ////////////////////////////////////
        for (int i = 0; i < num_balloon; i++)
        {
            float b_y = balloon_y[i];
            float b_z = balloon_z[i];

            float y1 = b_y + 1.5;
            float z1 = b_z - 0.8;

            float y2 = b_y - 1.5;
            float z2 = b_z + 0.8;

            float a_z = arrow_z+(3.5/2);
            float a_y = arrow_y;

            if (a_z >= z1 && a_z <= z2)
            {
                if (a_y >= y2 && a_y <= y1)
                {
                    balloon_y[i] = balloon_bottom_start;
                    balloon_speed[i] = ((float)rand_int_range(1,10) / 1000.0);
                    score++;
                }
            }
        }
    }

    lighting(balloon_ambient,balloon_diffuse,balloon_specular,balloon_shiness);

    glPushMatrix();

    glTranslatef(20,arrow_y,arrow_z+2);
    glScalef(0.2,0.2,3.0);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(20,arrow_y,arrow_z+3.3);
    glScalef(0.2,0.2,3.0);
    glutSolidCone(2,0.1,30,30);
    glPopMatrix();

    glPushMatrix();

    glTranslatef(20,arrow_y,arrow_z+0.38);
    glScalef(0.1,0.2,0.25);
    glutSolidCone(2,0.1,30,30);
    glPopMatrix();
}

void draw_balloon()
{
    for (int j = 0; j < num_balloon; j++)
    {

        balloon_y[j] = balloon_y[j] + balloon_speed[j];
        if (balloon_y[j] > balloon_top_height)
        {
            balloon_y[j] = balloon_bottom_start;
            balloon_speed[j] = ((float)rand_int_range(1,10) / 1000.0);
            miss--;
        }


        lighting(balloon_ambient,balloon_diffuse,balloon_specular,balloon_shiness);

        glPushMatrix();

        glTranslatef(20,balloon_y[j],balloon_z[j]);
        glScalef(0.8,1.5,0.8);
        glutSolidSphere(1.0, 50, 50);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(20,balloon_y[j] - 1.5,balloon_z[j]);
        glScalef(0.08,1.5,0.08);
        glutSolidCube(1);
        glPopMatrix();

    }
}

void handleKeyPressed(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 27:
        if(game_on == 0)
            game_on = 1;
        score = 0;
    case 'S':
    case 's':
        game_on = 1;
        first = 0;

    }
}

void initRendering()
{
    glEnable(GL_DEPTH_TEST);
}

void handleResize(int w, int h)
{
    glViewport(0,0,w,h);

}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,1,5,100);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(camera_x, camera_height, camera_y, camera_x + cos(camera_x_angle), camera_height, camera_y + sin(camera_y_angle), 0, 1, 0);

    //showing text
    draw_scene();
    if(miss <= 0)
    {
        game_on = 0;
        miss = 10;
    }
    if(score>high_score)
        high_score = score;
    if(first == 1)
    {
        showText("Controls: ",3.5,7,-1.5);
        showText("Press mouse left button to release arrow",3.5,6.5,-1.5);
        showText("Rules: ",3.5,6,-1.5);
        showText("If balloon go to upmost of screen 1 life reduces",3.5,5.6,-1.5);
        showText("Press S to start game",3.5,5,-1.5);
    }
    else
    {
        if(game_on == 1)
        {
            stringstream ss,ss1,ss2;
            ss << score;
            ss1<<miss;
            ss2<<high_score;
            string str = ss.str();
            string str1 = ss1.str();
            string str2 = ss2.str();
            showText("Player Score: "+str,4,9.7,-1);
            showText("Life Remaining: "+str1,4,9.3,-1);
            showText("High Score: "+str2,4,8.9,-1);
            draw_arrow();
            draw_balloon();
        }
        else if(game_on == 0)
        {
            stringstream ss,ss1;
            ss << score;
            ss1<<miss;
            string str = ss.str();
            string str1 = ss1.str();
            showText("Game Over",4,6,-1);
            showText("Total Score: "+str,4,5.4,-1);
            showText("Press Esc to play again",4,5.0,-1);
        }
    }


    //lighting in scene

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    glLightfv(GL_LIGHT0, GL_SPECULAR, no_light);

    glutSwapBuffers();

}

static void idle(void)
{
    glutPostRedisplay();
}

static void mouse_func(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON)
    {
        if (is_arrow_up == true)
        {
            is_arrow_up = false;
        }
    }
}


int main(int argc, char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(1000,800);
    glutInitWindowPosition(100,100);

    glutCreateWindow("Shooting Balloon");
    initRendering();

    glClearColor(0,0,0,1);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glGenTextures(2,texture);
    loadTexture("E:\\Shooting_Balloon\\background-wallpaper.bmp", 0);  //Give file full path unless program will crush


    glutDisplayFunc(display);
    glutKeyboardFunc(handleKeyPressed);
    glutReshapeFunc(handleResize);
    glutIdleFunc(idle);
    glutMouseFunc(mouse_func);
    glutMainLoop();
    return 0;
}
