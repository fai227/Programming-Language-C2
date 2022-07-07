#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFSIZE 1024
#define MONTHS 12
#define DAYS 365
#define NUMDATA 2

//プロトタイプ宣言
void CSV2AveArray(const char *fileName, double data[MONTHS][NUMDATA]);


//メイン関数
int main(void)
{
	//CSVの読み込みと平均の計算
	double data[MONTHS][NUMDATA];
	CSV2AveArray("C_05_kyotanabedata.csv", data);

	//平均データの書き出し
	FILE* fp;
	errno_t	error;
	error = fopen_s(&fp, "C_05_kyotanabedata_output.csv", "w");
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else {
		for (int i = 0; i < MONTHS; i++) {
			char tmpString[BUFFSIZE];
			sprintf_s(tmpString, 30,"2018/%d,%f,%f\n", i + 1, data[i][0], data[i][1]);
			fputs(tmpString, fp);
		}

		fclose(fp);
	}
	
	return 0;
}

//ファイル読み込み関数
void CSV2AveArray(const char* fileName, double data[MONTHS][NUMDATA]) {
	FILE* fp;
	char s[BUFFSIZE];

	errno_t	error;
	error = fopen_s(&fp, fileName, "r");

	int count = -1;
	if (error != 0) {
		fprintf_s(stderr, "failed to open");
	}
	else {
		//計算用変数
		char monthName[8];
		double sum[2] = { 0.0, 0.0 };
		
		int monthCount = -1;
		int dayCount = 0;

		while (fgets(s, BUFFSIZE, fp) != NULL) {
			//一行目は除外
			if (count >= 0) {
				char* p1;
				char delim[] = ",";
				char* ctx;

				p1 = strtok_s(s, delim, &ctx);
				
				char tmpName[8];
				strncpy_s(tmpName, 8, p1, 7);

				//合計初期化
				if (strcmp(monthName, tmpName) == 0) { //月が同じとき
						
				}
				else { //月が変わったとき
					//最初は計算しない
					if (monthCount >= 0) {
						//合計を計算
						data[monthCount][0] = sum[0] / (double)dayCount;
						data[monthCount][1] = sum[1] / (double)dayCount;
					}
					   
					

					//初期化
					strcpy_s(monthName, tmpName);
					sum[0] = 0.0;
					sum[1] = 0.0;
					dayCount = 0;
					monthCount++;
				}	

				
				int tmpCount = 0;
				while (p1 != NULL) {
					if (tmpCount > NUMDATA - 1) break;
					p1 = strtok_s(NULL, delim, &ctx);
					sum[tmpCount] += atof(p1);
					tmpCount++;
				}
				
				dayCount++;
				
			}

			count++;			
		}
		fclose(fp);

		//月が変わったときの処理をもう一度行う
		data[monthCount][0] = sum[0] / (double)dayCount;
		data[monthCount][1] = sum[1] / (double)dayCount;

	}
}