#include "Struct.h"

//合計得点を求める関数
int sum(Player *player, int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum += player[i].point;
	}
	return sum;
}