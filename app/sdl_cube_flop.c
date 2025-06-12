#include <SDL2/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

#define PI32 3.14159265359f

typedef struct { float x,y; } Vec2;
typedef struct { float x,y,z; } Vec3;
typedef struct { float x,y,z,w; } Quat;
typedef struct { float m[4][4]; } Mat4;

static Vec3 vec3(float x,float y,float z){ Vec3 v={x,y,z}; return v; }
static Vec2 vec2(float x,float y){ Vec2 v={x,y}; return v; }
static Quat quat(float x,float y,float z,float w){ Quat q={x,y,z,w}; return q; }
static Mat4 mat4_identity(void){ Mat4 m={0}; m.m[0][0]=m.m[1][1]=m.m[2][2]=m.m[3][3]=1; return m; }
static Mat4 mat4_mul(Mat4 a, Mat4 b){ Mat4 r={0}; for(int i=0;i<4;i++) for(int j=0;j<4;j++) for(int k=0;k<4;k++) r.m[i][j]+=a.m[i][k]*b.m[k][j]; return r; }
static Mat4 mat4_translate(Mat4 m, Vec3 t){ Mat4 tr=mat4_identity(); tr.m[0][3]=t.x; tr.m[1][3]=t.y; tr.m[2][3]=t.z; return mat4_mul(m,tr); }
static Mat4 mat4_scale(Mat4 m, float s){ Mat4 sc=mat4_identity(); sc.m[0][0]=sc.m[1][1]=sc.m[2][2]=s; return mat4_mul(m,sc); }
static Mat4 mat4_from_quat_pos(Quat q, Vec3 pos){
    Mat4 m=mat4_identity();
    float x=q.x,y=q.y,z=q.z,w=q.w;
    m.m[0][0]=1-2*y*y-2*z*z; m.m[0][1]=2*x*y+2*w*z; m.m[0][2]=2*x*z-2*w*y; m.m[0][3]=pos.x;
    m.m[1][0]=2*x*y-2*w*z; m.m[1][1]=1-2*x*x-2*z*z; m.m[1][2]=2*y*z+2*w*x; m.m[1][3]=pos.y;
    m.m[2][0]=2*x*z+2*w*y; m.m[2][1]=2*y*z-2*w*x; m.m[2][2]=1-2*x*x-2*y*y; m.m[2][3]=pos.z;
    return m; }
static Mat4 mat4_rotate(Mat4 m, Vec3 axis, float angle){
    float c=cosf(angle), s=sinf(angle); float x=axis.x,y=axis.y,z=axis.z; float mc=1-c;
    Mat4 r=mat4_identity();
    r.m[0][0]=x*x*mc+c; r.m[0][1]=x*y*mc+z*s; r.m[0][2]=x*z*mc-y*s;
    r.m[1][0]=y*x*mc-z*s; r.m[1][1]=y*y*mc+c; r.m[1][2]=y*z*mc+x*s;
    r.m[2][0]=z*x*mc+y*s; r.m[2][1]=z*y*mc-x*s; r.m[2][2]=z*z*mc+c;
    return mat4_mul(m,r); }
static Mat4 mat4_rotate_x(Mat4 m,float a){ return mat4_rotate(m,vec3(1,0,0),a); }
static Mat4 mat4_rotate_y(Mat4 m,float a){ return mat4_rotate(m,vec3(0,1,0),a); }
static Quat quat_from_axis_angle(Vec3 axis,float angle){ float h=angle*0.5f; float s=sinf(h); return quat(axis.x*s,axis.y*s,axis.z*s,cosf(h)); }
static Quat quat_mul(Quat a, Quat b){
    Quat q; q.w=a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z; q.x=a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y; q.y=a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x; q.z=a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w; return q; }
static float clampf(float v,float lo,float hi){ return v<lo?lo:(v>hi?hi:v); }

#define CELL_SIZE 1.0f
#define BOARD_SIZE 5

typedef struct { int x,y; } GridPos;
typedef struct { GridPos grid; Quat orientation; } Cube;

static Cube cube={{2,2},{0,0,0,1}};
static GridPos hole_pos={4,1};
static bool rotating=false; static float rotate_t=0.0f; static Vec3 pivot_offset; static Vec3 rotate_axis; static GridPos target_pos;

static Mat4 make_cube_xform(void){
    float bh=BOARD_SIZE*CELL_SIZE*0.5f;
    Vec3 pos=vec3((cube.grid.x+0.5f)*CELL_SIZE-bh, CELL_SIZE*0.5f, (cube.grid.y+0.5f)*CELL_SIZE-bh);
    Mat4 m=mat4_from_quat_pos(cube.orientation,pos);
    if(rotating){ m=mat4_translate(m,pivot_offset); m=mat4_rotate(m,rotate_axis,rotate_t*PI32*0.5f); m=mat4_translate(m,vec3(-pivot_offset.x,-pivot_offset.y,-pivot_offset.z)); }
    return m; }

static void mult_matrix(const Mat4* m){ glMultMatrixf(&m->m[0][0]); }

static void draw_rect_xform(Mat4 transform, Vec2 size, float r,float g,float b){
    glPushMatrix(); mult_matrix(&transform); glColor3f(r,g,b); float w=size.x,h=size.y; glBegin(GL_QUADS); glVertex3f(0,0,0); glVertex3f(w,0,0); glVertex3f(w,h,0); glVertex3f(0,h,0); glEnd(); glPopMatrix(); }

