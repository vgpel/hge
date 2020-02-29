
#ifndef HGETileMap_H
#define HGETileMap_H

#include <string>

#include <hge.h>
#include <hgesprite.h>
#include <hgevector.h>

#include "HGETileMapDatabases.h"


// ===================================================================
// Game-Specific defines - change these to match your tile size!


#define TILE_W 40
#define TILE_H 40

// Adjust these if you want to use different sized textures to the tile size
#define TILETEX_W TILE_W
#define TILETEX_H TILE_H


// ===================================================================
// Map File Struct

#define LEVEL_FILENAME_LEN 128

struct LevelInfo {
	int nTextures;
	int nTiles;
	int nEntitys;
	
	char filename[LEVEL_FILENAME_LEN];
	
	int tilew;	// in pixels
	int tileh;	// in pixels
	
	int texw;	// texture in pixels - usually will be the same as tile size.
	int texh;	// texture height in pixels
	
	int mapwidth;	// in tiles
	int mapheight;	// in tiles
};

// ===================================================================
// Map Texture Class

struct MapTexture
{
	unsigned int id;
	std::string path;
	hgeSprite *spr;

	int xo, yo;	// offsets

	MapTexture() { id = 0; path= ""; spr = NULL; }
	MapTexture(int i, std::string p, hgeSprite *s, int offsetx, int offsety) { id = i; path= p; spr = s; xo=offsetx; yo=offsety; }
};



// ===================================================================
// Map Entity Class

#define ENT_NAME_LEN 32
#define ENT_VALUE_LEN 128

struct entity_nv_pair
{
	char name[ENT_NAME_LEN];
	char value[ENT_VALUE_LEN];

	entity_nv_pair() { return; }
	entity_nv_pair(std::string nam, std::string val) { strcpy(name, nam.c_str()); strcpy(value, val.c_str()); }

	// Copy constructor
	entity_nv_pair(const entity_nv_pair &b)
	{
		strncpy(name, b.name, ENT_NAME_LEN);
		strncpy(value, b.value, ENT_VALUE_LEN);
    }
};

class CEntity
{
public:
	char name[ENT_NAME_LEN];

	// retrieves a value from an existing property
	std::string GetValue(std::string name)
	{
		for (int i=0; i<nvals.Max(); i++)
			if (std::string(nvals.Fetch(i).name) == name)
				return std::string(nvals.Fetch(i).value);
		
		return "";
	}

	// adds value to an existing property
	bool AddValue(std::string name, std::string value)
	{
		for (int i=0; i<nvals.Max(); i++)
			if (!strcmp(nvals.Fetch(i).name, name.c_str()))
			{
				sprintf(nvals.FetchPointer(i)->value, "%s", value.c_str());
				return true;
			}

		return false;	
	}

	// creates a property
	void AddNVProperty(std::string name, std::string value)
	{
		nvals.Add(entity_nv_pair(name, value));
	}

	// returns the number of nval pairs
	unsigned int GetNVCount() { return nvals.Max(); }	

	// returns a nval pair
	entity_nv_pair GetNVProperty(int index) { return nvals.Fetch(index); }

	// Copies from another entity to this one
	void Copy(CEntity *ent)
	{
		strcpy(name, ent->name);

		for (int i=0; i<ent->GetNVCount(); i++)
		{
			entity_nv_pair tmp = ent->GetNVProperty(i);
			AddNVProperty(tmp.name, tmp.value);
		}
	}

	CEntity(std::string nam) { strcpy(name, nam.c_str()); }

private:
	
	// TODO: code these!!
	//entity_nv_pair nvpairs[10];
	//int nPairs;

	CSDatabase<entity_nv_pair> nvals;
};


// ===================================================================
// Map Tile Class

class CTile
{
public:
	
	// for rendering
	hgeSprite *background;
	hgeSprite *ground;
	hgeSprite *foreground;
	CEntity *entity;

	bool collide;
	bool collide2;
	bool collide3;
	bool collide4;
	bool collide5;
	bool collide6;

	// for the editor only:
	unsigned int bgTexID;
	unsigned int gTexID;
	unsigned int fgTexID;

	// for the game only! set at load
	hgeVector loc;

	
	CTile()
	{
		background = NULL;
		ground = NULL;
		foreground = NULL;
		entity = NULL;
		bgTexID = 0;
		gTexID = 0;
		fgTexID = 0;
		collide = false;
		collide2 = false;
		collide3 = false;
		collide4 = false;
		collide5 = false;
		collide6 = false;
	}
	
	inline bool isBackground() { if (background) return true; else return false; }
	inline bool isGround() { if (ground) return true; else return false; }
	inline bool isForeground() { if (foreground) return true; else return false; }

	
};


// ===================================================================
// Map Class. Creates, Stores, Saves, Loads & Renders maps

#define MAX_TILES 512000

class HGETileMap
{
private:

	HGE *hge;

	CTile *tiles[MAX_TILES];


	// for the editor - texture name database
	CPDatabase<MapTexture> textures;


	int drawX, drawY; // x y offset of the map itself. internal

	// camera variables
	int vpWidth, vpHeight; // viewport width in tiles
	int vpX, vpY; // current viewport in pixels

	// Caches textures in the internal texture database. Returns texture ID
	MapTexture *AddTexture(std::string texture, int xo, int yo);


public:

	// level info header from file
	LevelInfo info;


	// Takes: Game pointer, X + Y offset to draw at, Level width and Height in tiles
	HGETileMap(HGE *hgep, int x, int y, int levelwidth, int levelheight)
	{
		hge = hgep;
		drawX = x;
		drawY = y;

		vpX = 0;
		vpY = 0;
		vpWidth = levelwidth + 1;
		vpHeight = levelheight + 1;
		
		return;
	}
	~HGETileMap();


	void LoadMap(std::string);


	CTile *GetOffsetTile(int, int);
	CTile *GetTile(int, int);

	// Gets tile from a game vector
	CTile *GameGetTile(hgeVector);

	// Sets camera offset in pixels
	void SetOffset(int, int);

	// Sets camera offset in tiles
	void SetTileOffset(int, int);

	// Returns the viewport offset in tiles
	void GetTileOffset(int *, int *);

	// Returns the map size in pixels
	void GetMapSize(int *, int *);

	// returns true if a point collides with a tile
	bool PointCollide(float, float);

    bool RectCollide(hgeRect r, CTile *plats[9],  int *nCols);


	void Init();
	bool Frame();
	void Render();
	void RenderFG();

};

#endif