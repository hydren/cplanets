/*
 * butcher_tables.cpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "runge_kutta.hpp"

#include <cmath>

#define X 0
#define s2 sqrt(2)
#define s5 sqrt(5)
#define s21 sqrt(21)

#define CREATE_BUILTIN_TABLE(name, size) const ButcherTable ButcherTable::name(size, copytable<size>(::name))

template<unsigned size>
double** copytable(const double table[size][size])
{
	double** data = new double*[size];
	for(unsigned i = 0; i < size; i++)
	{
		data[i] = new double[size];
		std::copy(table[i], table[i] + size, data[i]);
	}
	return data;
}

// sample of tables
const double RK1_EULER[2][2] = {
		{0,	X},
		{X,	1}
};
CREATE_BUILTIN_TABLE(RK1_EULER, 2);

const double RK2_MIDPOINT[3][3] = {
		{0,		X,		X},
		{0.5,	0.5,	X},
		{X,		0,		1}
};
CREATE_BUILTIN_TABLE(RK2_MIDPOINT, 3);

const double RK2_HEUN[3][3] = {
		{0,		X,		X},
		{1,		1,		X},
		{X,		-0.5,	0.5}
};
CREATE_BUILTIN_TABLE(RK2_HEUN, 3);

const double RK2_RALSTON[3][3] = {
		{0,			X,			X},
		{2.0/3.0,	2.0/3.0,	X},
		{X,			0.25,		0.75}
};
CREATE_BUILTIN_TABLE(RK2_RALSTON, 3);

const double RK3_KUTTA[4][4] = {
		{0,		X,			X, 			X},
		{0.5,	0.5,		X, 			X},
		{1,		-1,			2, 			X},
		{X,		1.0/6.0,	2.0/3.0,	1.0/6.0}
};
CREATE_BUILTIN_TABLE(RK3_KUTTA, 4);

const double RK3_2[4][4] = {
		{0,			X,			X, 			X},
		{1.0/3.0,	1.0/3.0,	X, 			X},
		{2.0/3.0,	0,			2.0/3.0,	X},
		{X,			0.25,		0,			0.75}
};
CREATE_BUILTIN_TABLE(RK3_2, 4);

const double RK4_CLASSIC[5][5] = {
		{0,		X,			X,			X,			X},
		{0.5, 	0.5,		X,			X,			X},
		{0.5,	0,			0.5,		X,			X},
		{1,		0,			0,			1,			X},
		{X,		1.0/6.0,	1.0/3.0,	1.0/3.0, 	1.0/6.0}
};
CREATE_BUILTIN_TABLE(RK4_CLASSIC, 5);

const double RK4_RULE_3_8[5][5] = {
		{0,			X,			X,			X,			X},
		{1.0/3.0, 	1.0/3.0,	X,			X,			X},
		{2.0/3.0,	-1.0/3.0,	1,			X,			X},
		{1,			1,			-1,			1,			X},
		{X,			0.125,		0.375,		0.375, 		0.125}
};
CREATE_BUILTIN_TABLE(RK4_RULE_3_8, 5);

const double RK4_GILL[5][5] = {
		{0,			X,				X,				X,				X},
		{0.5,	 	0.5,			X,				X,				X},
		{0.5,		0.5*s2-0.5,		1-s2*0.5,		X,				X},
		{1,			0,				-s2*0.5,		1+s2*0.5,		X},
		{X,			1.0/6.0,		(2-s2)/6.0,		(2+s2)/6.0, 	1.0/6.0}
};
CREATE_BUILTIN_TABLE(RK4_GILL, 5);

const double RK4_RALSTON[5][5] = {
		{0,						X,						X,						X,										X},
		{0.4, 					0.4,					X,						X,										X},
		{0.875 - 0.1875*s5,		(-2889+1428*s5)/1024.0,	(3785*-1620*s5)/1024.0,	X,										X},
		{1,						(-3365+2094*s5)/6040.0,	(-975-3046*s5)/2552.0,	(467040+203968*s5)/2400845.0,			X},
		{X,						(263 + 24*s5)/1812.0,	(125-1000*s5)/3828.0,	(3346 + 1623*s5)*(1024.0/5924787.0),	(30 - 4*s5)/123.0}
};
CREATE_BUILTIN_TABLE(RK4_RALSTON, 5);

const double RK5_NYSTROM[7][7] = {
		{0,				X,				X,				X,				X,				X,				X},
		{1.0/3.0,		1.0/3.0,		X,				X,				X,				X,				X},
		{0.4,			4.0/25.0,		6.0/25.0,		X,				X,				X,				X},
		{1,				0.25,			-3,				15.0/4.0,		X, 				X,				X},
		{2.0/3.0,		2.0/27.0,		30.0/27.0,		-50.0/81.0,		8.0/81.0,		X,				X},
		{0.8,			2.0/25.0,		12.0/25.0,		2.0/15.0,		8.0/75.0, 		0,				X},
		{X,				23.0/192.0,		0,				125.0/192.0,	0,				-81.0/192.0,	125.0/192.0}
};
CREATE_BUILTIN_TABLE(RK5_NYSTROM, 7);

const double RK6_BUTCHER[8][8] = {
		{0,				X,				X,				X,				X,				X,				X,				X},
		{1.0/3.0,		1.0/3.0,		X,				X,				X,				X,				X,				X},
		{2.0/3.0,		0,				2.0/3.0,		X,				X,				X,				X,				X},
		{1.0/3.0,		1.0/12.0,		1.0/3.0,		-1.0/12.0,		X, 				X,				X,				X},
		{0.5,			-0.0625,		1.125,			-0.1875,		-0.375,			X,				X,				X},
		{0.5,			0,				1.125,			-0.375,			-0.75,	 		0.5,			X,				X},
		{1,				9.0/44.0,		-9.0/11.0,		63.0/44.0,		18.0/11.0, 		-16.0/11.0,		0,				X},
		{X,				11.0/120.0,		0,				81.0/120.0,		81.0/120.0,		-32.0/120.0,	-32.0/120.0,    11.0/120.0}
};
CREATE_BUILTIN_TABLE(RK6_BUTCHER, 8);

const double RK8_VERNER[12][12] = {
		{0,					X,				X,					X,						X,						X,						X,						X,						X,					X,					X,					X},
		{0.5,				0.5,			X,					X,						X,						X,						X,						X,						X,					X,					X,					X},
		{0.5,				0.25,			0.25,				X,						X,						X,						X,						X,						X,					X,					X,					X},
		{0.5+s21/14.0,		1.0/7.0,		(-7-3*s21)/98.0,	(21+5*s21)/49.0,		X, 						X,						X,						X,						X,					X,					X,					X},
		{0.5+s21/14.0,		(11+s21)/84.0,	0,					(18+4*s21)/63.0,		(21-s21)/252.0,			X,						X,						X,						X,					X,					X,					X},
		{0.5,				(5+s21)/48.0,	0,					(9+s21)/36.0,			(-231+14*s21)/360.0,	(63-7*s21)/80.0,		X,						X,						X,					X,					X,					X},
		{0.5-s21/14.0,		(10-s21)/42.0,	0,					(-432+92*s21)/315.0,	(633-145*s21)/90.0, 	(-504+115*s21)/70.0,	(63-13*s21)/35.0,		X,						X,					X,					X,					X},
		{0.5-s21/14.0,		1.0/14.0,		0,					0,						0, 						(14-3*s21)/126.0,		(13-3*s21)/63.0,		1.0/9.0,				X,					X,					X,					X},
		{0.5,				0.03125,		0,					0,						0,						(91-21*s21)/576.0,		11.0/72.0,				(-385-75*s21)/1152.0,	(63+13*s21)/128.0,	X,					X,					X},
		{0.5+s21/14.0,		1.0/14.0,		0,					0,						0,	 					1.0/9.0,				(-733-147*s21)/2205.0,	(515+111*s21)/504.0,	(-51-11*s21)/56.0,	(132+28*s21)/245.0,	X,					X},
		{1,					0,				0,					0,						0, 						(-42+7*s21)/18.0,		(-18+28*s21)/45.0,		(-273-53*s21)/72.0,		(301+53*s21)/72.0,	(28-28*s21)/45.0,	(49-7*s21)/18.0,	X},
		{X,					0.05,			0,					0,						0,						0,						0,    					0,						49.0/180.0,			16.0/45.0,			49.0/180.0,			0.05}
};
CREATE_BUILTIN_TABLE(RK8_VERNER, 12);
