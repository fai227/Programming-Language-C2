#pragma once

#define MONTH 12
#define DAY 31

struct Day {
	double aveTemp;
	double highTemp;
	double lowTemp;

	//初期化コンストラクタ
	Day() {
		aveTemp = 0.0;
		highTemp = 0.0;
		lowTemp = 0.0;
	}
};

struct Month {
	double aveTemp;
	double highTemp;
	double lowTemp;
	Day day[DAY];

	//初期化コンストラクタ
	Month() {
		aveTemp = 0.0;
		highTemp = 0.0;
		lowTemp = 0.0;
	}
};

struct Year {
	double aveTemp;
	double highTemp;
	double lowTemp;
	Month month[MONTH];

	//初期化コンストラクタ
	Year() {
		aveTemp = 0.0;
		highTemp = 0.0;
		lowTemp = 0.0;
	}
};