/*
	simple_draw.h - Headerdatei für die Zeichenfunktionen zu "Simple DDE Draw"
	Autor: Martin Gräfe, martin.e.graefe@iem.thm.de
*/

#ifdef __cplusplus
extern "C" {
#endif

	/******************************************************
		Löschen der Grafik
		keine Parameter, kein Rückgabewert
	*******************************************************/
	void ClearGraphic(void);


	/******************************************************
		Grafik-Cursor positionieren
		Parameter: x, y
	*******************************************************/
	void MoveTo(int x, int y);


	/******************************************************
		Linie von Grafik-Cursor aus zu (x,y) ziehen
		Parameter: x, y
	*******************************************************/
	void DrawTo(int x, int y);


	/******************************************************
		Linie zeichnen
		Parameter: x1, y1, x2, y2
	*******************************************************/
	void DrawLine(int x1, int y1, int x2, int y2);


	/******************************************************
		Farbe und Strichstärke einstellen
		Parameter: Rot, Grün, Blau, Breite
	*******************************************************/
	void SetPen(int r, int g, int b, int w);


	/******************************************************
		Einzelnen Pixel setzen
		Parameter: x, y, Rot, Grün, Blau
	*******************************************************/
	void DrawPixel(int x, int y, int r, int g, int b);


	/******************************************************
		Text zeichnen
		Parameter: x, y, Text (UTF-8, max. 40 Bytes)
	*******************************************************/
	void PlaceText(int x, int y, char *text);


	/******************************************************
		Fenstergröße ändern / Fenster verschieben
		Parameter: Position x, Position y, Breite, Höhe
	*******************************************************/
	void ResizeGraphic(int x, int y, int width, int height);


	/******************************************************
		Fenster automatisch im Vordergrund ein-/ausschalten
		Parameter: on/off
	*******************************************************/
	void GraphicToFront(int on_off);


	/******************************************************
		X-Koordinate des Cursors (Maus) auslesen
		keine Parameter
		Rückgabewert(int): x-Koordinate des Cursors im Fenster
	*******************************************************/
	int GetMouseX(void);


	/******************************************************
		Y-Koordinate des Cursors auslesen
		keine Parameter
		Rückgabewert(int): y-Koordinate des Cursors im Fenster
	*******************************************************/
	int GetMouseY(void);


	/******************************************************
		Mausklick abfragen
		keine Parameter
		Rückgabewert: Bit 0: Maustaste ist momentan gedrückt
					  Bit 1: Maustaste wurde seit letzter
							 Abfrage gedrückt
	******************************************************/
	int GetMouseButton(void);

	/******************************************************
		Tastatur abfragen
		keine Parameter
		Rückgabewert(int): liefert 1, wenn eine Taste
		gedrückt wurde, sonst 0
	*******************************************************/
	int KeyPressed(void);

	/******************************************************
		Tastatur abfragen
		keine Parameter
		Rückgabewert(char): liefert den ASCII-Code der
		gedrückten Taste bzw. 0, wenn keine Taste gedrückt ist
	*******************************************************/
	int GetKey(void);

	/******************************************************
		Verzögerung
		Parameter: Zeit in Millisekunden
	*******************************************************/
	void Delay(int milliseconds);

#ifdef __cplusplus
}
#endif
