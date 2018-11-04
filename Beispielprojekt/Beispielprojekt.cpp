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
	Gosu::Image rasen;
	//Gosu::Image start;
	//Gosu::Image ziel;
	double pos_x1 = graphics().width() / 3.0;
	double pos_y1 = graphics().width() / 2.0;
	double pos_x2 = graphics().width() / 2.0;
	double pos_y2 = 0; // graphics().width() / 2.0;
	double pos_y3 = 10000000.0;
	double pos_x3 = 10000000.0;
	double delta_pos = 0;
	double vel_x, vel_y, angle1;
	double posX_Streckenrand_links = 0;
	double posX_Streckenrand_rechts = 800;

// Strecke
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
		, bild("Autorot.png")
		, hindernis("Autoblau.png")
		, boom("boom.png")
		, rasen("stueck_rasen.png")
		//, start("start.png")
		//, bahn("stueck_bahn.png")
		// Rakete startet in der Mitte des Bildschirmes
		//, pos(graphics().width() / 2.0, graphics().height() / 2.0)		
	{
		vel_x = vel_y = angle1 = 0;
		set_caption("Autorennspieleee");

		double x_strecke = 500;
		double y_strecke = 0;

	
		for (int i = 0; i < c_ANZAHL_STRECKENSTUECKE; i++) {
			
			if ( (i > 0) && (i < 21) ) {
				x_strecke += 0;
			}
			else if (i > 20 && (i < 56)) {
				x_strecke += 2;				
			}
			else if ((i > 55) && (i < 91)) {
				x_strecke -= 7;				
			}
			else if ((i > 90) && (i < 121)){
				x_strecke += 3;				
			}
			else if ((i > 120) && (i < 151)){
				x_strecke += 4;				
			}
			else if ((i > 150) && (i < 176)){
				x_strecke -= 5;				
			}
			else if ((i > 175) && (i < 201)){
				x_strecke += 8;				
			}
			else if ((i > 200) ){
				x_strecke -= 1;				
			}

			streckenstück_x.push_back(x_strecke);
			streckenstück_y.push_back(y_strecke);			
			y_strecke += c_RASEN_BREITE;
		}
		//streckenstück_idx = 0;
	}


	//double x = 0;
	//double y = 0;

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		// Strecke		
		int rasenOffsetY = (-1*(pos_y2) ) / c_RASEN_BREITE;		
		// linker Rasen
		for (int n = rasenOffsetY; n < rasenOffsetY+ c_WINDOW_HIGHT; n++) { // Nur Streckenstücke, ide im Window liegen			
			// prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE-1) || n < 0 ) {
				break;
			}
			rasen.draw_rot(pos_x2 - streckenstück_x[n], pos_y2 + streckenstück_y[n], 0, 0, 0.5, 0.5, 0.5);
		}
		// Rechter Rasen
		for (int n = rasenOffsetY; n < rasenOffsetY + c_WINDOW_HIGHT; n++) { // Nur Streckenstücke, ide im Window liegen
			// prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE - 1) || n < 0) {
				break;
			}
			rasen.draw_rot(pos_x2 + 1000 - streckenstück_x[n], pos_y2 + streckenstück_y[n], 0, 180, 0.5, 0.5, 0.5);
		}
		// Nur Streckenstücke, ide im Window liegen
		for (int n = rasenOffsetY; n < rasenOffsetY + c_WINDOW_HIGHT; n++) {
			// prevent access violation
			if (n >= (c_ANZAHL_STRECKENSTUECKE - 1) || n < 0) {
				break;
			}
			double streckenstück_y_cur  = -1 * streckenstück_y[n];
			double streckenstück_y_next = -1 * streckenstück_y[n+1];
			double offsetY = 420;
			if ( (pos_y2 <= (streckenstück_y_cur + offsetY)) && (pos_y2 >= (streckenstück_y_next + offsetY))) {
				
				posX_Streckenrand_links = (-1*streckenstück_x[n]) + 770;

				graphics().draw_line(posX_Streckenrand_links, 0, Gosu::Color::RED, posX_Streckenrand_links, c_WINDOW_HIGHT, Gosu::Color::RED,0);

				posX_Streckenrand_rechts = (-1*streckenstück_x[n]) + 1230;

				graphics().draw_line(posX_Streckenrand_rechts, 0, Gosu::Color::RED, posX_Streckenrand_rechts, c_WINDOW_HIGHT, Gosu::Color::RED, 0);

			}
		}
		 
		//Positionen der Hindernisse random bestimmen
		do
		{
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_y.push_back(Gosu::random(800, 5000));
			hindernisse_y.push_back(Gosu::random(5000, 10000));
			hindernisse_y.push_back(Gosu::random(10000, 15000));
			hindernisse_y.push_back(Gosu::random(15000, 20000));
			i++;
		} while (i <= 20);






		
		//Positionen der Hindernisse random bestimmen
		do
		{
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_x.push_back(Gosu::random(1, graphics().width()));
			hindernisse_y.push_back(Gosu::random(800, 5000));
			hindernisse_y.push_back(Gosu::random(5000, 10000));
			hindernisse_y.push_back(Gosu::random(10000, 15000));
			hindernisse_y.push_back(Gosu::random(15000, 20000));
			i++;
		} while (i <= 20);

		//Gosu::random();

		//Spieler 1 (rotes Auto)
		bild.draw_rot(pos_x1, pos_y1, 10,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
		);
		

		//Zeichnen von Hindernissen
		for (int j = 0; j < 80; j++)
		{
			hindernis.draw_rot(hindernisse_x.at(j), hindernisse_y.at(j), -10000,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
			);
		}

		//Aktuelles Hindernis mit Boom
		hindernis.draw_rot(pos_x2, pos_y2, 10,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
		);

		//Erscheinen bei Crash
		boom.draw_rot(pos_x3, pos_y3, 10, 0, 0.5, 0.5, 0.5, 0.5);

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

	double rot = 0.0;
	double poss = 0.0;
	//Vektor2d pos, speed, gravity;
	double speedy = 0;
	Funktionen abstand;
	std::vector<Planet> planets;


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//Nach links fahren
		if ((input().down(Gosu::KB_LEFT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			pos_x1 -= 3;	//im Normalfall (kein Crash)
			rot -= 10;
		}
		else if ((input().down(Gosu::KB_LEFT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 2.0) && (pos_x1 <= pos_x2))
		{
			pos_x1 -= 3;	//Bei Crash von rechts -> nach links fahren möglich
		}

		//Nach rechts fahren
		if ((input().down(Gosu::KB_RIGHT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			pos_x1 += 3;	//im Normalfall (kein Crash)
			rot += 10;
		}	
		else if ((input().down(Gosu::KB_RIGHT)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 2.0) && (pos_x1 >= pos_x2))
		{
			pos_x1 += 3;	//Bei Crash von links -> nach rechts fahren möglich
		}

		cout << delta_pos << "   " << poss << endl;	//nur zur Simulation

		//Funktionen beim Beschleunigen
		if ((input().down(Gosu::KB_DOWN)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) >= 1.0))
		{
			if (delta_pos<30)	//Maximalgeschwindigkeitsänderung: 20
			{
				delta_pos += 0.1;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.1
			}
			pos_y2 -= delta_pos;
			pos_y3 -= delta_pos;
			poss += delta_pos;
			
			for (int j = 0; j < hindernisse_x.size(); j++)
			{
				hindernisse_y.at(j) -= delta_pos;
			}
		}
		else if (!(input().down(Gosu::KB_DOWN))&&(delta_pos>0))	
		{
			delta_pos -= 0.2;	//leichtes Bremsen bei Inaktivität
			pos_y2 -= delta_pos;
			pos_y3 -= delta_pos;
			poss += delta_pos;	//nur zur Simulation

			for (int j = 0; j < hindernisse_x.size(); j++)
			{
				hindernisse_y.at(j) -= delta_pos;
			}
		}
		//else if ((input().down(Gosu::KB_DOWN)) && (pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts )) {
		if ((pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts)) {
			if (delta_pos > 4) {
				delta_pos -= 0.5; //Bremsen auf Gras bis Minimalspeed 1
			}
					
		}
		//Freistellen des Autos zurück bei Crash
		if ((input().down(Gosu::KB_UP)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) < 1.0))
			pos_y1 -= 15;	
		
		//Bremsen durch Hoch-Taste
		if ((input().down(Gosu::KB_UP)) && (delta_pos > 0))		
			delta_pos -= 1.0;

		//Schließen des Spiels mit ESC
		if (input().down(Gosu::KB_ESCAPE))
			close();

		// Crash-Erkennung (Abstand <=1.0) -> BOOM wird an Unfallstelle geheftet.
		if ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) <= 1.0 && (pos_x3 != graphics().width() / 2.0))
		{
			pos_y3 = pos_y1 + 20;
			pos_x3 = graphics().width() / 2.0;
		};
		//
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
- Beim Lenken soll sich das Auto drehen -> max. Winkel ca. 90° (Mike)
- Wenn Auto rechts oder links aus dem Bildschirm fährt -> Respawn mit v=0 (Mike/Patrick)
- Crash-Erkennung bei allen Hindernissen (Mike)
- Mehr Hindernisse einbauen (Mike)
- Ziellinie mit Feierwerk :D
- Streckenbegrenzung generieren (Patrick):
	- man soll erkennen können wann das Auto außerhalb und wann innerhalb ist (Patrick) ERLEDIGT
		- wenn innerhalb (Asphalt) -> normale Geschwindigkeitsverhältnisse ERLEDIGT
		- wenn außerhalb (Gras) -> schlechtere Geschwindigkeitsverhältnisse ERLEDIGT
			- außerhalb z.B. Schlammpfützen -> ganz schlechte Verhältnisse
- Multiplayer:
	- 2. Auto hinzufügen (Mike)
	- Differenz == Bildschirmhöhe zwischen beiden Autos -> Spielende (Mike)
	- Crash-Erkennung gegenseitig (Mike)
- Hauptmenü erstellen (Patrick):
	- Spiel variabler gestalten:
		- Einzelspieler/Mehrspieler
		- Schwierigkeit (z.B. höhere Geschwindigkeit)
		- evtl. Empfindlichkeit bei Lenkung
	- Statistik
*/