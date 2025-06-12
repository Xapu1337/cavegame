#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif
#include <math.h>
#include <stdbool.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define CELL_SIZE 1.0f
#define BOARD_SIZE 5

typedef struct {int x; int y;} GridPos;

static GridPos cube_pos = {2,2};
static GridPos hole_pos = {4,1};

static bool rotating = false;
static float rotate_t = 0.f;
static float axis_x, axis_y, axis_z;
static float pivot_x, pivot_y, pivot_z;
static GridPos target_pos;

static void draw_cube(void) {
    float s=CELL_SIZE/2.f;
    glBegin(GL_QUADS);
    glColor3f(0.f,1.f,0.f);
    // front
    glVertex3f(-s,-s, s); glVertex3f( s,-s, s); glVertex3f( s, s, s); glVertex3f(-s, s, s);
    // back
    glVertex3f(-s,-s,-s); glVertex3f(-s, s,-s); glVertex3f( s, s,-s); glVertex3f( s,-s,-s);
    // left
    glVertex3f(-s,-s,-s); glVertex3f(-s,-s, s); glVertex3f(-s, s, s); glVertex3f(-s, s,-s);
    // right
    glVertex3f( s,-s,-s); glVertex3f( s, s,-s); glVertex3f( s, s, s); glVertex3f( s,-s, s);
    // top
    glVertex3f(-s, s,-s); glVertex3f(-s, s, s); glVertex3f( s, s, s); glVertex3f( s, s,-s);
    // bottom
    glVertex3f(-s,-s,-s); glVertex3f( s,-s,-s); glVertex3f( s,-s, s); glVertex3f(-s,-s, s);
    glEnd();
}

static void draw_board(void) {
    glColor3f(0.2f,0.2f,0.2f);
    for(int y=0;y<BOARD_SIZE;y++)
    for(int x=0;x<BOARD_SIZE;x++) {
        if(x==hole_pos.x && y==hole_pos.y) continue;
        float bx = (x+0.5f-BOARD_SIZE/2.f)*CELL_SIZE;
        float by = (y+0.5f-BOARD_SIZE/2.f)*CELL_SIZE;
        glPushMatrix();
        glTranslatef(bx, -CELL_SIZE/2.f, by);
        glBegin(GL_QUADS);
        glVertex3f(-CELL_SIZE/2,0,-CELL_SIZE/2);
        glVertex3f( CELL_SIZE/2,0,-CELL_SIZE/2);
        glVertex3f( CELL_SIZE/2,0, CELL_SIZE/2);
        glVertex3f(-CELL_SIZE/2,0, CELL_SIZE/2);
        glEnd();
        glPopMatrix();
    }
}

static void set_camera(float yaw,float pitch,float dist) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, 16.0/9.0, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    float cx = sinf(yaw)*cosf(pitch)*dist;
    float cy = sinf(pitch)*dist;
    float cz = cosf(yaw)*cosf(pitch)*dist;
    gluLookAt(cx,cy,cz,0,0,0,0,1,0);
}

int main(int argc,char**argv){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window*win=SDL_CreateWindow("Cube Flop",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_OPENGL);
    SDL_GLContext ctx=SDL_GL_CreateContext(win);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    glEnable(GL_DEPTH_TEST);
    float cam_yaw=0.5f,cam_pitch=0.7f,cam_dist=8.f;
    int last_mx=0,last_my=0; Uint32 last_ticks=SDL_GetTicks();
    bool running=true; while(running){
        SDL_Event e; while(SDL_PollEvent(&e)){ if(e.type==SDL_QUIT) running=false; }
        int mx,my; Uint32 buttons=SDL_GetRelativeMouseState(&mx,&my);
        if(buttons&SDL_BUTTON(SDL_BUTTON_LEFT)){ cam_yaw+=mx*0.005f; cam_pitch+=my*0.005f; if(cam_pitch<0.2f)cam_pitch=0.2f; if(cam_pitch>1.2f)cam_pitch=1.2f; }
        const Uint8*k=SDL_GetKeyboardState(NULL);
        if(!rotating){
            if(k[SDL_SCANCODE_A]){ target_pos=(GridPos){cube_pos.x-1,cube_pos.y}; axis_x=0;axis_y=0;axis_z=1; pivot_x=-CELL_SIZE/2; pivot_y=-CELL_SIZE/2; pivot_z=0; rotating=true; }
            else if(k[SDL_SCANCODE_D]){ target_pos=(GridPos){cube_pos.x+1,cube_pos.y}; axis_x=0;axis_y=0;axis_z=-1; pivot_x=CELL_SIZE/2; pivot_y=-CELL_SIZE/2; pivot_z=0; rotating=true; }
            else if(k[SDL_SCANCODE_W]){ target_pos=(GridPos){cube_pos.x,cube_pos.y-1}; axis_x=-1;axis_y=0;axis_z=0; pivot_x=0; pivot_y=-CELL_SIZE/2; pivot_z=-CELL_SIZE/2; rotating=true; }
            else if(k[SDL_SCANCODE_S]){ target_pos=(GridPos){cube_pos.x,cube_pos.y+1}; axis_x=1;axis_y=0;axis_z=0; pivot_x=0; pivot_y=-CELL_SIZE/2; pivot_z=CELL_SIZE/2; rotating=true; }
        }
        Uint32 now=SDL_GetTicks(); float dt=(now-last_ticks)/1000.f; last_ticks=now;
        if(rotating){ rotate_t+=dt*3.f; if(rotate_t>=1.f){ cube_pos=target_pos; rotating=false; rotate_t=0.f; } }
        glClearColor(0.05f,0.05f,0.05f,1.f); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        set_camera(cam_yaw,cam_pitch,cam_dist);
        draw_board();
        glPushMatrix();
        float bx=(cube_pos.x+0.5f-BOARD_SIZE/2.f)*CELL_SIZE;
        float by=(cube_pos.y+0.5f-BOARD_SIZE/2.f)*CELL_SIZE;
        glTranslatef(bx,0,by);
        if(rotating){ glTranslatef(pivot_x,pivot_y,pivot_z); glRotatef(rotate_t*90.f,axis_x,axis_y,axis_z); glTranslatef(-pivot_x,-pivot_y,-pivot_z); }
        draw_cube();
        glPopMatrix();
        SDL_GL_SwapWindow(win);
    }
    SDL_GL_DeleteContext(ctx); SDL_DestroyWindow(win); SDL_Quit();
    return 0;
}
