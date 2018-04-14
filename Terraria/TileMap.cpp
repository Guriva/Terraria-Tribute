#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <random>
#include <ctime>
#include "TileMap.h"


using namespace std;


TileMap *TileMap::createTileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram *program)
{
	TileMap *map = new TileMap(levelFile, minCoords, program);

	return map;
}


TileMap::TileMap(const string &levelFile, const glm::vec2 &minCoords, ShaderProgram *program)
{
	loadLevel(levelFile);
	prog = program;
	prepareArrays(minCoords);
}

TileMap::~TileMap()
{
	if (map != NULL)
		delete map;
}


void TileMap::render() const
{
	glEnable(GL_TEXTURE_2D);
	tilesheet.use();
	glBindVertexArray(vao);
	glEnableVertexAttribArray(posLocation);
	glEnableVertexAttribArray(texCoordLocation);
	glDrawArrays(GL_TRIANGLES, 0, 24 * mapSize.x * mapSize.y);
	glDisable(GL_TEXTURE_2D);
}

void TileMap::update(glm::vec2 posTile, string type)
{
	int tile;
	glm::vec2 texCoordTile[2];

	tile = 0;
	int posx = int(posTile.x);
	int posy = int(posTile.y);
	int pos = posy * mapSize.x * 24 + posx * 24;
	if (type == "Dirt") {
		tile = rand()%27 + 1;
	}
	else if (type == "Iron Ore") {
		tile = rand() % 18 + 82;
	}
	else if (type == "Gold Ore") {
		tile = rand() % 18 + 64;
	}
	else if (type == "Stone") {
		tile = rand() % 18 + 28;
	}
	else if (type == "Wood") {
		tile = rand() % 9 + 46;
	}

	map[posy * mapSize.x + posx] = tile;
	if (tile != 0)
	{
		// Non-empty tile
		texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
		texCoordTile[1] = texCoordTile[0] + tileTexSize;
		// First triangle
		vertices[pos] = (posTile.x*16); vertices[pos + 1] = (posTile.y*16);
		vertices[pos + 2] = (texCoordTile[0].x); vertices[pos + 3] = (texCoordTile[0].y);
		vertices[pos + 4] = (posTile.x*16 + blockSize); vertices[pos + 5] = (posTile.y*16);
		vertices[pos + 6] = (texCoordTile[1].x); vertices[pos + 7] = (texCoordTile[0].y);
		vertices[pos + 8] = (posTile.x*16 + blockSize); vertices[pos + 9] = (posTile.y*16 + blockSize);
		vertices[pos + 10] = (texCoordTile[1].x); vertices[pos + 11] = (texCoordTile[1].y);
		// Second triangle
		vertices[pos + 12] = (posTile.x*16); vertices[pos + 13] = (posTile.y*16);
		vertices[pos + 14] = (texCoordTile[0].x); vertices[pos + 15] = (texCoordTile[0].y);
		vertices[pos + 16] = (posTile.x*16 + blockSize); vertices[pos + 17] = (posTile.y*16 + blockSize);
		vertices[pos + 18] = (texCoordTile[1].x); vertices[pos + 19] = (texCoordTile[1].y);
		vertices[pos + 20] = (posTile.x*16); vertices[pos + 21] = (posTile.y*16 + blockSize);
		vertices[pos + 22] = (texCoordTile[0].x); vertices[pos + 23] = (texCoordTile[1].y);
	}
	else {
		// First triangle
		vertices[pos] = (0); vertices[pos + 1] = (0);
		vertices[pos + 2] = (0); vertices[pos + 3] = (0);
		vertices[pos + 4] = (0); vertices[pos + 5] = (0);
		vertices[pos + 6] = (0); vertices[pos + 7] = (0);
		vertices[pos + 8] = (0); vertices[pos + 9] = (0);
		vertices[pos + 10] = (0); vertices[pos + 11] = (0);
		// Second triangle
		vertices[pos + 12] = (0); vertices[pos + 13] = (0);
		vertices[pos + 14] = (0); vertices[pos + 15] = (0);
		vertices[pos + 16] = (0); vertices[pos + 17] = (0);
		vertices[pos + 18] = (0); vertices[pos + 19] = (0);
		vertices[pos + 20] = (0); vertices[pos + 21] = (0);
		vertices[pos + 22] = (0); vertices[pos + 23] = (0);
	}
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mapSize.x * mapSize.y * 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = prog->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = prog->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

int TileMap::getTile(int x, int y) {
	return map[y * mapSize.x + x];
}

void TileMap::free()
{
	glDeleteBuffers(1, &vbo);
}

bool TileMap::loadLevel(const string &levelFile)
{
	srand(time(NULL));
	ifstream fin;
	string line, tilesheetFile;
	stringstream sstream;
	int tile;

	fin.open(levelFile.c_str());
	if (!fin.is_open())
		return false;
	getline(fin, line);
	if (line.compare(0, 7, "TILEMAP") != 0)
		return false;
	getline(fin, line);
	sstream.str(line);
	sstream >> mapSize.x >> mapSize.y;
	getline(fin, line);
	sstream.str(line);
	sstream >> tileSize >> blockSize;
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetFile;
	tilesheet.loadFromFile(tilesheetFile, TEXTURE_PIXEL_FORMAT_RGBA);
	tilesheet.setWrapS(GL_CLAMP_TO_EDGE);
	tilesheet.setWrapT(GL_CLAMP_TO_EDGE);
	tilesheet.setMinFilter(GL_NEAREST);
	tilesheet.setMagFilter(GL_NEAREST);
	getline(fin, line);
	sstream.str(line);
	sstream >> tilesheetSize.x >> tilesheetSize.y;
	tileTexSize = glm::vec2(1.f / tilesheetSize.x, 1.f / tilesheetSize.y);

	map = new int[mapSize.x * mapSize.y];
	for (int j = 0; j<mapSize.y; j++)
	{
		sstream.clear();
		getline(fin, line);
		sstream.str(line);
		for (int i = 0; i<mapSize.x; i++)
		{
			sstream >> tile;
			map[j*mapSize.x + i] = tile;
		}
		//fin.get(tile);
#ifndef _WIN32
		//fin.get(tile);
#endif
	}
	fin.close();

	return true;
}

void TileMap::prepareArrays(const glm::vec2 &minCoords)
{
	int tile;
	glm::vec2 posTile, texCoordTile[2];
	vertices = vector<float>(int(mapSize.x) * int(mapSize.y) * 24);
	for (int j = 0; j<mapSize.y; j++)
	{
		for (int i = 0; i<mapSize.x; i++)
		{
			tile = map[j * mapSize.x + i];
			int pos = j * mapSize.x * 24 + i * 24;
			posTile = glm::vec2(minCoords.x + i * tileSize, minCoords.y + j * tileSize);
			if (tile != 0)
			{
				// Non-empty tile
				texCoordTile[0] = glm::vec2(float((tile - 1) % tilesheetSize.x) / tilesheetSize.x, float((tile - 1) / tilesheetSize.x) / tilesheetSize.y);
				texCoordTile[1] = texCoordTile[0] + tileTexSize;
				// First triangle
				vertices[pos] = (posTile.x); vertices[pos + 1] = (posTile.y);
				vertices[pos + 2] = (texCoordTile[0].x); vertices[pos + 3] = (texCoordTile[0].y);
				vertices[pos + 4] = (posTile.x + blockSize); vertices[pos + 5] = (posTile.y);
				vertices[pos + 6] = (texCoordTile[1].x); vertices[pos + 7] = (texCoordTile[0].y);
				vertices[pos + 8] = (posTile.x + blockSize); vertices[pos + 9] = (posTile.y + blockSize);
				vertices[pos + 10] = (texCoordTile[1].x); vertices[pos + 11] = (texCoordTile[1].y);
				// Second triangle
				vertices[pos + 12] = (posTile.x); vertices[pos + 13] = (posTile.y);
				vertices[pos + 14] = (texCoordTile[0].x); vertices[pos + 15] = (texCoordTile[0].y);
				vertices[pos + 16] = (posTile.x + blockSize); vertices[pos + 17] = (posTile.y + blockSize);
				vertices[pos + 18] = (texCoordTile[1].x); vertices[pos + 19] = (texCoordTile[1].y);
				vertices[pos + 20] = (posTile.x); vertices[pos + 21] = (posTile.y + blockSize);
				vertices[pos + 22] = (texCoordTile[0].x); vertices[pos + 23] = (texCoordTile[1].y);
			}
			else {
				// First triangle
				vertices[pos] = (0); vertices[pos + 1] = (0);
				vertices[pos + 2] = (0); vertices[pos + 3] = (0);
				vertices[pos + 4] = (0); vertices[pos + 5] = (0);
				vertices[pos + 6] = (0); vertices[pos + 7] = (0);
				vertices[pos + 8] = (posTile.x); vertices[pos + 9] = (0);
				vertices[pos + 10] = (0); vertices[pos + 11] = (0);
				// Second triangle
				vertices[pos + 12] = (0); vertices[pos + 13] = (0);
				vertices[pos + 14] = (0); vertices[pos + 15] = (0);
				vertices[pos + 16] = (0); vertices[pos + 17] = (0);
				vertices[pos + 18] = (0); vertices[pos + 19] = (0);
				vertices[pos + 20] = (0); vertices[pos + 21] = (0);
				vertices[pos + 22] = (0); vertices[pos + 23] = (0);
			}
		}
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mapSize.x * mapSize.y * 24 * sizeof(float), &vertices[0], GL_STATIC_DRAW);
	posLocation = prog->bindVertexAttribute("position", 2, 4 * sizeof(float), 0);
	texCoordLocation = prog->bindVertexAttribute("texCoord", 2, 4 * sizeof(float), (void *)(2 * sizeof(float)));
}

// Collision tests for axis aligned bounding boxes.
// Method collisionMoveDown also corrects Y coordinate if the box is
// already intersecting a tile below.

bool TileMap::collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = pos.x / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) != 6 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) < 11)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x, y0, y1;

	x = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int y = y0; y <= y1; y++)
	{
		if (map[y*mapSize.x + x] != 0 && (map[y*mapSize.x + x]-1) / (tilesheetSize.x) != 6 && (map[y*mapSize.x + x]-1) / (tilesheetSize.x) < 11)
			return true;
	}

	return false;
}

