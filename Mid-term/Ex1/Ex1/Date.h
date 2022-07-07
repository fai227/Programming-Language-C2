#pragma once

#define MONTH 12
#define DAY 31

struct Day {
	double aveTemp;
	double highTemp;
	double lowTemp;

	//�������R���X�g���N�^
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

	//�������R���X�g���N�^
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

	//�������R���X�g���N�^
	Year() {
		aveTemp = 0.0;
		highTemp = 0.0;
		lowTemp = 0.0;
	}
};