static void draw_cube(Mat4 transform){
    Mat4 face; float s=CELL_SIZE; float gr=0.2f,gg=0.8f,gb=0.2f;
    face=transform; face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
    face=transform; face=mat4_rotate_y(face,PI32); face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
    face=transform; face=mat4_rotate_y(face,-PI32*0.5f); face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
    face=transform; face=mat4_rotate_y(face,PI32*0.5f); face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
    face=transform; face=mat4_rotate_x(face,-PI32*0.5f); face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
    face=transform; face=mat4_rotate_x(face,PI32*0.5f); face=mat4_translate(face,vec3(-s/2,-s/2,s/2)); draw_rect_xform(face,vec2(s,s),gr,gg,gb);
}

static Vec3 board_tile_world(int x,int y){ float bh=BOARD_SIZE*CELL_SIZE*0.5f; return vec3((x+0.5f)*CELL_SIZE-bh,0,(y+0.5f)*CELL_SIZE-bh); }

static void draw_board(void){
    for(int y=0;y<BOARD_SIZE;y++)
    for(int x=0;x<BOARD_SIZE;x++){
        if(x==hole_pos.x && y==hole_pos.y) continue;
        Mat4 t=mat4_identity(); t=mat4_rotate_x(t,-PI32*0.5f); t=mat4_translate(t,board_tile_world(x,y));
        draw_rect_xform(t,vec2(CELL_SIZE,CELL_SIZE),0.2f,0.2f,0.2f);
    }
}

int main(void){
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* win=SDL_CreateWindow("Cube Flop",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,800,600,SDL_WINDOW_OPENGL);
    SDL_GLContext ctx=SDL_GL_CreateContext(win);
    SDL_GL_SetSwapInterval(1);
    float cam_yaw=0.5f, cam_pitch=0.7f, cam_dist=8.0f; int lastx=0,lasty=0; bool dragging=false; Uint32 last=SDL_GetTicks();
    bool running=true; while(running){
        SDL_Event e; while(SDL_PollEvent(&e)){
            if(e.type==SDL_QUIT) running=false;
            else if(e.type==SDL_MOUSEBUTTONDOWN && e.button.button==SDL_BUTTON_LEFT) dragging=true;
            else if(e.type==SDL_MOUSEBUTTONUP && e.button.button==SDL_BUTTON_LEFT) dragging=false;
            else if(e.type==SDL_KEYDOWN && !rotating){
                if(e.key.keysym.sym==SDLK_a){ target_pos=(GridPos){cube.grid.x-1,cube.grid.y}; rotate_axis=vec3(0,0,1); pivot_offset=vec3(-CELL_SIZE/2,-CELL_SIZE/2,0); rotating=true; }
                else if(e.key.keysym.sym==SDLK_d){ target_pos=(GridPos){cube.grid.x+1,cube.grid.y}; rotate_axis=vec3(0,0,-1); pivot_offset=vec3(CELL_SIZE/2,-CELL_SIZE/2,0); rotating=true; }
                else if(e.key.keysym.sym==SDLK_w){ target_pos=(GridPos){cube.grid.x,cube.grid.y-1}; rotate_axis=vec3(-1,0,0); pivot_offset=vec3(0,-CELL_SIZE/2,-CELL_SIZE/2); rotating=true; }
                else if(e.key.keysym.sym==SDLK_s){ target_pos=(GridPos){cube.grid.x,cube.grid.y+1}; rotate_axis=vec3(1,0,0); pivot_offset=vec3(0,-CELL_SIZE/2,CELL_SIZE/2); rotating=true; }
            }
        }
        int mx,my; SDL_GetMouseState(&mx,&my); int dx=mx-lastx, dy=my-lasty; lastx=mx; lasty=my; if(dragging){ cam_yaw+=dx*0.005f; cam_pitch+=dy*0.005f; cam_pitch=clampf(cam_pitch,0.2f,1.2f); }
        Uint32 now=SDL_GetTicks(); float dt=(now-last)/1000.0f; last=now; if(rotating){ rotate_t+=dt*3.0f; if(rotate_t>=1.0f){ cube.grid=target_pos; cube.orientation=quat_mul(cube.orientation,quat_from_axis_angle(rotate_axis,PI32*0.5f)); rotating=false; rotate_t=0.0f; }}
        glViewport(0,0,800,600); glClearColor(0.05f,0.05f,0.05f,1); glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); glEnable(GL_DEPTH_TEST);
        glMatrixMode(GL_PROJECTION); glLoadIdentity(); gluPerspective(45.0,800.0/600.0,0.1,100.0);
        glMatrixMode(GL_MODELVIEW); glLoadIdentity(); float cx=sinf(cam_yaw)*cosf(cam_pitch)*cam_dist; float cy=sinf(cam_pitch)*cam_dist; float cz=cosf(cam_yaw)*cosf(cam_pitch)*cam_dist; gluLookAt(cx,cy,cz,0,0,0,0,1,0);
        draw_board(); Mat4 cxf=make_cube_xform(); draw_cube(cxf); if(!rotating && cube.grid.x==hole_pos.x && cube.grid.y==hole_pos.y){ printf("You Win!\n"); }
        SDL_GL_SwapWindow(win);
    }
    SDL_GL_DeleteContext(ctx); SDL_DestroyWindow(win); SDL_Quit(); return 0;
}
