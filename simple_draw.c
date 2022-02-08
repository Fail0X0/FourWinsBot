/*
	simple_draw.c - Definition der Zeichenfunktionen für "Simple DDE Draw"

	Autor: Martin Gräfe, martin.e.graefe@iem.thm.de
	mit Erweiterungen von Christian Loew
*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <Windows.h>
#include <Ddeml.h>

#define SERVICE_NAME "SimpleDraw"	// strings for identifying DDE application
#define TOPIC_NAME "Drawing"
#define ITEM_NAME "Command"
#define ITEM_NAME_XCOORD "XCoord"
#define ITEM_NAME_YCOORD "YCoord"
#define ITEM_NAME_MOUSEBUTTON "MouseButton"
#define ITEM_NAME_KEYPRESSED "KeyPressed"
#define ITEM_NAME_GETKEY "GetKey"

#define REQUEST_DELAY 10		// delay after DDE request to avoid a queue jam

HCONV hConversation = NULL;		// no initial conversation happening
DWORD sd_idInst = 0;
HSZ   sd_hsz_service, sd_hsz_topic, sd_hsz_item, sd_hsz_item_Xcoord, sd_hsz_item_Ycoord, sd_hsz_item_Mouse, sd_hsz_item_KeyPressed, sd_hsz_item_GetKey;


/******************************************************
	DDE-Callback-Funktion für Nachrichten vom Server
	zum Client
*******************************************************/
HDDEDATA DDECallback(UINT uType, UINT uFmt, HCONV hconv, HSZ hsz1, HSZ hsz2,
	HDDEDATA hdata, ULONG_PTR dwData1, ULONG_PTR dwData2)
{
	switch (uType) {
	case XTYP_DISCONNECT:
		hConversation = NULL;
		break;
	default: break;
	}
	return (HDDEDATA)NULL;
}


/******************************************************
	DDE-Verbindung zu "Simple DDE Draw" initialisieren
	Rückgabewert: 0 = OK, 1 = Fehler
*******************************************************/
int sd_dde_init() {
	if (sd_idInst)		// Initialisierung schon durchgeführt?
		return 1;
	if (DdeInitialize(&sd_idInst, (PFNCALLBACK)DDECallback, APPCLASS_STANDARD | APPCMD_CLIENTONLY, 0L) != DMLERR_NO_ERROR)
	{
		fprintf(stderr, "\n*** DDE-Initialisierung fehlgeschlagen!\n");
		return 1;
	}

	sd_hsz_service = DdeCreateStringHandleA(sd_idInst, SERVICE_NAME, CP_WINANSI);
	sd_hsz_topic = DdeCreateStringHandleA(sd_idInst, TOPIC_NAME, CP_WINANSI);
	sd_hsz_item = DdeCreateStringHandleA(sd_idInst, ITEM_NAME, CP_WINANSI);
	sd_hsz_item_Xcoord = DdeCreateStringHandleA(sd_idInst, ITEM_NAME_XCOORD, CP_WINANSI);
	sd_hsz_item_Ycoord = DdeCreateStringHandleA(sd_idInst, ITEM_NAME_YCOORD, CP_WINANSI);
	sd_hsz_item_Mouse = DdeCreateStringHandleA(sd_idInst, ITEM_NAME_MOUSEBUTTON, CP_WINANSI);
	sd_hsz_item_KeyPressed = DdeCreateStringHandleA(sd_idInst, ITEM_NAME_KEYPRESSED, CP_WINANSI);
	sd_hsz_item_GetKey = DdeCreateStringHandleA(sd_idInst, ITEM_NAME_GETKEY, CP_WINANSI);
	if ((sd_hsz_service == NULL) || (sd_hsz_topic == NULL) || (sd_hsz_item == NULL) || (sd_hsz_item_Xcoord == NULL) ||
		(sd_hsz_item_Ycoord == NULL) || (sd_hsz_item_Mouse == NULL) || (sd_hsz_item_KeyPressed == NULL) || (sd_hsz_item_GetKey == NULL))
	{
		fprintf(stderr, "\n*** DDE-String-Handle konnte nicht angelegt werden!\n");
		DdeUninitialize(sd_idInst);
		return 1;
	}

	hConversation = DdeConnect(sd_idInst, sd_hsz_service, sd_hsz_topic, NULL);
	if (hConversation == NULL) {
		system(" echo \"Simple DDE Draw.exe\" | %COMSPEC% > NUL:");		// versuche, Simple DDE Draw zu starten
		Sleep(1000);
		hConversation = DdeConnect(sd_idInst, sd_hsz_service, sd_hsz_topic, NULL);
		if (hConversation == NULL) {
			system(" echo \"..\\Simple DDE Draw.exe\" | %COMSPEC% > NUL:");	// versuche, Simple DDE Draw zu starten
			Sleep(1000);
			hConversation = DdeConnect(sd_idInst, sd_hsz_service, sd_hsz_topic, NULL);
			if (hConversation == NULL) {
				fprintf(stderr, "\n*** Keine Verbindung zu 'Simple DDE Draw'!\n");
				DdeFreeStringHandle(sd_idInst, sd_hsz_service);
				DdeFreeStringHandle(sd_idInst, sd_hsz_topic);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item_Xcoord);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item_Ycoord);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item_Mouse);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item_KeyPressed);
				DdeFreeStringHandle(sd_idInst, sd_hsz_item_GetKey);
				DdeUninitialize(sd_idInst);
				return 1;
			}
		}
	}
	return 0;
}


