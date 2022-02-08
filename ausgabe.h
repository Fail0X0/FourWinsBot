/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		ausgabe.h
				Funktionsdeklarationen für die Grafikausgabe
	Autor:		Martin Gräfe, Technische Hochschule Mittelhessen
*/

# ifndef AUSGABE_H
# define AUSGABE_H

# include "spiel.h"

# define RADIUS 26	// Größe der Spielsteine

//---------- Funktionsdeklarationen ----------

int fensterbreite(void);
int fensterhoehe(void);
int pos_x(int spalte);
int pos_y(int zeile);
int spalte_aus_x(int x_pos);
int eingabe_spalte(void);
void clear_mousebutton(void);
void meldung(char *text);
void zeichne_stein(int spalte, int zeile, enum farbe farbe);
void werfe_stein(int spalte, int zeile, enum farbe farbe);	// mit Animation
void zeichne_spielfeld(void);
void zeichne_alles(void);
void markiere_4er(int spalte, int zeile, enum richtung richtung);

# endif
