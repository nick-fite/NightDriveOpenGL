
/*#include <GLFW/glfw3.h>
void render_loop()
{
    glClearColor ( .7, .1, .1, 1.0f );
    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glViewport(0,0,1024,768);
    glMatrixMode(GL_PROJECTION);
    //gluPerspective( 65.0, (double)1024/(double)768, 1.0, 60.0 );
    glOrtho(0,1024,768,0,100,-100);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glPointSize(10);
    glBegin(GL_POINTS);
    glColor4f(1,1,1,1);
    glVertex3f(512,384,0);
    glEnd();

    glfwSwapBuffers(window);

}
int main ( int argc, char* argv[] )
{
    //init glfw
    glfwInit();

    glfwOpenWindow ( 1024, 768, 8, 8, 8, 8, 24, 0, GLFW_WINDOW );

    do {
        render_loop();
    } while ( glfwGetWindowParam ( GLFW_OPENED ) );

    glfwTerminate();
}*/
#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <stdio.h>
#include <stdbool.h>
#include <wchar.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

#define res 4
#define PI  3.14159265359
#define SW 160 * res
#define SH 120 * res
#define SW2 (SW/2)
#define SH2 (SH/2)
#define pixelScale 4/res
#define GLSW (SW*pixelScale)
#define GLSH (SH*pixelScale)

typedef struct
{
   int fr1, fr2;
}time; time T;

typedef struct  
{
   int x, y, z;
   int a;
   int l;
}player; player P;

typedef struct {
   int x1, y1, x2, y2; //4 corners of the wall
   int c; //wall color
}walls; walls W[30];

typedef struct {
   int ws, we;
   int z1, z2;
   int d;
}sectors; sectors S[30];

typedef struct
{
   float cos[360];
   float sin[360];
} math; math M;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void Draw3D();
void DrawWall(int x1, int x2, int b1, int b2, int t1, int t2);


void Init() {
   for (int x = 0; x < 360; x++)
   {
      M.cos[x] = cos(x/180.0*PI);
      M.sin[x] = sin(x/180.0*PI);
   }
   P.x = 70;
   P.y = -110;
   P.z = 20;
   P.a = 0;
   P.l = 0;
}

void Pixel(int x, int y, bool getBlack)
{
   int color = getBlack ? 150 : 255;
   glColor3ub(color,color,color); //WE GOT 2 COLORS AND THATS IT!
   glBegin(GL_POINTS);
   glVertex2i(x * pixelScale + 2, y * pixelScale + 2);
   glEnd();
}

void ClearBackground()
{
   for(int x = 0; x < SW; x++)
   {
      for(int y = 0; y < SH; y++)
      {
         Pixel(x,y,true);
      }
   }
}

int dist(int x1, int y1, int x2, int y2)
{
   int distance = sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
   return distance;

}


int main() {
    if (!glfwInit()) {
        printf("GLFW initialization failed!\n");
        return -1;
    }

    
    GLFWwindow* window = glfwCreateWindow(SW, SH, "Night Drive", NULL, NULL);
    if (!window) {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }
   glfwMakeContextCurrent(window);

   if (glewInit() != GLEW_OK) {
        printf("GLEW initialization failed!\n");
        return -1;
    }

   Init();
   
   glfwMakeContextCurrent(window);
   //glLoadIdentity();
   //glMatrixMode(GL_PROJECTION);
   //glOrtho(0, SW, 0, SH, 0, 1);
   //glMatrixMode(GL_MODELVIEW);
   //glLoadIdentity();

   //GLfloat pointVertex[] = {SW/2.0f, SH/2.0f};

   glfwSetKeyCallback(window, key_callback);
   printf("asd");

   while(!glfwWindowShouldClose(window))
   {

      //glClear(GL_COLOR_BUFFER_BIT);
      glClearColor(1.00f, 0.00f, 0.0f, 1.0f);
      //glEnableClientState(GL_VERTEX_ARRAY);
      //glPointSize(.1);
      //glVertexPointer(2, GL_FLOAT, 0, pointVertex);
      
      //T.fr1 = glfwGetTime();

      //double elapsedTime = T.fr1 - T.fr2;
      
      //if(T.fr1 - T.fr2 >= 50)
      //{
         //Draw3D();
         

        // T.fr2 = T.fr1;
      //}
      glfwPollEvents();
      
      //int x,y;
      //glClear(GL_COLOR_BUFFER_BIT);

   }

   glfwDestroyWindow(window);
   glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //printf("%d, ", key);
   if(key == GLFW_KEY_A)
   {
      P.a -=4; if(P.a < 0){P.a+=360;}
      printf("a, ");
   }
   if(key == GLFW_KEY_D)
   {
      P.a +=4; if(P.a > 359){P.a-=360;}
      printf("d, ");
   }
   
   int dx = M.sin[P.a] * 10.0;
   int dy = M.cos[P.a] * 10.0;

   if(key == GLFW_KEY_W)
   {
      P.x+=dx;
      P.y+=dy;
      printf("w, ");
   }
   if(key == GLFW_KEY_S)
   {
      P.x-=dx;
      P.y-=dy;
      printf("s, ");
   }

   if(key == GLFW_KEY_LEFT)
   {
      P.z -=4;

   }
   if(key == GLFW_KEY_RIGHT)
   {
      P.z += 4;
   }
   if(key == GLFW_KEY_UP)
   {
      P.l += 1;

   }
   if(key == GLFW_KEY_DOWN)
   {
      P.l -= 1;

   }

}

