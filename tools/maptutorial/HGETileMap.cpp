
#include "HGETileMap.h"
#include <vector>

HGETileMap::~HGETileMap()
{

}

void HGETileMap::Init()
{
	info.nTiles = 0;

	// null the tile array, delete existing tiles if necessary
	for (int i=0; i<MAX_TILES; i++)
		tiles[i] = NULL;

}

bool HGETileMap::Frame()
{
	return 1;
}	


void HGETileMap::Render()
{

	// work out the tile range
	//int pxWidth = (TILE_SIZE * vpWidth);
	int xtile = int(vpX / TILE_W); 
	int ytile = int(vpY / TILE_H);

	// calculate final pixel offset
	int px = vpX % TILE_W;
	int py = vpY % TILE_H;

	// Render tiles -  2 different render paths depending on if we're using a different texture size to the tile size
	if (TILE_H == TILETEX_H)
		for (int cy=ytile; cy<ytile+vpHeight; cy++)
			for (int cx=xtile; cx<xtile+vpWidth; cx++)
				if (tiles[cx+(cy*info.mapwidth)])
				{

					if (tiles[cx+(cy*info.mapwidth)]->isBackground())
						tiles[cx+(cy*info.mapwidth)]->background->Render(	drawX-px + (cx-xtile)*TILE_W, drawY - py - (TILETEX_H-TILE_H) + (cy-ytile)*TILE_H );

					if (tiles[cx+(cy*info.mapwidth)]->isGround())
						tiles[cx+(cy*info.mapwidth)]->ground->Render( drawX-px + (cx-xtile)*TILE_W,	drawY - py - (TILETEX_H-TILE_H) + (cy-ytile)*TILE_H );
						
				}



}
void HGETileMap::RenderFG()
{
	// work out the tile range
	//int pxWidth = (TILE_SIZE * vpWidth);
	int xtile = int(vpX / TILE_W); 
	int ytile = int(vpY / TILE_H);
	
	// calculate final pixel offset
	int px = vpX % TILE_W;
	int py = vpY % TILE_H;
	
	// Render foreground
	if (TILE_H == TILETEX_H)
		for (int cy=ytile; cy<ytile+vpHeight; cy++)
			for (int cx=xtile; cx<xtile+vpWidth; cx++)
				if (tiles[cx+(cy*info.mapwidth)])
					if (tiles[cx+(cy*info.mapwidth)]->isForeground())
						tiles[cx+(cy*info.mapwidth)]->foreground->Render(	drawX-px + (cx-xtile)*TILE_W, drawY - py - (TILETEX_H-TILE_H) + (cy-ytile)*TILE_H );
					
}

void HGETileMap::SetOffset(int x, int y)
{
	vpX = x;
	vpY = y;
}

void HGETileMap::SetTileOffset(int x, int y)
{
	vpX = x * TILE_W;
	vpY = y * TILE_H;
}

bool HGETileMap::PointCollide(float x, float y)
{
	if (x<0 || y<0) return false;

	// calculate which tile this is
	int tx = int(x / TILE_W);
	int ty = int(y / TILE_H);

	// tile index
	int idx = (tx)+((ty)*info.mapwidth);

	if (tiles[idx])
		return tiles[idx]->collide;

	return false;
}