bool TileMap::collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0 && (map[y*mapSize.x + x]-1) / (tilesheetSize.x) != 6 && (map[y*mapSize.x + x]-1) / (tilesheetSize.x) < 11)
		{
			if (*posY - tileSize * y + size.y <= 7)
			{
				*posY = tileSize * y - size.y;
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, int *posY) const
{
	int x0, x1, y;

	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y = pos.y / tileSize;
	for (int x = x0; x <= x1; x++)
	{
		if (map[y*mapSize.x + x] != 0 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) != 6 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) < 11)
		{
			if (*posY - tileSize * y <= 15)
			{
				*posY = tileSize * (y+1);
				return true;
			}
		}
	}

	return false;
}

bool TileMap::collisionNecro(const glm::ivec2 &pos, const glm::ivec2 &size) const
{
	int x0, x1, y0, y1;
	x0 = pos.x / tileSize;
	x1 = (pos.x + size.x - 1) / tileSize;
	y0 = pos.y / tileSize;
	y1 = (pos.y + size.y - 1) / tileSize;
	for (int x = x0; x <= x1; ++x) {
		for (int y = y0; y <= y1; ++y) {
			if (map[y*mapSize.x + x] != 0 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) != 6 && (map[y*mapSize.x + x] - 1) / (tilesheetSize.x) < 11) return true;
		}
	}
	return false;
}