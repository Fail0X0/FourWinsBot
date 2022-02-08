/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		spiel.c - Funktionsdefinitionen für die Spielfunktionen
	Autor:		Martin Gräfe, Technische Hochschule Mittelhessen
*/

# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include "spiel.h"
# include "ausgabe.h"

//---------- globale Variable für das Spielfeld ----------

enum farbe Spielfeld[BREITE][HOEHE];

//---------- Hilfsfunktionen ----------

int check_spalte(int spalte, char *fkt)	// prüft auf gültige Spaltennummer (0=OK, 1=Fehler)
{
	if ((spalte < 0) || (spalte >= BREITE))
	{
		char buffer[40];
		sprintf(buffer, "Err: %s(%d)", fkt, spalte);
		meldung(buffer);
		return 1;
	}
	return 0;
}

//---------- Funktionsdefinitionen ----------

void neues_spiel(enum farbe feld[BREITE][HOEHE])
{
	int spalte, zeile;

	if (feld == NULL)
		feld = Spielfeld;
	for (spalte = 0; spalte < BREITE; spalte++)
		for (zeile = 0; zeile < HOEHE; zeile++)
			feld[spalte][zeile] = leer;
}

enum farbe suche_4er(enum farbe feld[BREITE][HOEHE], int *p_spalte,
	int *p_zeile, enum richtung *p_richtung)		// sucht eine 4er-Reihe
{
	int i, zeile, spalte, count;
	enum farbe f;

	if (feld == NULL)
		feld = Spielfeld;
	*p_richtung = rechts;
	for (zeile = 0; zeile < HOEHE; zeile++)		// 4 in einer Zeile prüfen
	{
		f = leer;
		count = 0;
		for (spalte = 0; spalte < BREITE; spalte++)
		{
			if (feld[spalte][zeile] == f)
			{
				if (f == leer)
					continue;
				count++;
				if (count == 4)
				{
					*p_spalte = spalte - 3;
					*p_zeile = zeile;
					return f;
				}
			}
			else
			{
				f = feld[spalte][zeile];
				count = 1;
			}
		}
	}

	*p_richtung = oben;
	for (spalte = 0; spalte < BREITE; spalte++)		// 4 in einer Spalte prüfen
	{
		f = leer;
		count = 0;
		for (zeile = 0; zeile < HOEHE; zeile++)
		{
			if (feld[spalte][zeile] == leer)
				break;
			if (feld[spalte][zeile] == f)
			{
				if (f == leer)
					continue;
				count++;
				if (count == 4)
				{
					*p_spalte = spalte;
					*p_zeile = zeile - 3;
					return f;
				}
			}
			else
			{
				f = feld[spalte][zeile];
				count = 1;
			}
		}
	}

	spalte = 0;						// diagonal von links unten nach rechts oben prüfen
	zeile = HOEHE - 4;
	*p_richtung = oben_rechts;
	while (spalte < BREITE - 3)
	{
		f = leer;
		count = 0;
		for (i = 0; (zeile + i < HOEHE) && (spalte + i < BREITE); i++)
		{
			if (feld[spalte + i][zeile + i] == f)
			{
				if (f == leer)
					continue;
				count++;
				if (count == 4)
				{
					*p_spalte = spalte + i - 3;
					*p_zeile = zeile + i - 3;
					return f;
				}
			}
			else
			{
				f = feld[spalte + i][zeile + i];
				count = 1;
			}
		}
		if (zeile > 0)
			zeile--;
		else
			spalte++;
	}

	spalte = BREITE - 1;				// diagonal von rechts unten nach links oben prüfen
	zeile = HOEHE - 4;
	*p_richtung = oben_links;
	while (spalte > 2)
	{
		f = leer;
		count = 0;
		for (i = 0; (zeile + i < HOEHE) && (spalte - i >= 0); i++)
		{
			if (feld[spalte - i][zeile + i] == f)
			{
				if (f == leer)
					continue;
				count++;
				if (count == 4)
				{
					*p_spalte = spalte - i + 3;
					*p_zeile = zeile + i - 3;
					return f;
				}
			}
			else
			{
				f = feld[spalte - i][zeile + i];
				count = 1;
			}
		}
		if (zeile > 0)
			zeile--;
		else
			spalte--;
	}

	return keiner;
}

enum farbe gewonnen(enum farbe feld[BREITE][HOEHE])			// testet, ob ein Spieler gewonnen hat (-> rot/gelb/keiner)
{
	int zeile, spalte;
	enum richtung richtung;

	return suche_4er(feld, &spalte, &zeile, &richtung);
}

int spalte_voll(enum farbe feld[BREITE][HOEHE], int spalte)	// testet, ob noch Platz in einer Spalte (-> 0/1)
{
	if (check_spalte(spalte, "spalte_voll"))
		return 1;
	if (feld == NULL)
		return (Spielfeld[spalte][HOEHE - 1] != leer);
	return (feld[spalte][HOEHE - 1] != leer);
}

int alles_voll(enum farbe feld[BREITE][HOEHE])			// testet, ob alle Felder belegt sind
{
	int spalte;

	for (spalte = 0; spalte < BREITE; spalte++)
		if (!spalte_voll(feld, spalte))
			return 0;
	return 1;
}

int spalte_anzahl(enum farbe feld[BREITE][HOEHE], int spalte)	// fragt ab, wie viele Steine schon in der Spalte sind
{
	int i;

	if (check_spalte(spalte, "spalte_anzahl"))
		return 0;
	if (feld == NULL)
		feld = Spielfeld;
	for (i = 0; i < HOEHE; i++)
		if (feld[spalte][i] == leer)
			break;
	return i;
}

void setze_stein(enum farbe feld[BREITE][HOEHE], int spalte, enum farbe farbe)	// macht einen Spielzug 
{
	if (check_spalte(spalte, "setze_stein"))
		return;
	if (spalte_voll(feld, spalte))
	{
		char buffer[40];
		sprintf(buffer, "Err: setze_stein(%d) voll!", spalte);
		meldung(buffer);
		return;
	}
	if (feld == NULL)
		feld = Spielfeld;
	feld[spalte][spalte_anzahl(feld, spalte)] = farbe;
}

void entferne_stein(enum farbe feld[BREITE][HOEHE], int spalte)	// macht einen Spielzug 
{																// rückgängig
	int anzahl;

	if (check_spalte(spalte, "entferne_stein"))
		return;
	anzahl = spalte_anzahl(feld, spalte);
	if (anzahl < 1)
	{
		char buffer[40];
		sprintf(buffer, "Err: entferne_stein(%d) leer!", spalte);
		meldung(buffer);
		return;
	}
	feld[spalte][anzahl - 1] = leer;
}

enum farbe stein(int spalte, int zeile)		// fragt die Farbe eines Steins im
{											//   Spielfeld ab (-> rot/gelb/leer)
	if ((zeile < 0) || (zeile >= HOEHE) || (spalte < 0) || (spalte >= BREITE))
	{
		char buffer[40];
		sprintf(buffer, "Err: stein(%d, %d)", spalte, zeile);
		meldung(buffer);
		return leer;
	}
	return Spielfeld[spalte][zeile];
}

void kopiere_spielfeld(enum farbe ziel[BREITE][HOEHE])	// kopiert den aktuellen Spielstand
{														// in das Feld 'ziel'
	int spalte, zeile;

	for (zeile = 0; zeile < HOEHE; zeile++)
		for (spalte = 0; spalte < BREITE; spalte++)
			ziel[spalte][zeile] = stein(spalte, zeile);
}
