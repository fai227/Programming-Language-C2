#pragma once

#define BUFFSIZE 256

struct Player
{
	char name[BUFFSIZE];
	int id;
	int point;
};

struct Team
{
	char name[BUFFSIZE];
	int point;
	Player* player;
};

int sum(Player* player, int num);