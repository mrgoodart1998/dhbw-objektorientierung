#include "pch.h"
//#include "stdafx.h"
#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"
#include "Funktionen.h"
#define PI 3.14159265
using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

vector<double> hindernisse_x, hindernisse_y, abstaende1_x, abstaende1_y, abstaende1, abstaende2_x, abstaende2_y, abstaende2;
int anz_hindernisse = 10;
int i = 0;

class GameWindow : public Gosu::Window
{
	Gosu::Image player1, player2;
	Gosu::Image hindernis;
	Gosu::Image boom;
	Gosu::Image rasen;
	double pos_x1 = graphics().width() / 2.0;
	double pos_y1 = graphics().width() / 2.0;
	double pos_x2 = graphics().width() / 2.0;
	double pos_y2 = 800; // 0; // graphics().width() / 2.0;
	double pos_y3 = 10000000.0;
	double pos_x3 = 10000000.0;
	double pos_x4 = (graphics().width() / 2.0) + 200;
	double pos_y4 = graphics().width() / 2.0;
	double delta_pos = 0;
	double vel1 = 0;
	double vel2 = 0;
	double vel_x, vel_y, angle1;
	double posX_Streckenrand_links = 0;
	double posX_Streckenrand_rechts = 800;
	//Strecke
	vector<double> streckenstück_x;
	vector<double> streckenstück_y;
	//int streckenstück_idx = 0;
	const int c_ANZAHL_STRECKENSTUECKE = 10000;
	const int c_WINDOW_WIDTH = 1000;
	const int c_WINDOW_HIGHT = 1000;

	const int c_RASEN_BREITE = 24;

	//vector<double> oben_links_x;
	double delta_y = 24.5;
	double delta_bahn_y = 300;

public:

	GameWindow()
		: Window(1000, 1000)
		, player1("Autorot.png")
		, player2("Autogelb.png")
		, hindernis("Autoblau.png")
		, boom("boom.png")
		, rasen("stueck_rasen.png")
		//, start("start.png")
		//, bahn("stueck_bahn.png")
		// Rakete startet in der Mitte des Bildschirmes
		//, pos(graphics().width() / 2.0, graphics().height() / 2.0)

	{
		vel_x = vel_y = angle1 = 0;
		set_caption("Autorennspiel");

		double x_strecke = 500;
		double y_strecke = 0;


		for (int i = 0; i < c_ANZAHL_STRECKENSTUECKE; i++) {

			if ((i > 0) && (i < 21)) {
				x_strecke += 0;
			}
			else if (i > 20 && (i < 56)) {
				x_strecke += 2;
			}
			else if ((i > 55) && (i < 91)) {
				x_strecke -= 7;
			}
			else if ((i > 90) && (i < 121)) {
				x_strecke += 3;
			}
			else if ((i > 120) && (i < 151)) {
				x_strecke += 4;
			}
			else if ((i > 150) && (i < 176)) {
				x_strecke -= 5;
			}
			else if ((i > 175) && (i < 201)) {
				x_strecke += 8;
			}
			else if ((i > 200)) {
				x_strecke -= 1;
			}

			streckenstück_x.push_back(x_strecke);
			streckenstück_y.push_back(y_strecke);
			y_strecke += c_RASEN_BREITE;

		}
		//streckenstück_idx = 0;

		// Erzeuge Planeten
		//planets.push_back(Planet({ 200.0, 200.0 }, 0.1, "Autoweiss.png"));
		//planets.push_back(Planet({ 600.0, 200.0 }, 0.1, "Autogruen.png"));
		//planets.push_back(Planet({ 400.0, 500.0 }, 0.1, "Autoblau.png"));

		//Positionen der Hindernisse random bestimmen
		do
		{
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			//hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			//hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			//hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_y.push_back(Gosu::random(800, 5000));
			//hindernisse_y.push_back(Gosu::random(5000, 10000));
			//hindernisse_y.push_back(Gosu::random(10000, 15000));
			//hindernisse_y.push_back(Gosu::random(15000, 20000));
			i++;
		} while (i <= anz_hindernisse);

	}