void clipBehindPlayer(int *x1,int *y1,int *z1, int x2,int y2,int z2) //clip line
{
 float da=*y1;                                 //distance plane -> point a
 float db= y2;                                 //distance plane -> point b
 float d=da-db; if(d==0){ d=1;}
 float s = da/(da-db);                         //intersection factor (between 0 and 1)
 *x1 = *x1 + s*(x2-(*x1));
 *y1 = *y1 + s*(y2-(*y1)); if(*y1==0){ *y1=1;} //prevent divide by zero 
 *z1 = *z1 + s*(z2-(*z1));
}


int loadSectors[]=
{//wall start, wall end, z1 height, z2 height, bottom color, top color
 0,  4, 0, 40, 
 4,  8, 0, 40,
 8, 12, 0, 40,
 12,16, 0, 40,
};

int loadWalls[]=
{//x1,y1, x2,y2, color
  0, 0, 32, 0, 
 32, 0, 32,32,
 32,32,  0,32,
  0,32,  0, 0,

 64, 0, 96, 0,
 96, 0, 96,32,
 96,32, 64,32,
 64,32, 64, 0,

 64, 64, 96, 64,
 96, 64, 96, 96,
 96, 96, 64, 96,
 64, 96, 64, 64,

  0, 64, 32, 64,
 32, 64, 32, 96,
 32, 96,  0, 96,
  0, 96,  0, 64,
};

void Draw3D()
{


   int wx[4], wy[5], wz[4];
   float CS = M.cos[P.a], SN = M.sin[P.a];

   //offset bottom points by player
   int x1=40-P.x, y1= 10-P.y;
   int x2=40-P.x, y2=290-P.y;

   //world x pos 
   wx[0] = x1 * CS - y1 * SN;
   wx[1] = x2 * CS - y2 * SN;
   wx[2] = wx[0];
   wx[3] = wx[1];

   //world y pos 
   wy[0] = y1 * CS + x1 * SN;
   wy[1] = y2 * CS - x2 * SN;
   wy[2] = wy[0];
   wy[3] = wy[1];


   wz[0] = 0 - P.z + ((P.l*wy[0]) / 32.0);
   wz[1] = 0 - P.z + ((P.l*wy[1]) / 32.0);
   wz[2] = wz[0] + 40;
   wz[3] = wz[1] + 40;

   if(wy[0] < 1 && wy[1] < 1) return; 
   if(wy[0]<1)
    { 
     clipBehindPlayer(&wx[0],&wy[0],&wz[0], wx[1],wy[1],wz[1]); //bottom line
     clipBehindPlayer(&wx[2],&wy[2],&wz[2], wx[3],wy[3],wz[3]); //top line
    }
    //point 2 behind player, clip
    if(wy[1]<1)
    { 
     clipBehindPlayer(&wx[1],&wy[1],&wz[1], wx[0],wy[0],wz[0]); //bottom line
     clipBehindPlayer(&wx[3],&wy[3],&wz[3], wx[2],wy[2],wz[2]); //top line
    }

   wx[0] = wx[0] * 200/ wy[0] + SW2;
   wy[0] = wz[0] * 200/ wy[0] + SH2;

   wx[1] = wx[1] * 200/ wy[1] + SW2;
   wy[1] = wz[1] * 200/ wy[1] + SH2;

   wx[2] = wx[2] * 200/ wy[2] + SW2;
   wy[3] = wz[3] * 200/ wy[3] + SH2;

   //if(wx[0] > 0 && wx[0] < SW && wy[0] > 0 && wy[0] < SH) { Pixel(wx[0], wy[0], false);};
   //if(wx[1] > 0 && wx[1] < SW && wy[1] > 0 && wy[1] < SH) {Pixel(wx[1], wy[1], false);};
   DrawWall(wx[0], wx[1], wy[0], wy[1], wy[2], wy[3]);

}


