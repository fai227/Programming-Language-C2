#include <stdio.h>
#include <curses.h>
#include "Setting.h"
#include "Date.h"
#include "string.h"
#include "Chart.h"
#include <stdlib.h>

#define YEAR 10

void GetCSVData(const char* csvFile, Year year[YEAR]);

int main()
{
	//----------ファイル入力からデータ生成----------
	//現在のディレクトリを取得
	char currentDirectory[CHARBUFF];
	getCurrentDirectory(currentDirectory);
	//設定ファイルのディレクトリを設定
	char settingFile[CHARBUFF];
	sprintf_s(settingFile, "%s\\Setting.ini", currentDirectory);


	//文字列
	char section[CHARBUFF];
	char keyWord[CHARBUFF];
	char csvFileName[CHARBUFF];
	char csvFile[CHARBUFF];

	//読み込み先1指定
	sprintf_s(section, "SETTING");
	sprintf_s(keyWord, "CSV_FILE_NAME1");
	//CSVファイル1を読み込み
	readChar(section, keyWord, "Nara.csv", csvFileName, settingFile);
	sprintf_s(csvFile, "%s\\%s", currentDirectory, csvFileName);
	//データ1の設定
	Year data_one[YEAR];
	GetCSVData(csvFile, data_one);
	
	//読み込み先2指定
	sprintf_s(section, "SETTING");
	sprintf_s(keyWord, "CSV_FILE_NAME2");
	//CSVファイル2を読み込み
	readChar(section, keyWord, "Kyoto.csv", csvFileName, settingFile);
	sprintf_s(csvFile, "%s\\%s", currentDirectory, csvFileName);
	//データ2の設定
	Year data_two[YEAR];
	GetCSVData(csvFile, data_two);

	//----------操作----------
	//左の年、左の月、右の年、右の月
	int nums[] = { 0, 0, 0, 0};
	int maxs[] = { YEAR - 1, MONTH - 1, YEAR - 1, MONTH - 1 };
	int pointer = 0;
	int type = 0;

	//画面初期化
	int screenWidth = 80;
	int screenHeight = 40;
	initscr();
	noecho();
	resize_term(screenHeight, screenWidth);

	bool endFlag = false;
	while (!endFlag) {
		//キー入力
		char key = 0;
		while (key == 0) {	//キー入力を待つ
			key = getch();
			//キー入力がないときは何もしない
			if (key == 0) continue;

			switch (key)
			{

			case 'a':	//左にずらす
				pointer--;
				if (pointer < 0) pointer = 0;
				break;

			case 'd':	//右にずらす
				pointer++;
				if (pointer > 3) pointer = 3;
				break;

			case 'w':	//数を増やす
				nums[pointer]++;
				if (nums[pointer] > maxs[pointer]) nums[pointer] = maxs[pointer];
				break;

			case 's':	//数を減らす
				nums[pointer]--;
				if (nums[pointer] < 0) nums[pointer] = 0;
				break;

			case 'e':	//平均気温、最高気温、最低気温を変える
				type = (type + 1) % 3;
				break;

			case ' ':
				endFlag = true;
				break;

			default:
				break;
			}
		}

		//画面クリア
		erase();

		char tmpChar[CHARBUFF];

		//必要なデータの計算
		double dataOne[DAY];
		double dataTwo[DAY];
		double dataX[MONTH * DAY];

		for (int i = 0; i < DAY; i++) {
			dataX[i] = (double)i;

			switch (type)
			{
			case 0:
				dataOne[i] = data_one[nums[0]].month[nums[1]].day[i].aveTemp;
				dataTwo[i] = data_one[nums[2]].month[nums[3]].day[i].aveTemp;
				sprintf_s(tmpChar, "平均気温");
				break;

			case 1:
				dataOne[i] = data_one[nums[0]].month[nums[1]].day[i].highTemp;
				dataTwo[i] = data_one[nums[2]].month[nums[3]].day[i].highTemp;
				sprintf_s(tmpChar, "最高気温");
				break;

			default:
				dataOne[i] = data_one[nums[0]].month[nums[1]].day[i].lowTemp;
				dataTwo[i] = data_one[nums[2]].month[nums[3]].day[i].lowTemp;
				sprintf_s(tmpChar, "最低気温");
				break;
			}
		}
		mvaddstr(29, 36, tmpChar);


		sprintf_s(tmpChar, "データ１：%d年%d月", (nums[0] + 2011), (nums[1] + 1));
		Chart(0, 0, 40, 30, dataX, dataOne, DAY, tmpChar);
		sprintf_s(tmpChar, "データ２：%d年%d月", (nums[2] + 2011), (nums[3] + 1));
		Chart(40, 0, 40, 30, dataX, dataTwo, DAY, tmpChar);

		//マーカーを表示
		switch (pointer)
		{
		case 0:
			mvaddch(30, 24, '*');
			break;

		case 1:
			mvaddch(30, 27, '*');
			break;

		case 2:
			mvaddch(30, 64, '*');
			break;

		default:
			mvaddch(30, 67, '*');
			break;
		}

		//温度差を計算
		float dif[YEAR];

		for (int y = 0; y < YEAR; y++) {
			dif[y] = 0.0;
			for (int m = 0; m < MONTH; m++) {
				switch (type)
				{
				case 0:
					dif[y] += data_one[y].month[m].aveTemp - data_two[y].month[m].aveTemp;
					break;

				case 1:
					dif[y] += data_one[y].month[m].highTemp - data_two[y].month[m].highTemp;
					break;

				default:
					dif[y] += data_one[y].month[m].lowTemp - data_two[y].month[m].lowTemp;
					break;
				}
			}
		}

		//温度差を表示
		mvaddstr(32, 5, "温度差（データ１ーデータ２）");
		for (int y = 0; y < YEAR; y++) {
			//年を表示
			sprintf_s(tmpChar, "%d年", (2011 + y));
			mvaddstr(34, (y + 1) * 7, tmpChar);

			//差を表示
			sprintf_s(tmpChar, "%d", (int)dif[y]);
			mvaddstr(35, (y + 1) * 7, tmpChar);
		}

		refresh();
	}	
}

