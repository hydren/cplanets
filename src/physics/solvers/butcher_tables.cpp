/*
 * butcher_tables.cpp
 *
 *  Created on: 25 de jul de 2016
 *      Author: carlosfaruolo
 */

#include "runge_kutta.hpp"

#include <cmath>

#define X 0

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

const double RK3_1[4][4] = {
		{0,		X,			X, 			X},
		{0.5,	0.5,		X, 			X},
		{1,		-1,			2, 			X},
		{X,		1.0/6.0,	2.0/3.0,	1.0/6.0}
};
CREATE_BUILTIN_TABLE(RK3_1, 4);

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
		{0,			X,					X,					X,					X},
		{0.5,	 	0.5,				X,					X,					X},
		{0.5,		0.5*sqrt(2)-0.5,	1-sqrt(2)*0.5,		X,					X},
		{1,			-sqrt(2)*0.5,		1+sqrt(2)*0.5,		1,					X},
		{X,			1.0/6.0,			(2-sqrt(2))/6.0,	(2+sqrt(2))/6.0, 	1}
};
CREATE_BUILTIN_TABLE(RK4_GILL, 5);

const double RK4_RALSTON[5][5] = {
		{0,							X,								X,							X,										X},
		{0.4, 						0.4,							X,							X,										X},
		{7.0/8.0-3.0*sqrt(5)/16.0,	(-2889+1428*sqrt(5))/1024.0,	(3785-1620*sqrt(5))/1024.0,	X,										X},
		{1,							(-3365+2094*sqrt(5))/6040.0,	(-975-3046*sqrt(5))/2552.0,	(467040+203968*sqrt(5))/2400845.0,		X},
		{X,							(263 + 24*sqrt(5))/1812.0,		(125-1000*sqrt(5))/3828.0,	1024*(3346 + 1623*sqrt(5))/5924787.0, 	(30 - 4*sqrt(5))/123.0}
};
CREATE_BUILTIN_TABLE(RK4_RALSTON, 5);


