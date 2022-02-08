/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		computerspieler.c - Funktionsdefinition für den Computerspieler
	Autor:		BITTE HIER DIE AUTOREN EINTRAGEN!
*/

# include "spiel.h"
# include <stdlib.h>
#include <math.h>
#include "computerspieler.h"

#define INT_MAX 2147483647
#define INT_MIN -2147483648

//Globale Variablen
enum farbe meinSpieler;
int anzahlAusgaben = 0;
//int ausgSpalte;

int computerspieler(enum farbe farbe)	// Rückgabewert: Spaltennummer für den nächsten Wurf
{
	int freieSpalten[BREITE]; // Array welches die freien Spalten speichert
	int spalte, bewertung;
	enum farbe spielfeld[BREITE][HOEHE];		// neues Spielfeld für Probezüge
	struct rueckgabe rueckgabeWert;

	meinSpieler = farbe;
	kopiere_spielfeld(spielfeld);				// "echtes" Spielfeld kopieren
	
	printf("\nBesten Zug ermitteln:\n");
	//spalte = bestenZugErmitteln(spielfeld, farbe);

	rueckgabeWert = minimax(spielfeld, 7, INT_MIN, INT_MAX, 1);
	spalte = rueckgabeWert.spalte;
	bewertung = rueckgabeWert.bewertung;

	//spielfeldAusgeben(spielfeld);
	
	return spalte;								// Gewählten Zug zurückgeben.
}

int getFreieSpalten(int freieSpalten[], enum farbe spielfeld[BREITE][HOEHE])
{
	int spalte, anzahlFreierSpalten = 0;

	for (spalte = 0; spalte < BREITE; spalte++)
	{
		if (spalte_voll(spielfeld, spalte))
		{
			freieSpalten[spalte] = 0;
			continue;
		}
		freieSpalten[spalte] = 1;
		anzahlFreierSpalten++;
	}

	return anzahlFreierSpalten;
}

/*
	Ermittelt die Farbe des Gegners
*/
enum farbe getGegner(enum farbe spieler)
{
	if (spieler == rot)
	{
		return gelb;
	}
	return rot;
}

/*
	Rückgabewert: Bewertung des Spielzuges 
*/
struct rueckgabe minimax(enum farbe spielfeld[BREITE][HOEHE], int tiefe, int alpha, int beta, int maxSpieler)
{
	int freieSpalten[BREITE]; // Array welches die freien Spalten speichert
	int anzahlFreierSpalten, spaltei, neueBewertung;
	int spalteUndBewertung[2]; //0 = Spalte, 1 = Zeile
	enum farbe gegner;
	enum farbe spielfeldTemp[BREITE][HOEHE];
	struct rueckgabe rueckgabeWert;

	gegner = getGegner(meinSpieler);
	
	
	anzahlFreierSpalten = getFreieSpalten(freieSpalten, spielfeld);

	//Notiz an mich: gewonnen als Variable für schnellere abfrage
	if (tiefe == 0 || gewonnen(spielfeld) != keiner || !anzahlFreierSpalten)
	{
		if (gewonnen(spielfeld) == meinSpieler)
		{
			//printf("\nMein Spieler hat gewonnen");
			rueckgabeWert.bewertung = INT_MAX;
			return rueckgabeWert;
		}
		else if (gewonnen(spielfeld) == gegner)
		{
			//printf("\nGegner hat gewonnen");
			rueckgabeWert.bewertung = INT_MIN;
			return rueckgabeWert;
		}
		else if (!anzahlFreierSpalten) //Unentschieden
		{
			//printf("\nUnentschieden");
			rueckgabeWert.bewertung = 0;
			return rueckgabeWert;
		}
		else //Tiefe = 0
		{
			rueckgabeWert.bewertung = bewertungSpielfeld(spielfeld, meinSpieler);

			return rueckgabeWert;
		}
	} 