void DrawWall(int x1, int x2, int b1, int b2, int t1, int t2)
{
   int x, y;
   int dyb = b2-b1;
   int dyt = t2-t1;
   int dx = x2-x1; if(dx == 0){dx = 1;}
   int xs=x1;

   if(x1 < 1) {x1 = 1;}
   if(x2 < 1) {x2 = 1;}
   if(x1 > SW - 1) {x1 = SW-1;}
   if(x2 > SW - 1) {x2 = SW-1;}

   for(int x = x1; x < x2; x++)
   {
      int y1 = dyb * (x-xs+0.5)/dx+b1;
      int y2 = dyt * (x-xs+0.5)/dx+t1;

      if(y1 < 1) {x1 = 1;}
      if(y2 < 1) {x2 = 1;}
      if(y1 > SH - 1) {y1 = SH-1;}
      if(y2 > SH - 1) {y2 = SH-1;}
      
      for(y=y1; y< y2; y++)
      {
         Pixel(x,y, true);
      }
   }
}

//this is code i pulled from some random website I can't find now.
//not going to be using it in the end, it has a lot of issues, but it does properly
//render to the screen which is a good start.
/*#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

GLFWwindow* window;

float playerX = 100, playerY = 100; // Initial player position
float playerAngle = 0;  // Angle of view (direction player is facing)

#define MAP_WIDTH 10
#define MAP_HEIGHT 10
int map[MAP_WIDTH][MAP_HEIGHT] = {
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 0, 1, 0, 1, 1},
    {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
    {1, 0, 1, 0, 1, 1, 1, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 0, 0, 0, 1},
    {1, 0, 1, 1, 1, 1, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

void initGLFW() {
    if (!glfwInit()) {
        fprintf(stderr, "GLFW initialization failed!\n");
    }

    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "DOOM-like Engine", NULL, NULL);
    if (!window) {
        glfwTerminate();
        fprintf(stderr, "Window creation failed!\n");
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable V-Sync
}

void setupOpenGL() {
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);  // Set a dark background color
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, -1, 1);  // 2D orthographic projection
    glMatrixMode(GL_MODELVIEW);
}

void processInput() {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        playerX += cos(playerAngle) * 2;  // Move forward
        playerY += sin(playerAngle) * 2;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        playerX -= cos(playerAngle) * 2;  // Move backward
        playerY -= sin(playerAngle) * 2;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        playerAngle -= 0.05f;  // Rotate left
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        playerAngle += 0.05f;  // Rotate right
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Cast rays to render the walls
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float rayAngle = playerAngle - 30.0f + (60.0f * (float)x / (float)SCREEN_WIDTH);  // Field of view (60 degrees)
        float distance = 0;

        float rayX = playerX;
        float rayY = playerY;

        // Raycasting loop
        while (map[(int)(rayX / 32)][(int)(rayY / 32)] == 0) {
            distance += 0.1f;  // Step size
            rayX += cos(rayAngle) * 0.1f;
            rayY += sin(rayAngle) * 0.1f;

            // Check bounds to avoid out-of-bounds access
            if (rayX < 0 || rayX >= MAP_WIDTH * 32 || rayY < 0 || rayY >= MAP_HEIGHT * 32) {
                break;  // Exit the loop if the ray goes out of bounds
            }
        }

        // Avoid division by zero
        int wallHeight = 0;
        if (distance > 0) {
            wallHeight = SCREEN_HEIGHT / distance;  // Calculate wall height
        }

        // Draw the wall (or "3D effect")
        glColor3f(1.0f, 0.0f, 0.0f);  // Set wall color (red for simplicity)
        glBegin(GL_LINES);
        glVertex2i(x, SCREEN_HEIGHT / 2 - wallHeight / 2);
        glVertex2i(x, SCREEN_HEIGHT / 2 + wallHeight / 2);
        glEnd();
    }
}

void gameLoop() {
    while (!glfwWindowShouldClose(window)) {
        processInput();  // Handle input
        render();        // Render the scene

        glfwSwapBuffers(window);  // Swap the buffer to display the rendered scene
        glfwPollEvents();         // Poll for input events
    }
}

int main() {
    initGLFW();
    setupOpenGL();

    gameLoop();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}*/
