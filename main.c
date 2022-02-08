/*
	Projekt:	Informatik 2 - Spiel "Vier gewinnt"
	Datei:		main.c
	Autor:		Martin Gräfe, Technische Hochschule Mittelhessen
	Version mit grafischer Eingabe - benötigt "Simple Draw 3"!
*/

# define _CRT_SECURE_NO_WARNINGS
# include <stdio.h>
# include "ausgabe.h"
# include "spiel.h"
# include "computerspieler.h"

//---------- Hauptprogramm ----------

int main()
{
	enum farbe f;
	int spalte, zeile;
	enum richtung richtung;

	neues_spiel(NULL);
	zeichne_spielfeld();

	printf("\n\n V I E R   G E W I N N T\n"
		" =======================\n\n"
		" Zum Ziehen bitte die gew\201nschte Spalte anklicken!\n\n");
	f = rot;	// Rot fängt an
	do {
		if (f == rot)
		{
			spalte = eingabe_spalte();
			if ((spalte < 0) || (spalte >= BREITE) || spalte_voll(NULL, spalte))
			{
				printf("\n r:Fehler (ung\201ltige Spalte %d)\n", spalte);
				continue;
			}
			printf(" r:%d", spalte);
		}
		else
		{
			spalte = computerspieler(f);
			printf(" g:%d", spalte);
			clear_mousebutton();
		}
		werfe_stein(spalte, spalte_anzahl(NULL, spalte), f);	// Animation
		setze_stein(NULL, spalte, f);
		if (f == rot)
			f = gelb;
		else
			f = rot;
	} while ((gewonnen(NULL) == keiner) && !alles_voll(NULL));

	switch (suche_4er(NULL, &spalte, &zeile, &richtung))
	{
	case rot: printf("\n\n Rot hat gewonnen!\n\n ");
		markiere_4er(spalte, zeile, richtung);
		meldung("Rot hat gewonnen!");
		break;
	case gelb: printf("\n\n Gelb hat gewonnen!\n\n ");
		markiere_4er(spalte, zeile, richtung);
		meldung("Gelb hat gewonnen!");
		break;
	default: printf("\n\n Unentschieden.\n\n ");
		meldung("Unentschieden!");
	}

	return 0;
}