	//double x = 500;
	//double y = 0;

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		// Strecke		
		int rasenOffsetY = (-1 * (pos_y2)) / c_RASEN_BREITE;
		// linker Rasen
		for (int n = rasenOffsetY; n < rasenOffsetY + c_WINDOW_HIGHT; n++) { // Nur Streckenstücke, die im Window liegen			
																			 // prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE - 1) || n < 0) {
				break;
			}
			rasen.draw_rot(pos_x2 - streckenstück_x[n], pos_y2 + streckenstück_y[n], 0, 0, 0.5, 0.5, 0.5);
		}
		// Rechter Rasen
		for (int n = rasenOffsetY; n < rasenOffsetY + c_WINDOW_HIGHT; n++) { // Nur Streckenstücke, die im Window liegen
																			 // prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE - 1) || n < 0) {
				break;
			}
			rasen.draw_rot(pos_x2 + 1000 - streckenstück_x[n], pos_y2 + streckenstück_y[n], 0, 180, 0.5, 0.5, 0.5);
		}
		// Nur Streckenstücke, die im Window liegen
		for (int n = rasenOffsetY; n < rasenOffsetY + c_WINDOW_HIGHT; n++) {
			// prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE - 1) || n < 0) {
				break;
			}
			double streckenstück_y_cur = -1 * streckenstück_y[n];
			double streckenstück_y_next = -1 * streckenstück_y[n + 1];
			double offsetY = 420;
			if ((pos_y2 <= (streckenstück_y_cur + offsetY)) && (pos_y2 >= (streckenstück_y_next + offsetY))) {

				posX_Streckenrand_links = (-1 * streckenstück_x[n]) + 770;

				graphics().draw_line(posX_Streckenrand_links, 0, Gosu::Color::RED, posX_Streckenrand_links, c_WINDOW_HIGHT, Gosu::Color::RED, 0);

				posX_Streckenrand_rechts = (-1 * streckenstück_x[n]) + 1230;

				graphics().draw_line(posX_Streckenrand_rechts, 0, Gosu::Color::RED, posX_Streckenrand_rechts, c_WINDOW_HIGHT, Gosu::Color::RED, 0);

			}
		}


		//Gosu::random();

		//Spieler 1 (rotes Auto)
		player1.draw_rot(pos_x1, pos_y1, 10,
			rot1, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
		);

		//Spieler 2 (gelbes Auto)
		player2.draw_rot(pos_x4, pos_y4, 10,
			rot2, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
		);

		//Zeichnen von Hindernissen
		for (int j = 0; j < anz_hindernisse; j++)
		{
			hindernis.draw_rot(hindernisse_x.at(j), hindernisse_y.at(j), 8,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
			);
		}

		//Aktuelles Hindernis mit Boom
		hindernis.draw_rot(pos_x2, pos_y2, 8,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
		);

		//Erscheinen bei Crash
		boom.draw_rot(pos_x3, pos_y3, 100, 0, 0.5, 0.5, 0.5, 0.5);

		/*
		auto g2 = (gravity * 1000000000000.0).log();

		Vektor2d rose(50.0, 50.0);
		auto g = rose - g2;
		auto s = rose + speed * 1000.0;

		graphics().draw_rect(20, 20, 20, 10, Gosu::Color::WHITE, -100);

		for (auto planet : planets) {
		planet.draw();
		}
		*/
	}

	double rot1 = 0.0;
	double rot2 = 0.0;
	double poss = 0.0;
	double ypos1_vorher = 0.0;
	double ypos2_vorher = 0.0;
	//Vektor2d pos, speed, gravity;
	double speedy = 0;
	Funktionen abstand;
	std::vector<Planet> planets;


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		//cout << pos_x1 << "    " << pos_x2 << "       " << abstand_x << "    " << abstand_y << "   " << halbe_x1 << " Hello  " << halbe_x2 << endl;
		//abstaende1_x.clear();
		//abstaende1_y.clear();
		//abstaende1.clear();
		
		double abstand1_x, abstand1_y, abstand1, abstand2_x, abstand2_y, abstand2, tmp_abstand, abstand_x, abstand_y, hindernis1_nr, hindernis2_nr;
		abstand1_x = abstand1_y = abstand1 = abstand2_x = abstand2_y = abstand2 = tmp_abstand = abstand_x = abstand_y = hindernis1_nr = hindernis2_nr = 999999;
		
		for (size_t m = 0; m < anz_hindernisse; m++)
		{
			abstand_x = abstand.get_abstand_x(player1.width(), hindernis.width(), pos_x1, hindernisse_x.at(m));
			abstand_y = abstand.get_abstand_y(player1.height(), hindernis.height(), pos_y1, hindernisse_y.at(m));
			tmp_abstand = sqrt(abstand_x * abstand_x + abstand_y * abstand_y);
			if (tmp_abstand <= abstand1 && hindernisse_y.at(m) >= (pos_y1 - player1.height()*0.5))
			{
				abstand1 = tmp_abstand;
				abstand1_x = abstand_x;
				abstand1_y = abstand_y;
				hindernis1_nr = m;
			}
			abstand_x = abstand.get_abstand_x(player2.width(), hindernis.width(), pos_x4, hindernisse_x.at(m));
			abstand_y = abstand.get_abstand_y(player2.height(), hindernis.height(), pos_y4, hindernisse_y.at(m));
			tmp_abstand = sqrt(abstand_x * abstand_x + abstand_y * abstand_y);
			if (tmp_abstand <= abstand1 && hindernisse_y.at(m) >= (pos_y4 - player2.height()*0.5))
			{
				abstand2 = tmp_abstand;
				abstand2_x = abstand_x;
				abstand2_y = abstand_y;
				hindernis2_nr = m;
			}
		}

			cout << abstand2_y << "  " << hindernisse_y.at(0) << endl;
			//cout << abstaende1_x.at0) << endl;
			
			//Nach links fahren //Spieler 1
			if ((input().down(Gosu::KB_LEFT)) && ((abstand1) >= 1.0) && (pos_y2 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) >= 8.0)
					pos_x1 -= 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash)
				if (rot1 < 45)
					rot1 += 1;
			}
			else if ((input().down(Gosu::KB_LEFT)) && ((abstand1) <= 2.0) && (pos_x1 <= pos_x2))
			{
				pos_x1 -= 2;	//Bei Crash von rechts -> nach links fahren möglich
				if (rot1 < 45)
					rot1 += 1;
			}
			pos_x1 -= vel1 * sin(rot1*PI / 180);

			//Spieler 2
			if ((input().down(Gosu::KB_A)) && ((abstand2) >= 1.0) && (pos_y4 != ypos2_vorher))
			{
				ypos2_vorher = pos_y2;
				if (vel2 * sin(rot2*PI / 180) >= 8.0)
					pos_x4 -= 8.0;
				else
					pos_x4 -= vel2 * sin(rot2*PI / 180);	//im Normalfall (kein Crash)
				if (rot2 < 45)
					rot2 += 1;
			}
			else if ((input().down(Gosu::KB_A)) && ((abstand2) <= 2.0) && (pos_x4 <= pos_x2))
			{
				pos_x4 -= 2;	//Bei Crash von rechts -> nach links fahren möglich
				if (rot2 < 45)
					rot2 += 1;
			}
			pos_x4 -= vel2 * sin(rot2*PI / 180);

			//Nach rechts fahren Spieler 1
			if ((input().down(Gosu::KB_RIGHT)) && ((abstand1) >= 1.0) && (pos_y2 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) <= -8.0)
					pos_x1 += 8.0;
				else
					pos_x1 += vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash)
				if (rot1 > -45)
					rot1 -= 1;
			}
			else if ((input().down(Gosu::KB_RIGHT)) && ((abstand1) <= 2.0) && (pos_x1 >= pos_x2))
			{
				pos_x1 += 2;	//Bei Crash von links -> nach rechts fahren möglich
				if (rot1 > -45)
					rot1 -= 1;
			}

			//Spieler 2
			if ((input().down(Gosu::KB_D)) && ((abstand2) >= 1.0) && (pos_y4 != ypos2_vorher))
			{
				ypos2_vorher = pos_y2;
				if (vel2 * sin(rot2*PI / 180) <= -8.0)
					pos_x4 += 8.0;
				else
					pos_x4 += vel2 * sin(rot2*PI / 180);	//im Normalfall (kein Crash)
				if (rot2 > -45)
					rot2 -= 1;
			}
			else if ((input().down(Gosu::KB_D)) && ((abstand2) <= 2.0) && (pos_x4 >= pos_x2))
			{
				pos_x4 += 2;	//Bei Crash von links -> nach rechts fahren möglich
				if (rot2 > -45)
					rot2 -= 1;
			}
			//cout << pos_y1 << "  " << pos_y4 << "   " << vel1 << "  " << vel2 << "  " << vel1 * sin(rot1*PI / 180) << endl;
			//cout << abstaende_x.at(1) << "  " << abstaende_y.at(1) << "  " << abstaende.at(0) << "  " << pos_y1 << "   " << delta_pos << "   " << poss << endl;	//nur zur Simulation

			//Funktionen beim Beschleunigen Spieler 1
			//if ((input().down(Gosu::KB_DOWN)) && (!(input().down(Gosu::KB_UP))) && (pos_y1 == graphics().height() / 2.0) && (((abstand) >= 1.0) || (abstand_x > 0.0)))
			if ((input().down(Gosu::KB_DOWN)) && (!(input().down(Gosu::KB_UP))) && (((abstand1) >= 1.0) || (abstand1_x > 0.0)))
			{
				if (vel1 <= 0)
					vel1 = 0.05;
				if (vel1 < 20)	//Maximalgeschwindigkeitsänderung: 20
				{
					vel1 += 0.1;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.1
				}
				if ((pos_y1 > pos_y4) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
					pos_y4 -= vel1 * cos(rot2*PI / 180);
				}
				else if ((pos_y1 < pos_y4) || ((pos_y1 == pos_y4) && (vel1 < vel2)))
				{
					pos_y1 += vel1 * cos(rot1*PI / 180);
				}
				else if ((pos_y1 == pos_y4) && (vel1 == vel2))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
				}
				//poss += vel1 * cos(rot*PI / 180);
				for (int j = 0; j < hindernisse_x.size(); j++)
				{
					hindernisse_y.at(j) -= vel1 * cos(rot1*PI / 180);
				}

			}
			//else if ((!(input().down(Gosu::KB_DOWN)) && (vel1 > 0)) && (abstand > 0) && (pos_y1 == graphics().height() / 2.0))
			else if ((!(input().down(Gosu::KB_DOWN)) && (vel1 > 0)) && (abstand1 > 0))
			{
				vel1 -= 0.2;	//leichtes Bremsen bei Inaktivität
				if (pos_y1 > pos_y4 || ((pos_y1 == pos_y4) && (vel1 > vel2)))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
					pos_y4 -= vel1 * cos(rot2*PI / 180);
				}
				else if ((pos_y1 < pos_y4) || ((pos_y1 == pos_y4) && (vel1 < vel2)))
				{
					pos_y1 += vel1 * cos(rot1*PI / 180);
				}
				else if ((pos_y1 == pos_y4) && (vel1 == vel2))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
				}
				for (int j = 0; j < hindernisse_x.size(); j++)
				{
					hindernisse_y.at(j) -= vel1 * cos(rot1*PI / 180);
				}
			}
			else if ((pos_y1 > pos_y4) && (input().down(Gosu::KB_DOWN)) && (!(input().down(Gosu::KB_UP))) && (pos_y1 != graphics().height() / 2.0) && ((abstand1) >= 1.0) || (abstand1_x > 0.0))
			{
				//pos_y1 = graphics().height() / 2.0;
			}
			else if (vel1 < 0)
			{
				vel1 = 0;
			}
			//else if ((input().down(Gosu::KB_DOWN)) && (pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts )) {
			if ((pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts)) {
				if (vel1 > 4) {
					vel1 -= 0.25; //Bremsen auf Gras bis Minimalspeed 1
				}

			}

			//Freistellen des Autos zurück bei Crash
			if ((input().down(Gosu::KB_UP)) && ((abstand1) < 1.0) && (abstand1_x == 0.0))
				pos_y1 -= 100;

			//Bremsen durch Hoch-Taste
			if ((input().down(Gosu::KB_UP)) && (vel1 > 0))
				vel1 -= 1.0;

			//Schließen des Spiels mit ESC
			if (input().down(Gosu::KB_ESCAPE))
				close();

			// Crash-Erkennung (Abstand <=1.0) -> BOOM wird an Unfallstelle geheftet.
			for (size_t i = 0; i < hindernisse_x.size(); i++)
			{

				if ((abstand1) <= 1.0 && (pos_x3 != graphics().width() / 2.0))
				{
					vel1 = 0.0;
					rot1 = 0;
					pos_y3 = pos_y1 + 20;
					pos_x3 = hindernisse_x.at(hindernis1_nr);
				};
			}

			//Spieler 2
			//if ((input().down(Gosu::KB_S)) && (!(input().down(Gosu::KB_W))) && (pos_y4 == graphics().height() / 2.0) && (((abstand) >= 1.0) || (abstand_x > 0.0)))
			if ((input().down(Gosu::KB_S)) && (!(input().down(Gosu::KB_W))) && (((abstand2) >= 1.0) || (abstand2_x > 0.0)))
			{
				if (vel2 <= 0)
					vel2 = 0.05;
				if (vel2 < 20)	//Maximalgeschwindigkeitsänderung: 20
				{
					vel2 += 0.1;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.05
				}
				if ((pos_y4 > pos_y1) || ((pos_y1 == pos_y4) && (vel1 < vel2)))
				{
					pos_y2 -= vel2 * cos(rot2*PI / 180);
					pos_y3 -= vel2 * cos(rot2*PI / 180);
					pos_y1 -= vel2 * cos(rot2*PI / 180);
				}
				else if ((pos_y4 < pos_y1) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
				{
					pos_y4 += vel2 * cos(rot2*PI / 180);
				}
				else if ((pos_y1 == pos_y4) && (vel1 == vel2))
				{
					pos_y2 -= vel2 * cos(rot2*PI / 180);
					pos_y3 -= vel2 * cos(rot2*PI / 180);
				}
				//poss += vel1 * cos(rot*PI / 180);
				for (int j = 0; j < hindernisse_x.size(); j++)
				{
					hindernisse_y.at(j) -= vel2 * cos(rot2*PI / 180);
				}

			}
			//else if ((!(input().down(Gosu::KB_S)) && (vel2 > 0)) && (abstand > 0) && (pos_y4 == graphics().height() / 2.0))
			else if ((!(input().down(Gosu::KB_S)) && (vel2 > 0)) && (abstand2 > 0))
			{
				vel2 -= 0.2;	//leichtes Bremsen bei Inaktivität
				if ((pos_y4 > pos_y1) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
				{
					pos_y2 -= vel2 * cos(rot2*PI / 180);
					pos_y3 -= vel2 * cos(rot2*PI / 180);
					pos_y1 -= vel2 * cos(rot2*PI / 180);
				}
				else if ((pos_y4 < pos_y1) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
				{
					pos_y4 += vel2 * cos(rot2*PI / 180);
				}
				else if (pos_y1 == pos_y4 && (vel1 == vel2))
				{
					pos_y2 -= vel2 * cos(rot2*PI / 180);
					pos_y3 -= vel2 * cos(rot2*PI / 180);
				}
				for (int j = 0; j < hindernisse_x.size(); j++)
				{
					hindernisse_y.at(j) -= vel2 * cos(rot2*PI / 180);
				}
			}
			else if ((pos_y1 < pos_y4) && (input().down(Gosu::KB_S)) && (!(input().down(Gosu::KB_W))) && (pos_y4 != graphics().height() / 2.0) && ((abstand2) >= 1.0) || (abstand1_x > 0.0))
			{
				//pos_y4 = graphics().height() / 2.0;
			}
			else if (vel2 < 0)
			{
				vel2 = 0;
			}
			//else if ((input().down(Gosu::KB_DOWN)) && (pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts )) {
			if ((pos_x4 < posX_Streckenrand_links || pos_x4 > posX_Streckenrand_rechts)) {
				if (vel2 > 4) {
					vel2 -= 0.25; //Bremsen auf Gras bis Minimalspeed 1
				}

			}

			//Freistellen des Autos zurück bei Crash
			if ((input().down(Gosu::KB_W)) && ((abstand2) < 1.0) && (abstand2_x == 0.0))
				pos_y4 -= 100;

			//Bremsen durch Hoch-Taste
			if ((input().down(Gosu::KB_W)) && (vel2 > 0))
				vel2 -= 1.0;

			// Crash-Erkennung (Abstand <=1.0) -> BOOM wird an Unfallstelle geheftet.
			for (size_t i = 0; i < hindernisse_x.size(); i++)
			{

				if ((abstand2) <= 1.0 && (pos_x3 != graphics().width() / 2.0))
				{
					vel2 = 0.0;
					rot2 = 0;
					pos_y3 = pos_y4 + 20;
					pos_x3 = hindernisse_x.at(hindernis2_nr);
				};
			}


	}
};



// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();		//Aufruf des Spielefensters
}