// TODO: try and remove the checks for - coords, since it shouldn't happen!
// Returns an array of up to 9 potential collisions
bool HGETileMap::RectCollide(hgeRect r, CTile *plats[9],  int *nCols)
{
	int cPlat = 0;

	if (r.x1 < 0 || r.x2 < 0 || r.y1 < 0 || r.y2 < 0) return false;

	// calculate which tiles this affects
	int tx1 = int(r.x1 / TILE_W);		// start tile x
	int ty1 = int(r.y1 / TILE_H);		// start tile y
	int tx2 = int(r.x2 / TILE_W)+1;		// end tile x
	int ty2 = int(r.y2 / TILE_H)+1;		// end tile y

	// build a list of rects to test collision with
	std::vector<hgeRect> cols;
	std::vector<int> colidx;

	for (int iy=ty1; iy<ty2; iy++)
		for (int ix=tx1; ix<tx2; ix++)
			if (tiles[ix+(iy*info.mapwidth)])
				if (tiles[ix+(iy*info.mapwidth)]->collide || tiles[ix+(iy*info.mapwidth)]->collide2 || tiles[ix+(iy*info.mapwidth)]->collide3 || tiles[ix+(iy*info.mapwidth)]->collide4)
				{

					if (tiles[ix+(iy*info.mapwidth)]->collide3 || tiles[ix+(iy*info.mapwidth)]->collide4)
					{
						// if its an upwards slope

						hgeVector player_pos = hgeVector( r.x1 + ((r.x2 - r.x1)/2) , r.y2 );
						hgeRect tile_pos = hgeRect(ix*TILE_W, iy*TILE_H, (ix*TILE_W)+TILE_W, (iy*TILE_H)+TILE_H);

						if (tiles[ix+(iy*info.mapwidth)]->collide3)
						{
							if ( (player_pos.x - tile_pos.x1) > 40-(player_pos.y - tile_pos.y1) )
							{
								cols.push_back(hgeRect(ix*TILE_W, iy*TILE_H, (ix*TILE_W)+TILE_W, (iy*TILE_H)+TILE_H));
								colidx.push_back(ix+(iy*info.mapwidth));
							}
						}
						else
						{
							if ( (player_pos.x - tile_pos.x1) < (player_pos.y - tile_pos.y1) )
							{
								cols.push_back(hgeRect(ix*TILE_W, iy*TILE_H, (ix*TILE_W)+TILE_W, (iy*TILE_H)+TILE_H));
								colidx.push_back(ix+(iy*info.mapwidth));
							}
						}

					}
					else
					{
						cols.push_back(hgeRect(ix*TILE_W, iy*TILE_H, (ix*TILE_W)+TILE_W, (iy*TILE_H)+TILE_H));
						colidx.push_back(ix+(iy*info.mapwidth));
					}
				}


	
	// test for collisions
	for (int i=0; i<cols.size(); i++)
		if (cols[i].Intersect(&r))
		{
			plats[cPlat] = tiles[colidx[i]];
			cPlat ++;
		}

	*nCols = cPlat;

	// no collision was found	
	return false;
}

// Returns a tile at current offset
CTile *HGETileMap::GetOffsetTile(int x, int y)
{
	//calculate the tile offset
	int xtile = int(vpX / TILE_W); 
	int ytile = int(vpY / TILE_H);
	
	// tile index
	int idx = (xtile + x)+((ytile + y)*info.mapwidth);

	return tiles[idx];
}

// Returns a tile from tile coordinates
CTile *HGETileMap::GetTile(int x, int y)
{
	return tiles[x+(y*info.mapwidth)];
}

// Returns a tile at a game vector
CTile *HGETileMap::GameGetTile(hgeVector vec)
{
	//calculate the tile offset
	int xtile = int(vec.x / TILE_W); 
	int ytile = int(vec.y / TILE_H);
	
	// tile index
	int idx = xtile+(ytile*info.mapwidth);
	
	return tiles[idx];
}

void HGETileMap::GetTileOffset(int *x, int *y)
{
	*x = int(vpX / TILE_W); 
	*y = int(vpY / TILE_H);
}

// Returns map size in pixels
void HGETileMap::GetMapSize(int *w, int *h)
{
	*w = info.mapwidth * TILE_W;
	*h = info.mapheight * TILE_H;
}

MapTexture *HGETileMap::AddTexture(std::string texture, int xo, int yo)
{
	// cache the texture name and fetch a texture id
	hgeSprite *spr = NULL;
	HTEXTURE tex = 0;

	for (int i=0; i<textures.Max(); i++)
		if (textures.Fetch(i)->path == texture)
		{
			if (textures.Fetch(i)->xo == xo && textures.Fetch(i)->yo == yo)
			{
				return textures.Fetch(i);
			}
			else
			{
				tex = textures.Fetch(i)->spr->GetTexture();
				break;
			}
		}

	// create a sprite if not already cached / loaded
	if (!tex)	spr = new hgeSprite(hge->Texture_Load(texture.c_str()), xo, yo, TILETEX_W, TILETEX_H);
	else		spr = new hgeSprite(tex, xo, yo, TILETEX_W, TILETEX_H);
	// add a new texture object. ID starts at 1. 0 denotes a null texture
	textures.Add(new MapTexture(textures.Max()+1, texture, spr, xo, yo));
	return textures.Fetch(textures.Max()-1);
}


