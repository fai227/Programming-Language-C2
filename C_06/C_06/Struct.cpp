#include "Struct.h"

//‡Œv“¾“_‚ğ‹‚ß‚éŠÖ”
int sum(Player *player, int num) {
	int sum = 0;
	for (int i = 0; i < num; i++) {
		sum += player[i].point;
	}
	return sum;
}