	if (maxSpieler)
	{
		rueckgabeWert.bewertung = INT_MIN;
		srand(time(NULL));
		do
		{
			rueckgabeWert.spalte = rand() % BREITE;
		} while (!freieSpalten[rueckgabeWert.spalte]);

		for (spaltei = 0; spaltei < BREITE; spaltei++)
		{
			if (freieSpalten[spaltei])
			{
				kopiereSpielfeldVernuenfitg(spielfeld, spielfeldTemp);
				setze_stein(spielfeldTemp, spaltei, meinSpieler);
				//spielfeldAusgeben(spielfeldTemp);
				//printf("\nAnzahl: %d\n", anzahlAusgaben++);
				neueBewertung = minimax(spielfeldTemp, tiefe - 1, alpha, beta, 0).bewertung;

				if (neueBewertung > rueckgabeWert.bewertung)
				{
					rueckgabeWert.bewertung = neueBewertung;
					rueckgabeWert.spalte = spaltei;
				}
				alpha = max(alpha, rueckgabeWert.bewertung);
				if (alpha >= beta)
				{
					break;
				}
			}
		}
		return rueckgabeWert;
	}
	else
	{
		rueckgabeWert.bewertung = INT_MAX;
		srand(time(NULL));
		do
		{
			rueckgabeWert.spalte = rand() % BREITE;
		} while (!freieSpalten[rueckgabeWert.spalte]);

		for (spaltei = 0; spaltei < BREITE; spaltei++)
		{
			if (freieSpalten[spaltei])
			{
				kopiereSpielfeldVernuenfitg(spielfeld, spielfeldTemp);
				setze_stein(spielfeldTemp, spaltei, gegner);
				//spielfeldAusgeben(spielfeldTemp);
				//printf("\nAnzahl: %d\n", anzahlAusgaben++);
				neueBewertung = minimax(spielfeldTemp, tiefe - 1, alpha, beta, 1).bewertung;

				if (neueBewertung < rueckgabeWert.bewertung)
				{
					rueckgabeWert.bewertung = neueBewertung;
					rueckgabeWert.spalte = spaltei;
				}
				beta = min(beta, rueckgabeWert.bewertung);
				if (alpha >= beta)
				{
					break;
				}
			}
		}
		return rueckgabeWert;
	}
}

int bewertungSpielfeld(enum farbe spielfeld[BREITE][HOEHE], enum farbe spieler)
{
	int bewertung = 0, anzahlSpielerSteine = 0;
	int zeile, spalte, i;
	enum farbe ausschnitt[4];

	//Bewertung mittlere Spalte
	for (zeile = 0; zeile < HOEHE; zeile++)
	{
		if (spielfeld[3][zeile] == spieler)// wählt die mittlere Spalte aus
		{
			anzahlSpielerSteine++;
		}
	}
	bewertung += anzahlSpielerSteine * 3;

	//Bewertung Horizontal
	for (zeile = 0; zeile < HOEHE; zeile++)
	{
		for (spalte = 0; spalte < BREITE-3; spalte++)
		{
			for (i = 0; i < 4; i++)
			{
				ausschnitt[i] = spielfeld[spalte + i][zeile]; //Sammelt 4 horizontale Steine
			}
			bewertung += bewertungAusschnitt(ausschnitt, spieler);
		}
	}

	//Bewertung Vertikal
	for (spalte = 0; spalte < BREITE; spalte++)
	{
		for (zeile = 0; zeile < HOEHE-3; zeile++)
		{
			for (i = 0; i < 4; i++)
			{
				ausschnitt[i] = spielfeld[spalte][zeile+i]; //Sammelt 4 vertikale Steine
			}
			bewertung += bewertungAusschnitt(ausschnitt, spieler);
		}
	}
	
	//Bewertung Diagonal
	for (zeile = 0; zeile < HOEHE - 3; zeile++)
	{
		for (spalte = 0; spalte < BREITE - 3; spalte++)
		{
			for (i = 0; i < 4; i++)
			{
				ausschnitt[i] = spielfeld[spalte + i][zeile + i]; //Sammelt 4 vertikale Steine
			}
			bewertung += bewertungAusschnitt(ausschnitt, spieler);
		}
	}
	for (zeile = 0; zeile < HOEHE - 3; zeile++)
	{
		for (spalte = 0; spalte < BREITE - 3; spalte++)
		{
			for (i = 0; i < 4; i++)
			{
				ausschnitt[i] = spielfeld[spalte + i][zeile + 3 - i]; //Sammelt 4 vertikale Steine
			}
			bewertung += bewertungAusschnitt(ausschnitt, spieler);
		}
	}

	
	return bewertung;
}

