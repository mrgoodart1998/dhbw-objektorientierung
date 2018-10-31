#include "stdafx.h"
#include "pch.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"
#include "Funktionen.h"
using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

vector<double> hindernisse_x;
vector<double> hindernisse_y;
int anz_hindernisse = 10;
int i = 0;

class GameWindow : public Gosu::Window
{
	Gosu::Image bild;
	Gosu::Image hindernis;
	Gosu::Image boom;
	double pos_x1 = graphics().width() / 2.0;
	double pos_y1 = graphics().width() / 2.0;
	double pos_x2 = graphics().width() / 2.0;
	double pos_y2 = 800.0;
	double pos_y3 = 10000000.0;
	double pos_x3 = 10000000.0;
	double vel_x, vel_y, angle1;
public:

	GameWindow()
		: Window(1000, 1000)
		, bild("Autorot.png")
		, hindernis("Autoblau.png")
		, boom("boom.png")
		// Rakete startet in der Mitte des Bildschirmes
		, pos(graphics().width() / 2.0, graphics().height() / 2.0)

	{
		vel_x = vel_y = angle1 = 0;
		set_caption("Autorennspieleee");

		// Erzeuge Planeten
		//planets.push_back(Planet({ 200.0, 200.0 }, 0.1, "Autoweiss.png"));
		//planets.push_back(Planet({ 600.0, 200.0 }, 0.1, "Autogruen.png"));
		//planets.push_back(Planet({ 400.0, 500.0 }, 0.1, "Autoblau.png"));
	}


	double x = 0;
	double y = 0;

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		do
		{
			hindernisse_x.push_back(Gosu::random(1, graphics().width())),
				hindernisse_y.push_back(Gosu::random(800, 2500));
			i++;
		} while (i < 10);

		//Gosu::random();

		bild.draw_rot(pos_x1, pos_y1, -10,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
		);


		for (int j = 0; j < 10; j++)
		{
			hindernis.draw_rot(hindernisse_x.at(j), hindernisse_y.at(j), -10000,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
			);
		}


		//bild.draw_rot(pos.get_x(), pos.get_y(), 10.0,
			//rot, // Rotationswinkel in Grad
			//0.5, 0.5 // Position der "Mitte"
		hindernis.draw_rot(pos_x2, pos_y2, -10000,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
		);

		boom.draw_rot(pos_x3, pos_y3, 100, 0, 0.5, 0.5, 0.5, 0.5);

		//graphics().draw_triangle(0, 0, Gosu::Color::BLACK, 800, 0, Gosu::Color::BLACK, 400, 600, Gosu::Color::BLACK, -100);


		auto g2 = (gravity * 1000000000000.0).log();

		Vektor2d rose(50.0, 50.0);
		auto g = rose - g2;
		auto s = rose + speed * 1000.0;

		graphics().draw_rect(20, 20, 20, 10, Gosu::Color::WHITE, -100);
		//graphics().draw_line(pos.get_x(), pos.get_y(), Gosu::Color::GREEN, input().mouse_x(), input().mouse_y(), Gosu::Color::GREEN, -10.0);
		//graphics().draw_line(rose.get_x(), rose.get_y(), Gosu::Color::RED, g.get_x(), g.get_y(), Gosu::Color::RED, 10.0);
		//graphics().draw_line(rose.get_x(), rose.get_y(), Gosu::Color::BLUE, s.get_x(), s.get_y(), Gosu::Color::BLUE, 10.0);

		for (auto planet : planets) {
			planet.draw();
		}

	}

	double rot = 0.0;
	Vektor2d pos, speed, gravity;
	Funktionen abstand;
	double accel = 0.0;
	std::vector<Planet> planets;


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{




		if ((input().down(Gosu::KB_LEFT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			pos_x1 -= 3;
			rot -= 10;
		}
		else if ((input().down(Gosu::KB_LEFT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 2.0) && (pos_x1 <= pos_x2))
		{
			pos_x1 -= 3;
		}

		if ((input().down(Gosu::KB_RIGHT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			pos_x1 += 3;
			rot += 10;
		}
		else if ((input().down(Gosu::KB_RIGHT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 2.0) && (pos_x1 >= pos_x2))
		{
			pos_x1 += 3;
		}

		//if (input().down(Gosu::KB_DOWN) && (pos_y2!=(pos_y1+20)))
			//pos_y1 += 3;

		cout << abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2) << endl;

		if ((input().down(Gosu::KB_DOWN)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			pos_y2 -= 3;
			for (int j = 0; j < hindernisse_x.size(); j++)
			{
				hindernisse_y.at(j) -= 3;
			}
		}
		//else if ((input().down(Gosu::KB_DOWN)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) < 1.0))
			//pos_y1 += 3;


		if ((input().down(Gosu::KB_UP)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) < 1.0))
			pos_y1 -= 5;

		if (input().down(Gosu::KB_ESCAPE))
			close();



		// Crash-Erkennung
		if ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 1.0 && (pos_x3 != graphics().width() / 2.0))
		{
			pos_y3 = pos_y1 + 20;
			pos_x3 = graphics().width() / 2.0;
		};

		//cout << pos_x1 << "    " << pos_x2 << "       " << pos_y1 << "    " << pos_y2 << "   " << bild.width() << "   " << bild.height() << endl;


		// Geschwindigkeit führt zu Positionsänderung
		//pos += speed * DT;

		/*
		// Beschleunigung während "W" gedrückt
		if (input().down(Gosu::KB_W)) {
			accel += 0.00001;
		}
		else {
			accel -= 0.00002;
		}

		// Maximale Beschleunigung
		accel = Gosu::clamp(accel, 0.0, 0.0001);

		// Geschwindigkeit wird in Flugrichtung geändert
		speed += Vektor2d::from_angle(rot, accel);


		gravity = Vektor2d();

		// Planeten ziehen Raumschiff an
		for (auto planet : planets) {

			// Entfernung im Quadrat
			double dist = (pos - planet.pos).length_squared();

			// Winkel des Vektors vom Raumschiff zum Planet
			double angle = pos.angle(planet.pos);

			// Anziehungskraft
			double pull = planet.mass / dist;

			// Kollision
			if (dist < 1000) {
				speed = Vektor2d::from_angle(angle, pull);
				break;
			}
			gravity += Vektor2d::from_angle(angle, pull);
		}

		speed += gravity;

		// Raumschiff in Richtung Mauszeiger drehen
		double angle = pos.angle({ input().mouse_x(), input().mouse_y() });
		rot -= Gosu::angle_diff(angle, rot) / 36.0;
		*/

	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}