#include "pch.h"
//#include "stdafx.h"
#include "Planet.h"

Planet::Planet(Vektor2d pos, double mass, std::string img)
	: pos(pos)
	, mass(mass)
	, img(img)
{

}

void Planet::draw() {
	img.draw_rot(pos.get_x(), pos.get_y(), 0.0, 0.0, 0.5, 0.5, 0.1, 0.1);
}
