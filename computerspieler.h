/*
	Funktionsdeklaration für den Computerspieler
*/

# include "spiel.h"

struct rueckgabe {
	int bewertung;
	int spalte;
};

int computerspieler(enum farbe farbe);	// Rückgabewert: Spaltennummer für den nächsten Wurf

int getFreieSpalten(int freieSpalten[], enum farbe spielfeld[BREITE][HOEHE]);

/*
	Ermittelt die Farbe des Gegners
*/
enum farbe getGegner(enum farbe spieler);

/*
	Rückgabewert: Bewertung des Spielzuges
*/
struct rueckgabe minimax(enum farbe spielfeld[BREITE][HOEHE], int tiefe, int alpha, int beta, int spieler);

int bewertungSpielfeld(enum farbe spielfeld[BREITE][HOEHE], enum farbe spieler);

int bewertungAusschnitt(enum farbe ausschnitt[4], enum farbe spieler);

int bestenZugErmitteln(enum farbe spielfeld[BREITE][HOEHE], enum farbe spieler);

int freieZeile(enum farbe spielfeld[BREITE][HOEHE], int spalte);

void spielfeldAusgeben(enum farbe spielfeld[BREITE][HOEHE]);

void kopiereSpielfeldVernuenfitg(enum farbe spielfeldAlt[BREITE][HOEHE], enum farbe spielfeldNeu[BREITE][HOEHE]);