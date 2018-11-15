#include "pch.h"
#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "ctime"
//#include<Windows.h>
//#pragma comment(lib, "winmm.lib")

#include "Funktionen.h"
#include <math.h>
#include <stdlib.h>
#define PI 3.14159265
using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

class hauptmenue : public Gosu::Window
{
	Gosu::Image hauptmen;
	Gosu::Image highscore;
	Gosu::Font Textfeld, Highscore1, Highscore2, Highscore3;

public:
	bool *two_player_active = false;
	bool show_highscore = false;
	bool *exit_all = false;
	fstream f1;
	vector<string> list1;
	int number = 0;

	hauptmenue(bool *two_player_act, bool *exit_now)
		: Window(1000, 1000)
		, hauptmen("hauptmenue_resize.png")
		, highscore("highscore.jpg")
		, Textfeld(20)
		, Highscore1(20)
		, Highscore2(20)
		, Highscore3(20)



	{
		show_highscore = false;
		two_player_active = two_player_act;
		exit_all = exit_now;

	}

	void draw() override
	{
		if (!show_highscore) {
			hauptmen.draw_rot(500, 500, 0,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5); // Position der "Mitte"
		}

		if (show_highscore)
		{
			highscore.draw_rot(500, 500, 0,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5); // Position der "Mitte"

			Highscore1.draw_rel("1. Platz:  " + list1.at(number - 1), 500, (900 - 3 * 75), 10, 0.5, 0.5, 1.0, 1.0);
			Highscore2.draw_rel("2. Platz:  " + list1.at(number - 2), 500, (900 - 2 * 75), 10, 0.5, 0.5, 1.0, 1.0);
			Highscore3.draw_rel("3. Platz:  " + list1.at(number - 3), 500, (900 - 1 * 75), 10, 0.5, 0.5, 1.0, 1.0);
		}

	}



	void update() override {
		if (input().down(Gosu::KB_A))
		{
			close();
		}
		else if (input().down(Gosu::KB_B)) {
			*two_player_active = true;
			close();
		}
		else if (input().down(Gosu::KB_X)) {
			show_highscore = true;
			if (show_highscore) {
				int number_of_lines = 0;
				string h;
				string line;
				ifstream myfile("highscore.txt");
				if (myfile.is_open()) {
					while (!myfile.eof()) {
						getline(myfile, line);
						list1.push_back(line);
						number_of_lines++;
						//cout << line << endl;
					}
					number = number_of_lines;
					//cout << number << endl;
					myfile.close();
				}
				bool sortiert = TRUE;
				while (sortiert) {
					sortiert = FALSE;
					for (int i = 0; i < number - 1; i++)
						if (list1.at(i) > list1.at(i + 1)) {
							h = list1.at(i);
							list1.at(i) = list1.at(i + 1);
							list1.at(i + 1) = h;
							sortiert = TRUE;
						}
				}
			}
		}
		else if (input().down(Gosu::KB_ESCAPE)) {
			show_highscore = false;
		}
		else if (input().down(Gosu::KB_E)) {
			*exit_all = true;
			close();
		}
	};

};

class GameWindow : public Gosu::Window
{
	Gosu::Image player1, player2;
	Gosu::Image hindernis;
	Gosu::Image boom;
	Gosu::Image rasen;
	Gosu::Image ziel;
	Gosu::Image highscore;
	Gosu::Font font1, font2, font3, info1, info2, info3, info4, start_in, sieger;
	//Member Variablen
	double pos_x1 = graphics().width() / 2.0;
	double pos_y1 = graphics().width() / 2.0;
	double pos_x2 = graphics().width() / 2.0;
	double pos_y2 = 0; // graphics().width() / 2.0;
	double pos_y3 = 10000000.0;
	double pos_x3 = 10000000.0;
	double pos_x4 = (graphics().width() / 2.0) + 200;
	double pos_y4 = graphics().width() / 2.0;
	double delta_pos = 0;
	double vel1 = 0;
	double vel2 = 0;
	double vel_x, vel_y, angle1;
	//Strecke
	double posX_Streckenrand_links = 0;
	double posX_Streckenrand_rechts = 800;
	double posX_Bildschirm_links = 0;
	double posX_Bildschirm_rechts = 800;
	vector<double> streckenstück_x;
	vector<double> streckenstück_y;
	const int c_ANZAHL_STRECKENSTUECKE = 600;
	const int c_WINDOW_WIDTH = 1000;
	const int c_WINDOW_HIGHT = 1000;
	bool two_player_mod;
	const int c_RASEN_BREITE = 24;
	int time_sec = 0;
	int time_60hz_tick = 0;
	int time_60hz_tick2 = 0;
	bool crash_happen = false;
	int crash_time = 0;
	double delta_y = 24.5;
	double delta_bahn_y = 300;
	double rot1 = 0.0;
	double rot2 = 0.0;
	double poss = 0.0;
	double ypos1_vorher = 0.0;
	double ypos2_vorher = 0.0;
	double ypos12_vorher = 0.0;
	double ypos22_vorher = 0.0;
	double speedy = 0;
	Funktionen abstand;
	int finish_zeit_auto1 = 0;
	int finish_zeit_auto2 = 0;
	double delta_abstand_autos_x1 = 0.0;
	double delta_abstand_autos_y1 = 0.0;
	double delta_abstand_autos_x2 = 0.0;
	double delta_abstand_autos_y2 = 0.0;
	double delta_abstand_autos_x = 0.0;
	double delta_abstand_autos_y = 0.0;
	double vel_auto1_insg = 0.0;
	double vel_auto2_insg = 0.0;
	double weg_auto1 = 0.0;
	double weg_auto2 = 0.0;
	double score_auto1 = 0.0;
	double score_auto2 = 0.0;
	bool exit_screen_time = false;
	int time_sec_exit = 0;
	int *time_sec_fin_ex;
	double pos_x_boom = 10000.0;
	double pos_y_boom = 10000.0;
	string name_eingeben = "Name des Spielers:  ";
	string info = "(Mit Enter bestätigen)";
	string name;
	string info_1, info_2, info_3, info_4, sieger_text;
	string startin = "START in 3...";
	int q = 2;
	bool los_gehts = false;
	string name1, name2;
	bool namenende = false;
	vector<double> hindernisse_x, hindernisse_y, abstaende1_x, abstaende1_y, abstaende1, abstaende2_x, abstaende2_y, abstaende2;
	int anz_hindernisse = 5;
	int i = 0;
	int z = 0;
	int anz_streckenbereiche = 14;
	bool ziel_erreicht = false;
	bool player1_sieger = false;
	bool player2_sieger = false;
	double start_time = 0;
	bool beendet_ESC = false;
	bool endeee = false;
	int* score1 = 0;
	bool gecrashed = false;

public:

	GameWindow(bool two_player_active, int *auto_score1, int *time_sec_fin)
		: Window(1000, 1000)
		, player1("Autorot.png")
		, player2("Autogruen.png")
		, hindernis("Autoblau.png")
		, boom("boom.png")
		, rasen("stueck_rasen.png")
		, ziel("Ziel_Finish.jpg")
		, highscore("highscore.jpg")
		, font1(20)
		, font2(20)
		, font3(20)
		, info1(20)
		, info2(20)
		, info3(20)
		, info4(20)
		, start_in(50)
		, sieger(30)
	{
		two_player_mod = two_player_active;
		score1 = auto_score1;
		time_sec_fin_ex = time_sec_fin;
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
				x_strecke += 6;
			}
			else if ((i > 200) && (i < 250)) {
				x_strecke -= 5;
			}
			else if ((i > 251) && (i < 275)) {
				x_strecke += 6;
			}
			else if ((i > 276) && (i < 310)) {
				x_strecke -= 5;
			}
			else if ((i > 311) && (i < 333)) {
				x_strecke += 12;
			}
			else if ((i > 334) && (i < 346)) {
				x_strecke -= 12;
			}
			else if ((i > 347) && (i < 369)) {
				x_strecke += 16;
			}
			else if ((i > 370) && (i < 392)) {
				x_strecke -= 17;
			}
			else if ((i > 393) && (i < 412)) {
				x_strecke += 18;
			}
			else if ((i > 413) && (i < 430)) {
				x_strecke -= 8;
			}
			else if ((i > 431) && (i < 450)) {
				x_strecke += 0;
			}
			else if ((i > 500) && (i < 521)) {
				x_strecke += 9;
			}
			else if ((i > 522) && (i < 544)) {
				x_strecke -= 13;
			}
			else if ((i > 545)) {
				x_strecke += 0;
			}