/******************************************************
	Löschen der Grafik
	keine Parameter, kein Rückgabewert
*******************************************************/
void ClearGraphic(void) {
	if (!hConversation)
		if (sd_dde_init())
			return;
	DdeClientTransaction((LPBYTE)"0", 2, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Grafik-Cursor positionieren
	Parameter: x, y
*******************************************************/
void MoveTo(int x, int y) {
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "1 %d %d", x, y);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Linie von Grafik-Cursor aus zu (x,y) ziehen
	Parameter: x, y
*******************************************************/
void DrawTo(int x, int y) {
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "2 %d %d", x, y);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Linie zeichnen
	Parameter: x1, y1, x2, y2
*******************************************************/
void DrawLine(int x1, int y1, int x2, int y2) {
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "3 %d %d %d %d", x1, y1, x2, y2);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Farbe und Strichstärke einstellen
	Parameter: Rot, Grün, Blau, Breite
*******************************************************/
void SetPen(int r, int g, int b, int w) {
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "4 %d %d %d %d", r, g, b, w);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Einzelnen Pixel setzen
	Parameter: x, y, Rot, Grün, Blau
*******************************************************/
void DrawPixel(int x, int y, int r, int g, int b) {
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "5 %d %d %d %d %d", x, y, r, g, b);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Text zeichnen
	Parameter: x, y, Text (UTF-8, max. 40 Bytes)
*******************************************************/
void PlaceText(int x, int y, char *text) {
	static char buffer[56];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "6 %d %d '", x, y);
	strncat(buffer, text, 39);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Fenstergröße ändern / Fenster verschieben
	Parameter: Position x, Position y, Breite, Höhe
*******************************************************/
void ResizeGraphic(int x, int y, int width, int height)
{
	static char buffer[24];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "7 %d %d %d %d", x, y, width, height);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	Fenster automatisch im Vordergrund ein-/ausschalten
	Parameter: on/off
*******************************************************/
void GraphicToFront(int on_off) {
	static char buffer[16];

	if (!hConversation)
		if (sd_dde_init())
			return;
	sprintf(buffer, "8 %d", on_off);
	DdeClientTransaction((LPBYTE)buffer, (DWORD)strlen(buffer) + 1, hConversation, sd_hsz_item, CF_TEXT, XTYP_POKE, 3000, NULL);
	return;
}


/******************************************************
	X-Koordinate des Cursors (Maus) auslesen
	keine Parameter
	Rückgabewert: x-Koordinate des Cursors im Fenster
*******************************************************/
int GetMouseX(void)
{
	static char buffer[24];
	HDDEDATA hDDE_data;
	unsigned int PosX;

	if (!hConversation)											// Konversation initialisiert
		if (sd_dde_init())										// wenn nicht, wird dies gemacht
			return 0;

	hDDE_data = DdeClientTransaction(NULL, 0, hConversation, sd_hsz_item_Xcoord, CF_TEXT, XTYP_REQUEST, 3000, NULL);

	if (hDDE_data == NULL)										// Fehlerabfrage
	{
		fprintf(stderr, "\n*** Abfrage der Daten von SimpleDraw DDE fehlgeschlagen!\n");
		return 0;
	}

	DdeGetData(hDDE_data, (BYTE*)buffer, 24, 0);				// uebertragene Daten in Puffer schreiben
	PosX = 0;
	sscanf(buffer, "%d", &PosX);								// Daten von Puffer in PosX schreiben
	Sleep(REQUEST_DELAY);

	return PosX;
}


/******************************************************
	Y-Koordinate des Cursors auslesen
	keine Parameter
	Rückgabewert: y-Koordinate des Cursors im Fenster
*******************************************************/
int GetMouseY(void)
{
	static char buffer[24];
	HDDEDATA hDDE_data;
	unsigned int PosY;

	if (!hConversation)
		if (sd_dde_init())
			return 0;

	hDDE_data = DdeClientTransaction(NULL, 0, hConversation, sd_hsz_item_Ycoord, CF_TEXT, XTYP_REQUEST, 3000, NULL);

	if (hDDE_data == NULL)
	{
		fprintf(stderr, "\n*** Abfrage der Daten von SimpleDraw DDE fehlgeschlagen!\n");
		return 0;
	}

	DdeGetData(hDDE_data, (BYTE*)buffer, 24, 0);
	PosY = 0;
	sscanf(buffer, "%d", &PosY);
	Sleep(REQUEST_DELAY);

	return PosY;
}


/******************************************************
	Mausklick abfragen
	keine Parameter
	Rückgabewert: Bit 0: Maustaste ist momentan gedrückt
				  Bit 1: Maustaste wurde seit letzter
						 Abfrage gedrückt
******************************************************/
int GetMouseButton(void)
{
	static char buffer[24];
	HDDEDATA hDDE_data;
	unsigned int MouseButton;

	if (!hConversation)
		if (sd_dde_init())
			return 0;

	hDDE_data = DdeClientTransaction(NULL, 0, hConversation, sd_hsz_item_Mouse, CF_TEXT, XTYP_REQUEST, 3000, NULL);

	if (hDDE_data == NULL)
	{
		fprintf(stderr, "\n*** Abfrage der Daten von SimpleDraw DDE fehlgeschlagen!\n");
		return 0;
	}

	DdeGetData(hDDE_data, (BYTE*)buffer, 24, 0);
	MouseButton = 0;
	sscanf(buffer, "%d", &MouseButton);
	Sleep(REQUEST_DELAY);

	return MouseButton;
}


/******************************************************
	Tastatur abfragen
	keine Parameter
	Rückgabewert(int): liefert 1, wenn gerade eine Taste
	gedrückt wird, sonst 0
*******************************************************/
int KeyPressed(void)
{
	static char buffer[24];
	HDDEDATA hDDE_data;
	unsigned int pressedKey;

	if (!hConversation)
		if (sd_dde_init())
			return 0;

	hDDE_data = DdeClientTransaction(NULL, 0, hConversation, sd_hsz_item_KeyPressed, CF_TEXT, XTYP_REQUEST, 3000, NULL);

	if (hDDE_data == NULL)
	{
		fprintf(stderr, "\n*** Abfrage der Daten von SimpleDraw DDE fehlgeschlagen!\n");
		return 0;
	}

	DdeGetData(hDDE_data, (BYTE*)buffer, 24, 0);
	pressedKey = 0;
	sscanf(buffer, "%d", &pressedKey);
	Sleep(REQUEST_DELAY);

	return pressedKey;
}


/******************************************************
	Tastatur abfragen
	keine Parameter
	Rückgabewert(char): liefert den ASCII-Code der momentan
	gedrückten Taste bzw. 0 wenn keine Taste gedrückt ist
*******************************************************/
int GetKey(void)
{
	static char buffer[24];
	HDDEDATA hDDE_data;
	unsigned char gedruckteTaste;

	if (!hConversation)
		if (sd_dde_init())
			return 0;

	hDDE_data = DdeClientTransaction(NULL, 0, hConversation, sd_hsz_item_GetKey, CF_TEXT, XTYP_REQUEST, 3000, NULL);

	if (hDDE_data == NULL)
	{
		fprintf(stderr, "\n*** Abfrage der Daten von SimpleDraw DDE fehlgeschlagen!\n");
		return 0;
	}

	DdeGetData(hDDE_data, (BYTE*)buffer, 24, 0);
	gedruckteTaste = 0;
	sscanf(buffer, "%c", &gedruckteTaste);
	Sleep(REQUEST_DELAY);

	return gedruckteTaste;
}


/******************************************************
	Verzögerung
	Parameter: Zeit in Millisekunden
*******************************************************/
void Delay(int milliseconds)
{
	Sleep(milliseconds);
}
