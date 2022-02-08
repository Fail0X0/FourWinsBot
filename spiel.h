/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		spiel.h - Funktionsdefinitionen für die Spielfunktionen
	Autor:		Martin Gräfe, Technische Hochschule Mittelhessen
*/

# ifndef SPIEL_H
# define SPIEL_H

# define BREITE 7	// Anzahl der Spalten
# define HOEHE 7	// max. Anzahl Steine pro Spalte

/*---------- Aufzählungstyp für Spielfeldelemente ----------*/

enum farbe { leer = 0, keiner = 0, rot, gelb };

/*---------- Aufzählungstyp für Richtung einer 4er-Reihe ----------*/

enum richtung { rechts, oben, oben_rechts, oben_links };

/*---------- Funktionsdeklarationen ----------*/

void neues_spiel(enum farbe feld[BREITE][HOEHE]);	// Spielfeld leeren

enum farbe suche_4er(enum farbe feld[BREITE][HOEHE], int *p_spalte,
	int *p_zeile, enum richtung *p_richtung);		// sucht eine 4er-Reihe

enum farbe gewonnen(enum farbe feld[BREITE][HOEHE]);	/* testet, ob ein Spieler gewonnen hat
														   (-> rot/gelb/keiner) */

int spalte_voll(enum farbe feld[BREITE][HOEHE], int spalte);	/* testet, ob noch Platz in
																   einer Spalte (-> 0/1) */

int alles_voll(enum farbe feld[BREITE][HOEHE]);	// testet, ob alle Felder belegt sind (-> 0/1)

int spalte_anzahl(enum farbe feld[BREITE][HOEHE], int spalte);	/* fragt ab, wie viele Steine
																   schon in der Spalte sind */

enum farbe stein(int spalte, int zeile);	/* fragt die Farbe eines Steins im
											   Spielfeld ab (-> rot/gelb/leer) */

void kopiere_spielfeld(enum farbe ziel[BREITE][HOEHE]);	/* kopiert den aktuellen Spielstand
														   in das Feld 'ziel' */

void setze_stein(enum farbe ziel[BREITE][HOEHE], int spalte, enum farbe farbe);	// macht einen Spielzug 

void entferne_stein(enum farbe feld[BREITE][HOEHE], int spalte);	// macht einen Spielzug rückgängig

# endif