void HGETileMap::LoadMap(std::string filename)
{
	long i, j;
	
	FILE *fil = fopen(filename.c_str(), "rb");
	
	// ======================================
	// Read the LevelInfo header
	
	fread(&info, sizeof(LevelInfo), 1, fil);
	

	// Delete texture database TODO: call freetexture?
	textures.clear();

	// ======================================
	// Read the texture database
	
	// textures
	char tmp[LEVEL_FILENAME_LEN];
	int xo, yo;
	for (i=0; i<info.nTextures; i++)
	{
		fread(tmp, LEVEL_FILENAME_LEN, 1, fil);
		fread(&xo, sizeof(int), 1, fil);
		fread(&yo, sizeof(int), 1, fil);
		AddTexture(tmp, xo, yo);
	}
	
	
	// null the tile array, delete existing tiles if necessary
	for (i=0; i<MAX_TILES; i++)
		if (tiles[i])
		{
			// remove any entitys
			if (tiles[i]->entity)
				delete tiles[i]->entity;

			delete tiles[i];
			tiles[i] = NULL;
		}
		
	
	// ======================================
	// Read tiles
	
	for (i=0; i<info.nTiles-1; i++)
	{
		fread(&j, sizeof(int), 1, fil);
		
		tiles[j] = new CTile();
		
		tiles[j]->loc = hgeVector( (j % info.mapwidth) * TILE_W , ceil(j / info.mapwidth) * TILE_H );

		fread(&tiles[j]->bgTexID, sizeof(int), 1, fil);
		fread(&tiles[j]->gTexID, sizeof(int), 1, fil);
		fread(&tiles[j]->fgTexID, sizeof(int), 1, fil);
		
		if (tiles[j]->bgTexID > 0)
			tiles[j]->background = textures.Fetch(tiles[j]->bgTexID-1)->spr;
		
		if (tiles[j]->gTexID > 0)
			tiles[j]->ground = textures.Fetch(tiles[j]->gTexID-1)->spr;
		
		if (tiles[j]->fgTexID > 0)
			tiles[j]->foreground = textures.Fetch(tiles[j]->fgTexID-1)->spr;
		
		
		// Read entitys
		int ent_tmp = 0;
		fread(&ent_tmp, sizeof(int), 1, fil);
		if (ent_tmp)
		{
			char tmp2[ENT_NAME_LEN];
			
			// read entity name
			fread(&tmp2, ENT_NAME_LEN, 1, fil);
			
			// make entity
			tiles[j]->entity = new CEntity(tmp2);
			
			// read number of entity pairs
			unsigned int a;
			fread(&a, sizeof(a), 1, fil);
			
			// read each pair
			for (int k=0; k<a; k++)
			{
				entity_nv_pair nvpair;
				fread(nvpair.name, ENT_NAME_LEN, 1, fil);
				fread(nvpair.value, ENT_VALUE_LEN, 1, fil);
				
				tiles[j]->entity->AddNVProperty(nvpair.name, nvpair.value);
			}

			// Grab the x, y of this tile
			int tmp_x = (j % info.mapwidth) * TILE_W;
			int tmp_y = ceil(j / info.mapwidth) * TILE_H;


			// NOTE: this is where you can write code to make your game register entitys & do something with them.
			// eg:
			//
			// my_game_class->RegisterEntity(tiles[j]->entity, tmp_x, tmp_y);
			//
			// Post on the forum if you require more information..
			
		}
		
		
		// read collision data
		fread(&tiles[j]->collide, sizeof(bool), 1, fil);
		fread(&tiles[j]->collide2, sizeof(bool), 1, fil);
		fread(&tiles[j]->collide3, sizeof(bool), 1, fil);
		fread(&tiles[j]->collide4, sizeof(bool), 1, fil);
		fread(&tiles[j]->collide5, sizeof(bool), 1, fil);
		fread(&tiles[j]->collide6, sizeof(bool), 1, fil);	
	}
	
	
	
	
	fclose(fil);
}