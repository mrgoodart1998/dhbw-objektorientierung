#include "pch.h"
//#include "stdafx.h"
#include "Funktionen.h"
#include "Gosu/Gosu.hpp"
#include <cmath>

double Funktionen::get_abstand_x(double ob1_breite, double ob2_breite, double ob1_posx, double ob2_posx) const
{
	double abstand_x;
	double halbe_x1 = ob1_breite *0.25;
	double halbe_x2 = ob2_breite *0.25;
	if (ob2_posx > ob1_posx)
		abstand_x = ob2_posx - ob1_posx - halbe_x1 - halbe_x2;
	else if (ob1_posx > ob2_posx)
		abstand_x = ob1_posx - ob2_posx - halbe_x1 - halbe_x2;
	else if (ob1_posx == ob2_posx)
		abstand_x = 0;
	if (abstand_x < 0)
		abstand_x = 0;

	return abstand_x;
}

double Funktionen::get_abstand_y(double ob1_hoehe, double ob2_hoehe, double ob1_posy, double ob2_posy) const
{
	double hoehe_y1 = ob1_hoehe;
	double hoehe_y2 = ob2_hoehe;
	double differenz;
	double abstand_y = 99999;
	double y1_oben = ob1_posy - hoehe_y1;
	double y2_oben = ob2_posy - hoehe_y2;
	for (int i = y1_oben; i < y1_oben + hoehe_y1; i += 5)
	{
		for (int j = y2_oben; j < y2_oben + hoehe_y2; j += 5)
		{
			if (y1_oben + i > y2_oben + j)
				differenz = y1_oben + i - y2_oben - j;
			else if (y1_oben + i < y2_oben + j)
				differenz = y2_oben + j - y1_oben - i;
			else if (y1_oben + i == y2_oben + j)
				differenz = 0;
			if (differenz <= 5)
				differenz = 0;
			if (differenz < abstand_y)
				abstand_y = differenz;
		}
	}

	return abstand_y;
}