/*
Verbesserungsvorschläge und ToDo-Liste:
- Beim Lenken soll sich das Auto drehen -> max. Winkel ca. 90° (Mike) erldeigt, aber anpassungswürdig
- Wenn Auto rechts oder links aus dem Bildschirm fährt -> Respawn mit v=0 (Mike/Patrick)
- Crash-Erkennung bei allen Hindernissen (Mike) erledigt, aber anpassungswürdig
- Mehr Hindernisse einbauen (Mike)
- Ziellinie mit Feuerwerk :D
- Streckenbegrenzung generieren (Patrick):
	- man soll erkennen können wann das Auto außerhalb und wann innerhalb ist (Patrick)
		- wenn innerhalb (Asphalt) -> normale Geschwindigkeitsverhältnisse
		- wenn außerhalb (Gras) -> schlechtere Geschwindigkeitsverhältnisse
		- außerhalb z.B. Schlammpfützen -> ganz schlechte Verhältnisse
- Multiplayer:
	- 2. Auto hinzufügen (Mike) erledigt
	- Differenz == Bildschirmhöhe zwischen beiden Autos -> Spielende (Mike)
	- Crash-Erkennung gegenseitig (Mike)
	-vorderes Auto muss bei Bildchirmmitte sein!
- Hauptmenü erstellen (Patrick):
	- Spiel variabler gestalten:
		- Einzelspieler/Mehrspieler
		- Schwierigkeit (z.B. höhere Geschwindigkeit)
		- evtl. Empfindlichkeit bei Lenkung
		- Statistik
*/