			streckenstück_x.push_back(x_strecke);
			streckenstück_y.push_back(y_strecke);
			y_strecke += c_RASEN_BREITE;
		}

		//Positionen der Hindernisse random bestimmen
		hindernisse_x.clear();
		hindernisse_y.clear();
		int faktor = 0;

		do
		{	
			faktor += 1000;
			hindernisse_x.push_back(rand() % graphics().width());
			hindernisse_x.push_back(rand() % graphics().width());
			hindernisse_x.push_back(rand() % graphics().width());
			hindernisse_x.push_back(rand() % graphics().width());
			hindernisse_x.push_back(rand() % graphics().width());
			hindernisse_y.push_back(rand() % 1000 +faktor);
			hindernisse_y.push_back(rand() % 1000 +faktor);
			hindernisse_y.push_back(rand() % 1000 +faktor);
			hindernisse_y.push_back(rand() % 1000 +faktor);
			hindernisse_y.push_back(rand() % 1000 +faktor);
			
			z++;
		} while (z < anz_streckenbereiche);

		//Info am Startbeginn
		if (two_player_mod)
		{
			info_1 = "Spieler 1: rotes Auto, Pfeiltasten; Spieler 2: gelbes Auto, W/A/S/D";
			info_2 = "Beschleunigung: Pfeil runter/S   Bremsen: Pfeil hoch/W   Lenken: Pfeil links, rechts/A, D";
			info_3 = "Spielende: Hinterer Spieler aus Sichtfeld oder Ziellinie erreicht oder ESC";
			info_4 = "Have fun!";
		}
		else if (!two_player_mod)
		{
			info_1 = "Spieler 1: rotes Auto, Steuerung mit Pfeiltasten";
			info_2 = "Beschleunigung: Pfeil runter   Bremsen: Pfeil hoch   Lenken: Pfeil links/rechts";
			info_3 = "Spielende: Ziellinie erreicht oder ESC";
			info_4 = "Have fun!";
		}
	}

	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		
		// Strecke		
		//int pos_car_all = pos_y2
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
			double offsetY = 500;
			if ((pos_y2 <= (streckenstück_y_cur + offsetY)) && (pos_y2 >= (streckenstück_y_next + offsetY))) {

				posX_Streckenrand_links = (-1 * streckenstück_x[n]) + 770;

				graphics().draw_line(posX_Streckenrand_links, 0, Gosu::Color::RED, posX_Streckenrand_links, c_WINDOW_HIGHT, Gosu::Color::RED, 0);

				posX_Streckenrand_rechts = (-1 * streckenstück_x[n]) + 1230;

				graphics().draw_line(posX_Streckenrand_rechts, 0, Gosu::Color::RED, posX_Streckenrand_rechts, c_WINDOW_HIGHT, Gosu::Color::RED, 0);

			}
		}

		// Bildschirmbegrenzung in links
		for (int n = 1; n < c_WINDOW_WIDTH; n++) {

			posX_Bildschirm_links = 10;

			graphics().draw_line(posX_Bildschirm_links, 0, Gosu::Color::WHITE, posX_Bildschirm_links, c_WINDOW_HIGHT, Gosu::Color::WHITE, 0);
		}
		// Bildschirmbegrenzung in rechts
		for (int n = 1; n < c_WINDOW_WIDTH; n++) {

			posX_Bildschirm_rechts = 990;

			graphics().draw_line(posX_Bildschirm_rechts, 0, Gosu::Color::WHITE, posX_Bildschirm_rechts, c_WINDOW_HIGHT, Gosu::Color::WHITE, 0);
		}
		//Ziel bei c_Anzahl_Streckenstücke
		if ((-1)*pos_y2 > ((c_ANZAHL_STRECKENSTUECKE*c_RASEN_BREITE) - 1000))
		{
			ziel.draw_rot(500, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel.draw_rot(400, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel.draw_rot(600, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel_erreicht = true;
			bool fertig = false;
			time_sec_exit++;
			
			ziel_erreicht = true;
			if (pos_y1 > pos_y4 && !player1_sieger)
			{
				if (two_player_mod) {
					player1_sieger = true;
					sieger_text = "Sieger ist Spieler 1! Herzlichen Glueckwunsch!!!";
				}
				else if (!two_player_mod)
				{
					double end_time = clock();
					player1_sieger = true;
					double ben_Zeit = (end_time - start_time) / CLOCKS_PER_SEC;
					*time_sec_fin_ex = ben_Zeit;
					if (ben_Zeit < 300)
						*score1 = (300-ben_Zeit) * pos_y2 *(-1) /1000;
					if (ben_Zeit >= 300)
						score1 = 0;
					cout << "Startzeit: " << start_time << endl;
					cout << "Strecke: " << pos_y2 * (-1.0) << endl;
					cout << "Zeit: " << *time_sec_fin_ex << endl;
					int score = *score1;
					string score_s = to_string(score);
					string time_s = to_string(ben_Zeit);
					cout << "Score 1: " << *score1 << endl;
					cout << "Score: " << score << endl;
					sieger_text = "Ziel erreicht! Score: " +score_s ; 
				}
			}
			if (pos_y4 > pos_y4 && !player2_sieger)
			{
				if (two_player_mod)
				{
					player2_sieger = true;
					sieger_text = "Sieger ist Spieler 2! Herzlichen Glueckwunsch!!!";
				}
			}

			sieger.draw_rel(sieger_text, 500, 700, 50, 0.5, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
			//system("pause");
			

			if (time_sec_exit > 60) {
				//font1.draw_rel(name_eingeben, 300, 700, 100, 0.0, 0.5, 1.0, 1.0);
				//font2.draw_rel(info, 300, 725, 100, 0.0, 0.5, 1.0, 1.0);

				if (time_sec_exit>300)
				{
					close();
				}
			}
		}
		//Ziel bei abhängen des anderen Autos
		if (((pos_y1 - 650) > pos_y4 || (pos_y4 - 650) > pos_y1) && two_player_mod)
		{
			ziel.draw_rot(500, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel.draw_rot(400, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel.draw_rot(600, 500, 10, 0, 0.5, 0.5, 0.5);
			ziel_erreicht = true;
			time_sec_exit++;
			sieger.draw_rel(sieger_text, 500, 700, 50, 0.5, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
			if (pos_y1 > pos_y4 && !player1_sieger)
			{
				player1_sieger = true;
				sieger_text = "Sieger ist Spieler 1! Herzlichen Glueckwunsch!!!";
			}
			if (pos_y4 > pos_y1 && !player2_sieger)
			{
				player2_sieger = true;
				sieger_text = "Sieger ist Spieler 2! Herzlichen Glueckwunsch!!!";
			}



			if (time_sec_exit > 300) {
				close();
			}
		}

		//Spiel beendet durch ESC
		if (beendet_ESC)
		{
			close();
		}
		
		//Spieler 1 (rotes Auto)
		player1.draw_rot(pos_x1, pos_y1, 10,
			rot1, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
		);
		
		//Spieler 2 (gelbes Auto)
		if (two_player_mod) {
			player2.draw_rot(pos_x4, pos_y4, 10,
				rot2, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5 // Position der "Mitte"
			);
		}
		
		//Zeichnen von Hindernissen
		for (int j = 0; j < hindernisse_x.size(); j++)
		{
			hindernis.draw_rot(hindernisse_x.at(j), hindernisse_y.at(j), 8,
				0, // Rotationswinkel in Grad
				0.5, 0.5, 0.5, 0.5 // Position der "Mitte";
			);
		}
		
		//Infotext
		graphics().draw_rect(0, pos_y2 + 50, 1000, 175, Gosu::Color::BLACK, 50);
		info1.draw_rel(info_1, 250, pos_y2 + 100, 100, 0.0, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
		info2.draw_rel(info_2, 250, pos_y2 + 125, 100, 0.0, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
		info3.draw_rel(info_3, 250, pos_y2 + 150, 100, 0.0, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
		info4.draw_rel(info_4, 250, pos_y2 + 175, 100, 0.0, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
		start_in.draw_rel(startin, 500, pos_y2 + 300, 100, 0.5, 0.5, 1.0, 1.0, Gosu::Color::WHITE);
		
		if (q==2)
		{
			time_60hz_tick2++;
			if (time_60hz_tick2 == 75) {
				time_60hz_tick2 = 0;
				startin = "START in 2...";
				q--;
			}
		}

		if (q == 1)
		{
			time_60hz_tick2++;
			if (time_60hz_tick2 == 75) {
				time_60hz_tick2 = 0;
				startin = "START in 1...";
				q--;
			}
		}

		if (q == 0)
		{
			time_60hz_tick2++;
			if (time_60hz_tick2 == 75) {
				time_60hz_tick2 = 0;
				startin = "START in 0...";
				q--;
				start_time = clock();
				los_gehts = true;
			}
		}
		
		
		//Erscheinen bei Crash
		if (crash_happen) {
			boom.draw_rot(pos_x3, pos_y3, 100, 0, 0.5, 0.5, 0.5, 0.5);
			if (time_sec > crash_time + 1) {
				crash_happen = false;
			}
		}

		//Window-Begrenzung rot
		graphics().draw_rect(0, pos_y1 - 1000, 10, 10000, Gosu::Color::RED, 100);
		graphics().draw_rect(990, pos_y1 - 1000, 10, 10000, Gosu::Color::RED, 100);
		
	}

	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		// NUR ZUM TEST
		if (ziel_erreicht && !namenende)
		{

			//cout << "Hallo" << endl;
			string name_vorher = name1;
			if (input().down(Gosu::KB_A))
			{
				while (endeee==false)
				{
					
					if(input().down(Gosu::KB_X))
						endeee = true;
					cout << "WHILEEEEEEEEE" << endl;
					if (endeee == true)
						cout << "TRUUUUUUUUUUUUUUE" << endl;
					if (endeee == false)
						cout << "FAAAAAAAAAAAAAALSE" << endl;
				}
				cout << "A";
				name1 += "A";
			}
			if (input().down(Gosu::KB_B))
				if (!input().down(Gosu::KB_B))
				{
					cout << "B";
					name1 += "B";
				}
			if (input().down(Gosu::KB_C))
				if (!input().down(Gosu::KB_C))
				{
					cout << "C";
					name1 += "C";
				}
			if (input().down(Gosu::KB_D))
				name1 += "D";
			if (input().down(Gosu::KB_E))
				name1 += "E";
			if (input().down(Gosu::KB_F))
				name1 += "F";
			if (input().down(Gosu::KB_G))
				name1 += "G";
			if (input().down(Gosu::KB_H))
				name1 += "H";
			if (input().down(Gosu::KB_I))
				name1 += "I";
			if (input().down(Gosu::KB_J))
				name1 += "J";
			if (input().down(Gosu::KB_K))
				name1 += "K";
			if (input().down(Gosu::KB_L))
				name1 += "L";
			if (input().down(Gosu::KB_M))
				name1 += "M";
			if (input().down(Gosu::KB_N))
				name1 += "N";
			if (input().down(Gosu::KB_O))
				name1 += "O";
			if (input().down(Gosu::KB_P))
				name1 += "P";
			if (input().down(Gosu::KB_Q))
				name1 += "Q";
			if (input().down(Gosu::KB_R))
				name1 += "R";
			if (input().down(Gosu::KB_S))
				name1 += "S";
			if (input().down(Gosu::KB_T))
				name1 += "T";
			if (input().down(Gosu::KB_U))
				name1 += "U";
			if (input().down(Gosu::KB_V))
				name1 += "V";
			if (input().down(Gosu::KB_W))
				name1 += "W";
			if (input().down(Gosu::KB_X))
				name1 += "X";
			if (input().down(Gosu::KB_Y))
				name1 += "Y";
			if (input().down(Gosu::KB_Z))
				name1 += "Z";
			if (input().down(Gosu::KB_SPACE))
				name1 += " ";
			if (input().down(Gosu::KB_BACKSLASH))
				name1 = name_vorher;
			if (input().down(Gosu::KB_Z))
			{
				namenende = true;
				cout << name1 << endl;
			}
			
		}
		if (los_gehts && !ziel_erreicht)
		{
			
			
			time_60hz_tick++;
			if (time_60hz_tick == 35) {
				time_60hz_tick = 0;
				time_sec++;
			}

			double abstand1_x, abstand1_y, abstand1, abstand2_x, abstand2_y, abstand2, tmp_abstand, abstand_x, abstand_y, hindernis1_nr, hindernis2_nr, abstand3_x, abstand3_y, abstand3;
			abstand1_x = abstand1_y = abstand1 = abstand2_x = abstand2_y = abstand2 = tmp_abstand = abstand_x = abstand_y = hindernis1_nr = hindernis2_nr = abstand3_x = abstand3_y = abstand3 = 999999;

			for (size_t m = 0; m < hindernisse_x.size(); m++)
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
				if (two_player_mod)
				{
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
			}

			/*cout << "POS_Y1: " << pos_y1 << endl;
			cout << "POS_Y2: " << pos_y2 << endl;
			cout << "POS_Y3: " << pos_y3 << endl;
			cout << "POS_Y4: " << pos_y4 << endl;*/

			//Nach links fahren //Spieler 1
			if (!two_player_mod&&(input().down(Gosu::KB_LEFT)) && ((abstand1) >= 1.0) && (pos_y2 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) >= 8.0)
					pos_x1 -= 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash)
				if (rot1 < 45)
					rot1 += 1;
			}
			else if (!two_player_mod&&(input().down(Gosu::KB_LEFT)) && pos_y1 < (graphics().height()*0.5)) //wenn beide Autos nicht in der Mitte sind
			{
				if (vel1 * sin(rot1*PI / 180) >= 8.0)
					pos_x1 -= 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);
				if (rot1 < 45 && (ypos12_vorher < pos_y1 || ypos12_vorher == 500))
					rot1 += 1;
			}
			else if(two_player_mod&&(input().down(Gosu::KB_LEFT)) && ((abstand1) >= 1.0) && (pos_y2 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) >= 8.0)
					pos_x1 -= 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash)
				if (rot1 < 45)
					rot1 += 1;
			}
			pos_x1 -= vel1 * sin(rot1*PI / 180);

			//Spieler 2
			if (two_player_mod)
			{
				if ((input().down(Gosu::KB_A)) && ((abstand2) >= 1.0) && (pos_y4 != ypos2_vorher)) ////y2 zu y4
				{
					ypos2_vorher = pos_y2;
					if (vel2 * sin(rot2*PI / 180) >= 8.0)
						pos_x4 -= 8.0;
					else
						pos_x4 -= vel2 * sin(rot2*PI / 180);	//im Normalfall (kein Crash)
					if (rot2 < 45)
						rot2 += 1;
				}
				else if ((input().down(Gosu::KB_A)) && pos_y1 < (graphics().height()*0.5) - 0.5 && pos_y4 < (graphics().height()*0.5) - 0.5) //wenn beide Autos nicht in der Mitte sind
				{
					if (vel2 * sin(rot2*PI / 180) >= 8.0)
						pos_x4 -= 8.0;
					else
						pos_x4 -= vel2 * sin(rot2*PI / 180);
					if (rot2 < 45 && (ypos22_vorher < pos_y4 || ypos22_vorher == 500))
						rot2 += 1;
				}
				pos_x4 -= vel2 * sin(rot2*PI / 180);
			}
			//Nach rechts fahren Spieler 1
			if (!two_player_mod&&(input().down(Gosu::KB_RIGHT)) && ((abstand1) >= 1.0) && (pos_y2 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) <= -8.0)
					pos_x1 += 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash) 
				if (rot1 > -45)
					rot1 -= 1;
			}
			else if (!two_player_mod && (input().down(Gosu::KB_RIGHT)) && pos_y1 < (graphics().height()*0.5)) //wenn beide Autos nicht in der Mitte sind
			{
				if (vel1 * sin(rot1*PI / 180) >= 8.0)
					pos_x1 += 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);
				if (rot1 > -45 && (ypos12_vorher < pos_y1 || ypos12_vorher == 500))
					rot1 -= 1;
			}
			else if (two_player_mod&&(input().down(Gosu::KB_RIGHT)) && ((abstand1) >= 1.0) && (pos_y1 != ypos1_vorher))
			{
				ypos1_vorher = pos_y2;
				if (vel1 * sin(rot1*PI / 180) <= -8.0)
					pos_x1 += 8.0;
				else
					pos_x1 -= vel1 * sin(rot1*PI / 180);	//im Normalfall (kein Crash) 
				if (rot1 > -45)
					rot1 -= 1;
			}
			

			//Spieler 2
			if (two_player_mod)
			{
				if ((input().down(Gosu::KB_D)) && ((abstand2) >= 1.0) && (pos_y4 != ypos2_vorher)) //y2 zu y4
				{
					ypos2_vorher = pos_y2;
					if (vel2 * sin(rot2*PI / 180) <= -8.0)
						pos_x4 += 8.0; //- zu +
					else
						pos_x4 -= vel2 * sin(rot2*PI / 180);	//im Normalfall (kein Crash)
					if (rot2 > -45)
						rot2 -= 1;
				}
				else if ((input().down(Gosu::KB_D)) && pos_y1 < (graphics().height()*0.5) - 0.5 && pos_y4 < (graphics().height()*0.5) - 0.5) //wenn beide Autos nicht in der Mitte sind
				{
					if (vel2 * sin(rot2*PI / 180) >= 8.0)
						pos_x4 += 8.0;
					else
						pos_x4 -= vel2 * sin(rot2*PI / 180);
					if (rot2 > 45 && (ypos22_vorher < pos_y4 || ypos12_vorher == 500))
						rot2 -= 1;
				}
			}
			//cout << pos_y1 << "  " << pos_y4 << "   " << vel1 << "  " << vel2 << "  " << vel1 * sin(rot1*PI / 180) << endl;
			//cout << abstaende_x.at(1) << "  " << abstaende_y.at(1) << "  " << abstaende.at(0) << "  " << pos_y1 << "   " << delta_pos << "   " << poss << endl;	//nur zur Simulation


			//Funktionen beim Beschleunigen Spieler 1
			//if ((input().down(Gosu::KB_DOWN)) && (!(input().down(Gosu::KB_UP))) && (pos_y1 == graphics().height() / 2.0) && (((abstand) >= 1.0) || (abstand_x > 0.0)))
			if ((input().down(Gosu::KB_DOWN)) && (!(input().down(Gosu::KB_UP))) && (((abstand1) >= 1.0) || (abstand1_x > 0.0)))
			{
				vel_auto1_insg += vel1;
				if (vel1 <= 0)
					vel1 = 0.05;
				if (vel1 < 20)	//Maximalgeschwindigkeitsänderung: 20
				{
					vel1 += 0.1;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.1
				}
				if (((pos_y1 > pos_y4) || ((pos_y1 == pos_y4) && (vel1 > vel2))) && (pos_y1 >= graphics().height() / 2.0))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
					ypos22_vorher = pos_y4;
					pos_y4 -= vel1 * cos(rot2*PI / 180);
					for (int j = 0; j < hindernisse_x.size(); j++)
					{
						hindernisse_y.at(j) -= vel1 * cos(rot1*PI / 180);
					}
				}
				else if ((pos_y1 <= graphics().height() / 2.0))
				{
					pos_y1 += vel1 * cos(rot1*PI / 180);

					if (pos_y1 > (graphics().height() / 2.0) - 2 && pos_y1 < (graphics().height() / 2.0) + 2)
					{
						pos_y1 = graphics().height() / 2.0;
					}

				}
				else if ((pos_y1 < pos_y4) || ((pos_y1 == pos_y4) && (vel1 < vel2)))
				{
					ypos12_vorher = pos_y1;
					pos_y1 += vel1 * cos(rot1*PI / 180);
				}
				else if ((pos_y1 == pos_y4) && (vel1 == vel2))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
					for (int j = 0; j < hindernisse_x.size(); j++)
					{
						hindernisse_y.at(j) -= vel1 * cos(rot1*PI / 180);
					}
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
					ypos22_vorher = pos_y4;
					pos_y4 -= vel1 * cos(rot2*PI / 180);
					for (int j = 0; j < hindernisse_x.size(); j++)
					{
						hindernisse_y.at(j) -= vel1 * cos(rot1*PI / 180);
					}
				}
				else if ((pos_y1 < pos_y4) || ((pos_y1 == pos_y4) && (vel1 < vel2)))
				{
					ypos12_vorher = pos_y1;
					pos_y1 += vel1 * cos(rot1*PI / 180);
				}
				else if ((pos_y1 == pos_y4) && (vel1 == vel2))
				{
					pos_y2 -= vel1 * cos(rot1*PI / 180);
					pos_y3 -= vel1 * cos(rot1*PI / 180);
					for (int j = 0; j < hindernisse_x.size(); j++)
					{
						hindernisse_y.at(j) -= 0.5 * vel1 * cos(rot1*PI / 180);
					}
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

			//Wenn Auto 1 weiter als die Mitte des Bildschirms ist
			if (pos_y1 > graphics().height() / 2.0)
			{
				double differenz = pos_y1 - graphics().height() / 2.0;
				pos_y1 = graphics().height() / 2.0;
				pos_y2 -= differenz;
				pos_y3 -= differenz;
				ypos22_vorher = pos_y4;
				pos_y4 -= differenz;
				for (int j = 0; j < hindernisse_x.size(); j++)
				{
					hindernisse_y.at(j) -= differenz;
				}
			}
			//else if ((input().down(Gosu::KB_DOWN)) && (pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts )) {
			if ((pos_x1 < posX_Streckenrand_links || pos_x1 > posX_Streckenrand_rechts)) {
				if (vel1 > 10)
					vel1 = 10.0;
				if (vel1 > 4) {
					vel1 -= 0.25; //Bremsen auf Gras bis Minimalspeed 1
				}
			}

			//Zurücksetzen bei Verlassen des Bildschirmrands Auto 1
			if ((pos_x1 < posX_Bildschirm_links || pos_x1 > posX_Bildschirm_rechts)) {
				ypos12_vorher = pos_y1;
				pos_y1 = 200;
				pos_x1 = c_WINDOW_WIDTH / 2.0;
				rot1 = 0;
				vel1 = vel1 / 2.0;
			}
			//Freistellen des Autos zurück bei Crash
			if ((input().down(Gosu::KB_UP)) && ((abstand1) < 1.0) && (abstand1_x <= 1.0))
			{
				ypos12_vorher = pos_y1;
				pos_y1 -= 100;
			}

			//Bremsen durch Hoch-Taste
			if ((input().down(Gosu::KB_UP)) && (vel1 > 0))
				vel1 -= 1.0;

				
				
			//Schließen des Spiels mit ESC
			if (input().down(Gosu::KB_ESCAPE))
				beendet_ESC = true;
			
			// Crash-Erkennung (Abstand <=1.0) -> BOOM wird an Unfallstelle geheftet.
			for (size_t i = 0; i < hindernisse_x.size(); i++)
			{

				if ((abstand1) <= 1.0)
				{
					crash_happen = true;
					crash_time = time_sec;
					vel1 = 0.0;
					rot1 = 0;
					pos_y3 = hindernisse_y.at(hindernis1_nr);
					pos_x3 = hindernisse_x.at(hindernis1_nr);
					//string text = "HALLLLOOOOO";
					//font.draw_rel("HALLLLOOOOO", 500, 500, 100, 0.5, 0.5, 1.0, 1.0);
				};
			}

			//Korrigierung Geschwindigkeit
			if (vel1 <= 0.01)
				vel1 = 0.0;

			//Spieler 2
			if (two_player_mod)
			{
				//if ((input().down(Gosu::KB_S)) && (!(input().down(Gosu::KB_W))) && (pos_y4 == graphics().height() / 2.0) && (((abstand) >= 1.0) || (abstand_x > 0.0)))
				if ((input().down(Gosu::KB_S)) && (!(input().down(Gosu::KB_W))) && (((abstand2) >= 1.0) || (abstand2_x > 0.0)))
				{
					if (vel2 <= 0)
						vel2 = 0.05;
					if (vel2 < 20)	//Maximalgeschwindigkeitsänderung: 20
					{
						vel2 += 0.1;	//Pro Durchlauf Erhöhung der Geschwindigkeit um 0.05
					}
					if ((pos_y4 > pos_y1) || ((pos_y1 == pos_y4) && (vel1 < vel2)) && (pos_y4 >= graphics().height() / 2.0))
					{
						pos_y2 -= vel2 * cos(rot2*PI / 180);
						pos_y3 -= vel2 * cos(rot2*PI / 180);
						ypos12_vorher = pos_y1;
						pos_y1 -= vel2 * cos(rot2*PI / 180);
						for (int j = 0; j < hindernisse_x.size(); j++)
						{
							hindernisse_y.at(j) -= vel2 * cos(rot2*PI / 180);
						}
					}
					else if ((pos_y4 <= graphics().height() / 2.0))
					{
						ypos22_vorher = pos_y4;
						pos_y4 += vel2 * cos(rot2*PI / 180);
					}
					else if ((pos_y4 < pos_y1) || ((pos_y1 == pos_y4) && (vel2 < vel1)))
					{
						ypos22_vorher = pos_y4;
						pos_y4 += vel2 * cos(rot2*PI / 180);
					}
					else if ((pos_y1 == pos_y4) && (vel1 == vel2))
					{
						pos_y2 -= vel2 * cos(rot2*PI / 180);
						pos_y3 -= vel2 * cos(rot2*PI / 180);
						for (int j = 0; j < hindernisse_x.size(); j++)
						{
							hindernisse_y.at(j) -= vel2 * cos(rot2*PI / 180);
						}
					}
					if (pos_y4 > (graphics().height() / 2.0) - 2 && pos_y4 < (graphics().height() / 2.0) + 2)
					{
						ypos22_vorher = pos_y4;
						pos_y4 = graphics().height() / 2.0;
					}
					//poss += vel1 * cos(rot*PI / 180);


				}
				//else if ((!(input().down(Gosu::KB_S)) && (vel2 > 0)) && (abstand > 0) && (pos_y4 == graphics().height() / 2.0))
				else if ((!(input().down(Gosu::KB_S)) && (vel2 > 0)) && (abstand2 > 0))
				{
					vel2 -= 0.2;	//leichtes Bremsen bei Inaktivität
					if ((pos_y4 > pos_y1) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
					{
						pos_y2 -= vel2 * cos(rot2*PI / 180);
						pos_y3 -= vel2 * cos(rot2*PI / 180);
						ypos12_vorher = pos_y1;
						pos_y1 -= vel2 * cos(rot2*PI / 180);
						for (int j = 0; j < hindernisse_x.size(); j++)
						{
							hindernisse_y.at(j) -= vel2 * cos(rot2*PI / 180);
						}
					}
					else if ((pos_y4 < pos_y1) || ((pos_y1 == pos_y4) && (vel1 > vel2)))
					{
						ypos22_vorher = pos_y4;
						pos_y4 += vel2 * cos(rot2*PI / 180);
					}
					else if (pos_y1 == pos_y4 && (vel1 == vel2))
					{
						pos_y2 -= vel2 * cos(rot2*PI / 180);
						pos_y3 -= vel2 * cos(rot2*PI / 180);
						for (int j = 0; j < hindernisse_x.size(); j++)
						{
							hindernisse_y.at(j) -= 0.5* vel2 * cos(rot2*PI / 180);
						}
					}
				}
				else if (vel2 < 0)
				{
					vel2 = 0;
				}

				//Wenn Auto 2 weiter als die Mitte des Bildschirms ist
				if (pos_y4 > graphics().height() / 2.0)
				{
					double differenz = pos_y4 - graphics().height() / 2.0;
					pos_y4 = graphics().height() / 2.0;
					pos_y2 -= differenz;
					pos_y3 -= differenz;
					ypos12_vorher = pos_y1;
					pos_y1 -= differenz;
					for (int j = 0; j < hindernisse_x.size(); j++)
					{
						hindernisse_y.at(j) -= differenz;
					}
				}

				//Bremsen auf Gras bis Minimalspeed
				if ((pos_x4 < posX_Streckenrand_links || pos_x4 > posX_Streckenrand_rechts)) {
					if (vel1 > 10)
						vel1 = 10.0;
					if (vel2 > 4) {
						vel2 -= 0.25;
					}

				}

				//Zurücksetzen bei Verlassen des Bildschirmrands Auto 2
				if ((pos_x4 < posX_Bildschirm_links || pos_x4 > posX_Bildschirm_rechts)) {
					ypos22_vorher = pos_y4;
					pos_y4 = 400;
					pos_x4 = c_WINDOW_WIDTH / 2.0;
					rot2 = 0;
					vel2 = vel2 / 2.0;
				}

				//Freistellen des Autos zurück bei Crash
				if ((input().down(Gosu::KB_W)) && ((abstand2) < 1.0) && (abstand2_x <= 1.0))
				{
					ypos22_vorher = pos_y4;
					pos_y4 -= 100;
				}

				//Bremsen durch Hoch-Taste
				if ((input().down(Gosu::KB_W)) && (vel2 > 0))
					vel2 -= 1.0;

				// Crash-Erkennung (Abstand <=1.0) -> BOOM wird an Unfallstelle geheftet.
				for (size_t i = 0; i < hindernisse_x.size(); i++)
				{

					if ((abstand2) <= 1.0 && (pos_x3 != graphics().width() / 2.0))
					{
						crash_happen = true;
						crash_time = time_sec;
						vel2 = 0.0;
						rot2 = 0;
						pos_y3 = hindernisse_y.at(hindernis2_nr);
						pos_x3 = hindernisse_x.at(hindernis2_nr);
					};
				}

				//Korrigierung Geschwindigkeit
				if (vel2 <= 0.01)
					vel2 = 0.0;

				//Crash zwischen beiden Autos
				abstand3_x = abstand.get_abstand_x(player1.width(), player2.width(), pos_x1, pos_x4);
				abstand3_y = abstand.get_abstand_y(player1.height(), player2.height(), pos_y1, pos_y4);
				abstand3 = sqrt(abstand3_x * abstand3_x + abstand3_y * abstand3_y);
				if (abstand3<=1.0&&!gecrashed)
				{
					rot1 *= -0.5;
					rot2 *= -0.5;
					vel1 *= 0.5;
					vel2 *= 0.5;
					gecrashed = true;
				}

				if (abstand3 > 1.0&&gecrashed)
					gecrashed = false;

			}
			/*for (int n = 1; n < c_WINDOW_WIDTH; n++) {
			graphics().draw_line((pos_x1 + 19), (pos_y1 - 45), Gosu::Color::WHITE, (pos_x4 - 19), (pos_y4 - 45), Gosu::Color::WHITE, 10);
			graphics().draw_line((pos_x1 + 19), (pos_y1 + 45), Gosu::Color::WHITE, (pos_x4 - 19), (pos_y4 + 45), Gosu::Color::WHITE, 10);
			graphics().draw_line((pos_x1 - 19), (pos_y1 - 45), Gosu::Color::BLUE, (pos_x4 + 19), (pos_y4 - 45), Gosu::Color::BLUE, 10);
			graphics().draw_line((pos_x1 - 19), (pos_y1 + 45), Gosu::Color::BLUE, (pos_x4 + 19), (pos_y4 + 45), Gosu::Color::BLUE, 10);

			}*/
		}
		
	}
};

//************************************************************************************************************************

class highscore_end : public Gosu::Window
{
	Gosu::Image highscore;
	Gosu::TextInput score_play;
	Gosu::Font Textfeld, Highscore1, Highscore2, Highscore3;

public:
	int score_auto1_var = 0;
	int time_sec_fin = 0;
	fstream f;
	vector<string> list;
	int number = 0;
	highscore_end(int score_auto1, double time_sec_final, bool two_player_active)
		: Window(1000, 1000)
		, highscore("highscore.jpg")
		, Textfeld(20)
		, Highscore1(20)
		, Highscore2(20)
		, Highscore3(20)
	{
		score_auto1_var = score_auto1;
		time_sec_fin = time_sec_final;
		string player1;
		string player2;
		if (!two_player_active)
		{
			cout << " NAME PLAYER 1: " << endl;
			cin >> player1;
			f.open("highscore.txt", ios::app);
			f << score_auto1 << "   " << player1 << "  Time: " << time_sec_fin << endl;

			f.close();
		}
		/* Sortieren */
		int number_of_lines = 0;
		string h;
		string line;
		ifstream myfile("highscore.txt");
		if (myfile.is_open()) {
			while (!myfile.eof()) {
				getline(myfile, line);
				list.push_back(line);
				number_of_lines++;
				cout << line << endl;
			}
			number = number_of_lines;
			//cout << number << endl;
			myfile.close();
		}
		bool sortiert = TRUE;
		while (sortiert) {
			sortiert = FALSE;
			for (int i = 0; i < number - 1; i++)
				if (list.at(i) > list.at(i + 1)) {
					h = list.at(i);
					list.at(i) = list.at(i + 1);
					list.at(i + 1) = h;
					sortiert = TRUE;
				}

		}

	}

	void draw() override
	{
		highscore.draw_rot(500, 500, 0,
			0, // Rotationswinkel in Grad
			0.5, 0.5, 0.5, 0.5); // Position der "Mitte"

		Highscore1.draw_rel("1. Platz:  " + list.at(number - 1), 500, (900 - 1 * 75), 10, 0.5, 0.5, 1.0, 1.0);
		Highscore2.draw_rel("2. Platz:  " + list.at(number - 2), 500, (900 - 2 * 75), 10, 0.5, 0.5, 1.0, 1.0);
		Highscore3.draw_rel("3. Platz:  " + list.at(number - 3), 500, (900 - 3 * 75), 10, 0.5, 0.5, 1.0, 1.0);
	}

	void update() override {
		if (input().down(Gosu::KB_ESCAPE))
		{
			close();
		}
	};
};


// C++ Hauptprogramm
int main()
{
	bool two_player_active = false;
	int score_auto1_var = 0;
	int time_sec_fin = 0;
	bool exit_all = false;
	hauptmenue window(&two_player_active, &exit_all);
	sndPlaySound(L"tone.wav", SND_FILENAME | SND_ASYNC | SND_LOOP);
	while (!exit_all) {
		window.show();
		if (!exit_all) {
			GameWindow gamewindow(two_player_active, &score_auto1_var, &time_sec_fin);
			gamewindow.show();
			// cout << "SCCCCCCOOOOORE: " << score_auto1_var << endl;
			highscore_end high_window(score_auto1_var, time_sec_fin, two_player_active);
			high_window.show();
			two_player_active = false;
		}
	};
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
- wenn innerhalb (Asphalt) -> normale Geschwindigkeitsverhältnisse ERLEDIGT
- wenn außerhalb (Gras) -> schlechtere Geschwindigkeitsverhältnisse ERLEDIGT
- außerhalb z.B. Schlammpfützen -> ganz schlechte Verhältnisse NICHT NÖTIG
- Multiplayer:
- 2. Auto hinzufügen (Mike) erledigt
- Differenz == Bildschirmhöhe zwischen beiden Autos -> Spielende (Mike)
- Crash-Erkennung gegenseitig (Patrick) ERLEDIGT
-vorderes Auto muss bei Bildchirmmitte sein!
- Hauptmenü erstellen (Patrick):
- Spiel variabler gestalten:
- Einzelspieler/Mehrspieler
- Schwierigkeit (z.B. höhere Geschwindigkeit)
- evtl. Empfindlichkeit bei Lenkung
- Statistik
*/