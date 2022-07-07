#include <curses.h>
#include <string.h>

void DrawChart(int x, int y, int w, int h, double* dataX, double* dataY, int array_size) {
	//�ő�l�ŏ��l�v�Z
	double maxX = dataX[0];
	double minX = dataX[0];
	double maxY = dataY[0];
	double minY = dataY[0];

	for (int i = 0; i < array_size; i++) {
		double tmpDataX = dataX[i];
		if (tmpDataX > maxX) {
			maxX = tmpDataX;
		}
		else if (tmpDataX < minX) {
			minX = tmpDataX;
		}

		double tmpDataY = dataY[i];
		if (tmpDataY > maxY) {
			maxY = tmpDataY;
		}
		else if (tmpDataY < minY) {
			minY = tmpDataY;
		}
	}

	//�O���t�`��
	for (int i = 0; i < array_size; i++) {
		double positionX = (double)x + (double)w * (dataX[i] - minX) / (maxX - minX);
		double positionY = (double)y + (double)h - (double)h * (dataY[i] - minY) / (maxY - minY);
		mvaddch((int)positionY, (int)positionX, '*');
	}
}

void Chart(int x, int y, int w, int h, double* dataX, double* dataY, int array_size, char* graghName) {
	//����̐���\��
	int graphH = h - 1;
	for (int i = 0; i < graphH; i++) {
		mvaddch(y + i, x, '|');
		mvaddch(y + i, x + w - 1, '|');
	}
	for (int i = 0; i < w; i++) {
		mvaddch(y, x + i, '-');
		mvaddch(y + graphH - 1, x + i, '-');
	}

	//���O���t�̕\��
	DrawChart(x + 1, y + 1, w - 3, graphH - 3, dataX, dataY, array_size);

	//�^�C�g���̕\��
	mvaddstr(y + h - 1, x + w / 2 - strlen(graghName) / 2, graghName);
}
