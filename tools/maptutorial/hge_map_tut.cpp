/*
** Haaf's Game Engine 1.8
** Copyright (C) 2003-2007, Relish Games
** hge.relishgames.com
**
** hge_map_tut - Using Merciors HGETileMap class
** 
** Please, feel free to use and modify any of the included source code as you see fit
*/



#include <hge.h>
#include "HGETileMap.h"

HGE *hge=0;

// Create a pointer to the tilemap object
HGETileMap *tilemap;

// Quad is the basic primitive in HGE
// used for rendering graphics.
// Quad contains 4 vertices, numbered
// 0 to 3 clockwise.
hgeQuad quad;

// Some "gameplay" variables and constants
float x=100.0f, y=100.0f;
float dx=0.0f, dy=0.0f;

// mouse position
float mx, my;

const float speed=20;
const float friction=0.98f;



bool FrameFunc()
{
	// Get the time elapsed since last call of FrameFunc().
	// This will help us to synchronize on different
	// machines and video modes.
	float dt=hge->Timer_GetDelta();

	// Process keys
	if (hge->Input_GetKeyState(HGEK_ESCAPE)) return true;
	if (hge->Input_GetKeyState(HGEK_LEFT)) dx-=speed*dt;
	if (hge->Input_GetKeyState(HGEK_RIGHT)) dx+=speed*dt;
	if (hge->Input_GetKeyState(HGEK_UP)) dy-=speed*dt;
	if (hge->Input_GetKeyState(HGEK_DOWN)) dy+=speed*dt;

	dx*=friction; dy*=friction; x+=dx; y+=dy;

	// limit position so we don't go off the map
	if(x+dx<=0) {x=0;dx=0;}
	if(y+dy<=0) {y=0;dy=0;}


	// Set the tile map render offset
	tilemap->SetOffset((int)x, (int)y);


	// Grab mouse position
	hge->Input_GetMousePos(&mx, &my);

	// See if mouse collides with map collision layer
	if (tilemap->PointCollide(mx + x, my + y))
		quad.blend=BLEND_ALPHAADD | BLEND_COLORADD | BLEND_ZWRITE;
	else
		quad.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;


	// Set up quad's screen coordinates
	quad.v[0].x=mx-16; quad.v[0].y=my-16;
	quad.v[1].x=mx+16; quad.v[1].y=my-16;
	quad.v[2].x=mx+16; quad.v[2].y=my+16;
	quad.v[3].x=mx-16; quad.v[3].y=my+16;	


	// Continue execution
	return false;
}

// This function will be called by HGE when
// the application window should be redrawn.
// Put your rendering code here.
bool RenderFunc()
{
	// Begin rendering quads.
	// This function must be called
	// before any actual rendering.
	hge->Gfx_BeginScene();

	// Clear screen with black color
	hge->Gfx_Clear(0);


	// Render the tile map here
	tilemap->Render();

	// Render quads here. This time just
	// one of them will serve our needs.
	hge->Gfx_RenderQuad(&quad);

	// Render the tile maps foreground after the player
	tilemap->RenderFG();

	// End rendering and update the screen
	hge->Gfx_EndScene();

	// RenderFunc should always return false
	return false;
}


int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Get HGE interface
	hge = hgeCreate(HGE_VERSION);

	// Set up log file, frame function, render function and window title
	hge->System_SetState(HGE_LOGFILE, "hge_map_tut.log");
	hge->System_SetState(HGE_FRAMEFUNC, FrameFunc);
	hge->System_SetState(HGE_RENDERFUNC, RenderFunc);
	hge->System_SetState(HGE_TITLE, "HGE Map Tutorial");

	// Set up video mode
	hge->System_SetState(HGE_WINDOWED, true);
	hge->System_SetState(HGE_SCREENWIDTH, 800);
	hge->System_SetState(HGE_SCREENHEIGHT, 600);
	hge->System_SetState(HGE_SCREENBPP, 32);

	if(hge->System_Initiate())
	{
		// Load sound and texture

		quad.tex=hge->Texture_Load("gfx/particles.png");
		if( !quad.tex)
		{
			// If one of the data files is not found, display
			// an error message and shutdown.
			MessageBox(NULL, "Can't load PARTICLES.PNG", "Error", MB_OK | MB_ICONERROR | MB_APPLMODAL);
			hge->System_Shutdown();
			hge->Release();
			return 0;
		}

		// Set up quad which we will use for rendering sprite
		quad.blend=BLEND_ALPHAADD | BLEND_COLORMUL | BLEND_ZWRITE;

		for(int i=0;i<4;i++)
		{
			// Set up z-coordinate of vertices
			quad.v[i].z=0.5f;
			// Set up color. The format of DWORD col is 0xAARRGGBB
			quad.v[i].col=0xFFFFA000;
		}

		// Set up quad's texture coordinates.
		// 0,0 means top left corner and 1,1 -
		// bottom right corner of the texture.
		quad.v[0].tx=96.0/128.0; quad.v[0].ty=64.0/128.0; 
		quad.v[1].tx=128.0/128.0; quad.v[1].ty=64.0/128.0; 
		quad.v[2].tx=128.0/128.0; quad.v[2].ty=96.0/128.0; 
		quad.v[3].tx=96.0/128.0; quad.v[3].ty=96.0/128.0;


		x=0;y=0;

		// Load map using the HGETileMap class

		// Create new instance of tilemap.
		// First coordinates are x,y in pixels to render at, Second are render size IN TILES
		// 800x600 resolution, 40 pixel tiles. 800/40 = 20, 600/40 = 15

		tilemap = new HGETileMap(hge, 0, 0, 20, 15);

		// Initialize map
		tilemap->Init();
		
		// Load Map file
		tilemap->LoadMap("test.map");


		// Let's rock now!
		hge->System_Start();

		// Free loaded texture and sound
		hge->Texture_Free(quad.tex);
	}
	else MessageBox(NULL, hge->System_GetErrorMessage(), "Error", MB_OK | MB_ICONERROR | MB_SYSTEMMODAL);

	// Clean up and shutdown
	hge->System_Shutdown();
	hge->Release();
	return 0;
}