int bewertungAusschnitt(enum farbe ausschnitt[4], enum farbe spieler)
{
	int bewertung = 0, i;
	int anzahlSpielerSteine = 0, anzahlGegnerSteine = 0, anzahlKeineSteine = 0;
	enum farbe gegner = getGegner(spieler);

	//printf("\nAusschnitt: %s %s %s %s", ausschnitt[0], ausschnitt[1], ausschnitt[2], ausschnitt[3]);


	//Zählt die Steine welche wem Spieler gehören
	for (i = 0; i < 4; i++)
	{
		if (ausschnitt[i] == spieler)
		{
			anzahlSpielerSteine++;
		}
		else if (ausschnitt[i] == gegner)
		{
			anzahlGegnerSteine++;
		}
		else
		{
			anzahlKeineSteine++;
		}
	}

	//Positive Bewertung 
	if (anzahlSpielerSteine == 4)
	{
		bewertung += 100;
	}
	else if (anzahlSpielerSteine == 3 && anzahlKeineSteine == 1)
	{
		bewertung += 5;
	}
	else if (anzahlSpielerSteine == 2 && anzahlKeineSteine == 2)
	{
		bewertung += 2;
	}

	//Negative Bewertung
	if (anzahlGegnerSteine == 3 && anzahlKeineSteine == 1)
	{
		bewertung -= 4;
	}

	return bewertung;
}

int bestenZugErmitteln(enum farbe spielfeld[BREITE][HOEHE], enum farbe spieler)
{
	int freieSpalten[BREITE], anzahlFreieSpalten;
	int besteSpalte, bestesErgebnis = -10000, ergebnis;
	int spalte;
	enum farbe tempSpielfeld[BREITE][HOEHE]; //Notiz an mich: Evt auf static setzten 
	
	anzahlFreieSpalten = getFreieSpalten(freieSpalten, spielfeld);

	if (anzahlFreieSpalten)
	{
		spalte = 0;
		do
		{
			besteSpalte = spalte; //Notiz an mich: Zufaellige auswahl
		} while (!freieSpalten[spalte++]); //Ermittelt die nächste freie Spalte

		printf("\nbeste Spalte ermitteln\n");
		//Ermittlung der besten Spalte
		for (spalte = 0; spalte < BREITE; spalte++)
		{
			if (freieSpalten[spalte])
			{
				printf("Schleife: %d\n", spalte);
				kopiere_spielfeld(tempSpielfeld);
				setze_stein(tempSpielfeld, spalte, spieler);
				ergebnis = bewertungSpielfeld(tempSpielfeld, spieler);

				printf("Spalte: %d Bewertung: %d\n", spalte, ergebnis);

				if (ergebnis > bestesErgebnis)
				{
					bestesErgebnis = ergebnis;
					besteSpalte = spalte;
				}
			}
		}
	}
	else
	{
		besteSpalte = -1; //Fehler wenn keine Spalte mehr frei ist
	}

	printf("\nbeste Spalte: %d\n", besteSpalte);

	return besteSpalte;
}


int freieZeile(enum farbe spielfeld[BREITE][HOEHE], int spalte)
{
	int zeile;

	for (zeile = 0; zeile < HOEHE; zeile++)
	{
		if (spielfeld[spalte][zeile] != keiner)
		{
			return zeile;
		}
	}
}

void spielfeldAusgeben(enum farbe spielfeld[BREITE][HOEHE])
{
	int spalte, zeile;
	
	printf("\n\n");
	for (zeile = 0; zeile < HOEHE; zeile++)
	{
		for (spalte = 0; spalte < BREITE; spalte++)
		{
			switch (spielfeld[spalte][zeile])
			{
			case rot:
				printf("Rot  ");
				break;
			case gelb:
				printf("Gelb ");
				break;
			case leer:
				printf("Leer ");
				break;
			default:
				break;
			}
			
		}
		printf("\n");
	}
	
	printf("\n");
	/*
	for (spalte = 0; spalte < BREITE; spalte++)
	{
		for (zeile = 0; zeile < HOEHE; zeile++)
		{
			switch (spielfeld[spalte][zeile])
			{
			case rot:
				printf("Rot  ");
				break;
			case gelb:
				printf("Gelb ");
				break;
			case leer:
				printf("Leer ");
				break;
			default:
				break;
			}

		}
		printf("\n");
	}
	printf("\n");
	*/
}

void kopiereSpielfeldVernuenfitg(enum farbe spielfeldAlt[BREITE][HOEHE], enum farbe spielfeldNeu[BREITE][HOEHE])	// kopiert den aktuellen Spielstand
{														
	int spalte, zeile;

	for (zeile = 0; zeile < HOEHE; zeile++)
		for (spalte = 0; spalte < BREITE; spalte++)
			spielfeldNeu[spalte][zeile] = spielfeldAlt[spalte][zeile];
}

