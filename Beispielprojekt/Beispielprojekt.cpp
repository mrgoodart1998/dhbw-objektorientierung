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
	double pos_x1 = graphics().width() / 2.0;
	double pos_y1 = graphics().width() / 2.0;
	double pos_x2 = graphics().width() / 2.0;
	double pos_y2 = 800.0;
	double pos_y3 = 10000000.0;
	double pos_x3 = 10000000.0;
	double delta_pos = 0;
	double vel_x, vel_y, angle1;
public:

	GameWindow()
		: Window(1000, 1000)
		, bild("Autorot.png")
		, hindernis("Autoblau.png")
		, boom("boom.png")
		// Rakete startet in der Mitte des Bildschirmes
		//, pos(graphics().width() / 2.0, graphics().height() / 2.0)

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
		bild.draw_rot(pos_x1, pos_y1, -10,
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
		hindernis.draw_rot(pos_x2, pos_y2, -10000,
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
			if (delta_pos<20)	//Maximalgeschwindigkeitsänderung: 20
			{
				delta_pos += 0.05;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.05
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
		
		//Freistellen des Autos zurück bei Crash
		if ((input().down(Gosu::KB_UP)) && ((abstand.get_abstand(bild.width(), bild.height(), hindernis.width(), hindernis.height(), pos_x1, pos_y1, pos_x2, pos_y2)) < 1.0))
			pos_y1 -= 5;	
		
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
	- man soll erkennen können wann das Auto außerhalb und wann innerhalb ist (Patrick)
		- wenn innerhalb (Asphalt) -> normale Geschwindigkeitsverhältnisse
		- wenn außerhalb (Gras) -> schlechtere Geschwindigkeitsverhältnisse
			- außerhalb z.B. Schlammpfützen -> ganz schlechte Verhältnisse
- Multiplayer:
	- 2. Auto hinzufügen (Mike)
	- Differenz == Bildschirmhöhe zwischen beiden Autos -> Spielende (Mike)
	- Crash-Erkennung gegenseitig (Mike)
- Hauptmenü erstellen (Patrick):
	- Spiel variabler gestalten:
		- Einzelspieler/Mehrspieler
		- Schwirigkeit (z.B. höhere Geschwindigkeit)
		- evtl. Empfindlichkeit bei Lenkung
	- Statistik
*/