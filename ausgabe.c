/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		ausgabe.c
				Funktionsdefinitionen für die Grafikausgabe
	Autor:		Martin Gräfe, Technische Hochschule Mittelhessen
*/

# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include <stdlib.h>
# include <Windows.h>
# include "ausgabe.h"
# include "spiel.h"
# include "simple_draw.h"

//---------- Hilfsfunktionen ----------

int fensterbreite(void)
{
	return (2 * RADIUS + 6) * (BREITE)+30;
}

int fensterhoehe(void)
{
	return 2 * RADIUS * (HOEHE)+55;
}

int pos_x(int spalte)
{
	return spalte * (2 * RADIUS + 6) + RADIUS + 18;
}

int pos_y(int zeile)
{
	return 40 + (HOEHE - zeile) * 2 * RADIUS - RADIUS;
}

//---------- Funktionsdefinitionen ----------

int spalte_aus_x(int x_pos)
{
	if (x_pos < 18)
		return 0;
	return (x_pos - 18) / (2 * RADIUS + 6);
}

int eingabe_spalte(void)
{
	while (!(GetMouseButton() & 2))
		Delay(50);
	return spalte_aus_x(GetMouseX());
}

void clear_mousebutton(void)
{
	GetMouseButton();
}

void meldung(char *text)
{
	int y, b;

	b = fensterbreite();
	SetPen(255, 255, 255, 1);
	for (y = 10; y < 35; y++)
		DrawLine(15, y, b - 15, y);

	SetPen(250, 0, 0, 2);
	MoveTo(15, 10);
	DrawTo(b - 15, 10);
	DrawTo(b - 15, 35);
	DrawTo(15, 35);
	DrawTo(15, 10);
	DrawLine(16, 10, 15, 10);		// linke obere Ecke sauber zeichnen
	PlaceText(20, 13, text);
	system("pause");
}

void zeichne_stein(int spalte, int zeile, enum farbe farbe)
{
	int x, y;

	switch (farbe)
	{
	case rot: SetPen(250, 0, 0, RADIUS * 2); break;
	case gelb: SetPen(240, 230, 0, RADIUS * 2); break;
	default: SetPen(255, 255, 255, RADIUS * 2);
	}
	x = pos_x(spalte);
	y = pos_y(zeile);
	DrawLine(x, y, x, y);
}

void werfe_stein(int spalte, int zeile, enum farbe farbe)	// mit Animation
{
	int x, y, i;

	x = pos_x(spalte);
	y = pos_y(HOEHE - 1);
	for (i = y; i <= pos_y(zeile); i += RADIUS / 2)
	{
		if (i > y)
		{
			SetPen(255, 255, 255, RADIUS * 2);
			DrawLine(x, i - RADIUS / 2, x, i - RADIUS / 2);
		}
		switch (farbe)
		{
		case rot: SetPen(250, 0, 0, RADIUS * 2); break;
		case gelb: SetPen(240, 230, 0, RADIUS * 2); break;
		default: SetPen(255, 255, 255, RADIUS * 2);
		}
		DrawLine(x, i, x, i);
		Sleep(25);
	}
}

void zeichne_spielfeld(void)
{
	int x, y, spalte, hoehe;
	char buffer[40];

	ClearGraphic();
	ResizeGraphic(-1, -1, fensterbreite(), fensterhoehe());
	hoehe = HOEHE * RADIUS * 2;

	SetPen(0, 0, 200, 5);
	y = pos_y(0) + RADIUS + 5;
	DrawLine(15, y, fensterbreite() - 15, y);

	SetPen(0, 0, 200, 3);
	y = pos_y(0) + RADIUS + 5;
	for (spalte = 0; spalte <= BREITE; spalte++)
	{
		x = pos_x(spalte) - RADIUS - 3;
		DrawLine(x, y, x, 40);
	}

	for (spalte = 0; spalte < BREITE; spalte++)		// Beschriftung der Spalten
	{
		x = pos_x(spalte) - 4;
		sprintf(buffer, "%d", spalte);
		PlaceText(x, 15, buffer);
	}
}

void zeichne_alles(void)
{
	int spalte, zeile;

	zeichne_spielfeld();
	for (spalte = 0; spalte < BREITE; spalte++)
		for (zeile = 0; zeile < HOEHE; zeile++)
			zeichne_stein(spalte, zeile, stein(spalte, zeile));
}

void markiere_4er(int spalte, int zeile, enum richtung richtung)
{
	SetPen(0, 210, 0, 7);
	switch (richtung)
	{
	case rechts:
		DrawLine(pos_x(spalte), pos_y(zeile), pos_x(spalte + 3), pos_y(zeile));
		break;
	case oben:
		DrawLine(pos_x(spalte), pos_y(zeile), pos_x(spalte), pos_y(zeile + 3));
		break;
	case oben_rechts:
		DrawLine(pos_x(spalte), pos_y(zeile), pos_x(spalte + 3), pos_y(zeile + 3));
		break;
	case oben_links:
		DrawLine(pos_x(spalte), pos_y(zeile), pos_x(spalte - 3), pos_y(zeile + 3));
		break;
	}
}