void GetCSVData(const char* csvFile, Year years[YEAR]) {
	//読み込み開始
	fprintf_s(stdout, "%sの読み込みを開始します。\n", csvFile);
	FILE* fp;

	char fileLine[CHARBUFF];
	char monthName[CHARBUFF] = "****/**";
	char* p1, * ctx;

	char delim[] = ", ";

	errno_t error;
	error = fopen_s(&fp, csvFile, "r");
	if (error != 0) {
		fprintf_s(stderr, "読み込みに失敗しました。");
	}
	else {
		int tmpYear = 0;
		int tmpMonth = 0;
		int tmpDay = 0;
		//一行ずつ読んでいく
		while (fgets(fileLine, CHARBUFF, fp) != NULL) {
			//分割してポインタp1に渡す
			p1 = strtok_s(fileLine, delim, &ctx);

			//現在の年月を取得
			char tmpMonthName[CHARBUFF];
			strncpy_s(tmpMonthName, CHARBUFF, p1, 7);

			if (strcmp(monthName, tmpMonthName) != 0) {  //月が変わったとき
				if (tmpDay != 0) {  //一番最初は行わない
					strcpy_s(monthName, tmpMonthName);
					//日付を戻す
					tmpMonth++;
					tmpDay = 0;
					if (tmpMonth >= MONTH) {  //年が変わったので月を戻す
						tmpYear++;
						tmpMonth = 0;
					}
				}
				else {  //一番最初は登録のみ
					strcpy_s(monthName, tmpMonthName);
				}
			}

			int i = 0;
			while (p1 != NULL) {
				if (i >= 3) break;  //データを取り出し切ったらやめる
				p1 = strtok_s(NULL, delim, &ctx);
				//0の場合は登録できないので0.001で登録
				double tmpValue = atof(p1);
				if (tmpValue == 0.0) tmpValue = 0.001;
				switch (i) {
				case 0:  //平均気温
					years[tmpYear].month[tmpMonth].day[tmpDay].aveTemp = tmpValue;
					break;
				case 1:  //最高気温
					years[tmpYear].month[tmpMonth].day[tmpDay].highTemp = tmpValue;
					break;

				case 2:  //最低気温
					years[tmpYear].month[tmpMonth].day[tmpDay].lowTemp = tmpValue;
					break;
				}
				i++;
			}

			//終了時日にちを１つ進める
			tmpDay++;
		}
		fclose(fp);
	}

	//月の平均を求める
	for (int tmpYear = 0; tmpYear < YEAR; tmpYear++) {
		for (int tmpMonth = 0; tmpMonth < MONTH; tmpMonth++) {
			double aveTmp = 0.0;
			double highTmp = 0.0;
			double lowTmp = 0.0;

			int dayLength = DAY;

			for (int tmpDay = 0; tmpDay < DAY; tmpDay++) {
				if (years[tmpYear].month[tmpMonth].day[tmpDay].aveTemp != 0.0) {  //代入されているとき
					aveTmp += years[tmpYear].month[tmpMonth].day[tmpDay].aveTemp;
					highTmp += years[tmpYear].month[tmpMonth].day[tmpDay].highTemp;
					lowTmp += years[tmpYear].month[tmpMonth].day[tmpDay].lowTemp;
				}
				else {
					dayLength = tmpDay;
					break;
				}
			}

			//月平均の登録
			years[tmpYear].month[tmpMonth].aveTemp = aveTmp / (double)dayLength;
			years[tmpYear].month[tmpMonth].highTemp = highTmp / (double)dayLength;
			years[tmpYear].month[tmpMonth].lowTemp = lowTmp / (double)dayLength;
		}
	}

	//年の平均を求める
	for (int tmpYear = 0; tmpYear < YEAR; tmpYear++) {
		double aveTmp = 0.0;
		double highTmp = 0.0;
		double lowTmp = 0.0;

		for (int tmpMonth = 0; tmpMonth < MONTH; tmpMonth++) {
			aveTmp += years[tmpYear].month[tmpMonth].aveTemp;
			highTmp += years[tmpYear].month[tmpMonth].highTemp;
			lowTmp += years[tmpYear].month[tmpMonth].lowTemp;
		}

		//年平均の登録
		years[tmpYear].aveTemp = aveTmp / MONTH;
		years[tmpYear].highTemp = highTmp / MONTH;
		years[tmpYear].lowTemp = lowTmp / MONTH;
	}
}