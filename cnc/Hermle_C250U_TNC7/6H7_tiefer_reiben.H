BEGIN PGM 6H7_TIEFER_REIBEN MM

; ================================================================
; Hermle C250U / Steuerung TNC7
; Bauteil: Aufnahme, KMW Zg.-Nr. 10114-200120.002.0
;
; Ausgangslage: 2x Bohrung 6H7 (Positionstoleranz zueinander Ø0,01,
; siehe Ansicht E-E der Zeichnung).
;
; Ablauf:
;   1) Bohrung 1 UND Bohrung 2 (beide 6H7) antasten -> KEINE
;      Grunddrehung (Software), sondern der Tisch (Drehachse C)
;      wird real um den gemessenen Winkel gedreht (Zyklus 403)
;   2) Bohrung 1 (6H7) erneut antasten -> wird neuer Werkstueck-
;      Nullpunkt X0/Y0 (Zyklus 412)
;   3) Bohrung 1 (jetzt bei X0 Y0) tiefer reiben (Zyklus 201),
;      Bearbeitungsposition ueber Kontur-Label (LBL 1) angefahren
;
; ACHTUNG - vor dem ersten Abarbeiten:
;   - Alle mit *** markierten Werte direkt in den Zyklen unten mit
;     den realen Werten aus Zeichnung/CAM ueberschreiben. Aktuell
;     sind es Platzhalter!
;   - Zyklus 403 dreht die C-Achse real - VOR dem Reiben pruefen,
;     dass die Basisdrehung im Bezugspunkt danach 0 ist (Tisch ist
;     jetzt mechanisch ausgerichtet, nicht rechnerisch).
;   - Parameterbelegung der Zyklen 403/412 im Klartext-Editor der
;     TNC7 gegenpruefen (Hilfefenster zeigt die fuer den installierten
;     NC-Software-Stand aktuellen Feldnamen/Reihenfolge sowie die
;     Nummer der Kompensationsachse).
;   - Erstlauf im Testlauf-Grafikmodus bzw. im Einzelsatz mit
;     reduziertem Vorschub und vergroessertem Sicherheitsabstand
;     pruefen. Bei Ø0,01-Toleranz unbedingt Messprotokoll nach dem
;     Antasten pruefen, bevor gerieben wird.
; ================================================================

BLK FORM 0.1 Z X-100 Y-100 Z-50
BLK FORM 0.2 X+200 Y+150 Z+0


; ---------------------------------------------------------------
; 1) Antasttaster einwechseln
; ---------------------------------------------------------------
TOOL CALL "TS_MESSTASTER" Z                        ; *** Tastername/-nummer anpassen


; ---------------------------------------------------------------
; 2) Ausrichtung ueber die 2x 6H7-Bohrungen antasten - KEINE
;    Grunddrehung: der Zyklus dreht die C-Achse (Tisch) real um den
;    ermittelten Winkel, bis die Bohrungslinie mechanisch fluchtet.
; ---------------------------------------------------------------
CYCL DEF 403 ROT-KOMPENSATION UEBER DREHACHSE ~
   Q263=+0 ;1. BOHRUNGSMITTE 1.ACHSE (X) = Bohrung 1 ~
   Q264=+0 ;1. BOHRUNGSMITTE 2.ACHSE (Y) = Bohrung 1 ~
   Q265=+0 ;2. BOHRUNGSMITTE 1.ACHSE (X) = Bohrung 2 ~
   Q266=+0 ;2. BOHRUNGSMITTE 2.ACHSE (Y) = Bohrung 2 ~
   Q261=-5 ;MESSHOEHE (inkr. zur Werkstueckoberkante) ~
   Q260=+50 ;SICHERE HOEHE (abs.) ~
   Q301=1 ;FAHREN AUF SICHERE HOEHE ZWISCHEN BOHRUNGEN (1=ja) ~
   Q312=6 ;KOMPENSATIONSACHSE (6=C-ACHSE) ~
   Q336=+0 ;SOLLWINKEL BEZUGSACHSE

; *** in Q263/Q264/Q265/Q266/Q261/Q260 die realen Werte eintragen,
; Q312 (Kompensationsachse) gegen Maschinenkinematik pruefen.

; Ab hier ist der Tisch (C-Achse) mechanisch ausgerichtet,
; Basisdrehung im Bezugspunkt bleibt 0.


; ---------------------------------------------------------------
; 3) Bohrung 1 (6H7) erneut antasten -> wird neuer Nullpunkt X0/Y0
; ---------------------------------------------------------------
CYCL DEF 412 BOHRUNG ALS BEZUG ~
   Q321=+0 ;BOHRUNGSMITTE 1.ACHSE (Sollwert Bohrung 1) ~
   Q322=+0 ;BOHRUNGSMITTE 2.ACHSE (Sollwert Bohrung 1) ~
   Q262=6 ;NENN-DURCHMESSER (6H7) ~
   Q325=+0 ;STARTWINKEL ~
   Q247=+90 ;WINKELSCHRITT ~
   Q261=-5 ;MESSHOEHE ~
   Q260=+50 ;SICHERE HOEHE ~
   Q301=1 ;FAHREN AUF SICHERE HOEHE ZWISCHEN PUNKTEN (1=ja) ~
   Q305=0 ;NUMMER IN TABELLE (0 = aktiver Bezugspunkt) ~
   Q331=+0 ;NEUER BEZUGSPUNKT 1.ACHSE (0 = Bohrung 1 wird X0) ~
   Q332=+0 ;NEUER BEZUGSPUNKT 2.ACHSE (0 = Bohrung 1 wird Y0) ~
   Q303=1 ;MESSWERTUEBERTRAGUNG (1=aktiv)

; *** Q321/Q322 muessen mit Q263/Q264 aus Zyklus 403 uebereinstimmen.
; *** Q305/Q303 gegen Zykleneditor pruefen.

; Bohrung 1 liegt jetzt bei X+0 Y+0.


; ---------------------------------------------------------------
; 4) Bohrung 1 (6H7) tiefer reiben
;    Bearbeitungspositionen stehen als Kontur-Label (LBL 1) am Ende
;    des Programms - fuer weitere Positionen dort einfach weitere
;    L X.. Y.. / CYCL CALL Zeilen ergaenzen.
; ---------------------------------------------------------------
TOOL CALL "REIBAHLE_D6H7" Z S1200                  ; *** Werkzeugname/-nummer, Drehzahl anpassen
L Z+100 R0 FMAX M3 M8                              ; Spindel EIN, Kuehlmittel EIN

CYCL DEF 201 REIBEN ~
   Q200=+2 ;SICHERHEITSABSTAND ~
   Q201=-35 ;TIEFE (inkr., neuer/groesserer Wert = "tiefer reiben") ~
   Q206=100 ;VORSCHUB TIEFENZUSTELLUNG ~
   Q211=0.5 ;VERWEILZEIT UNTEN ~
   Q208=250 ;VORSCHUB RUECKZUG ~
   Q203=+0 ;KOORDINATE OBERFLAECHE ~
   Q204=+50 ;2. SICHERHEITSABSTAND

; *** Q201 (Tiefe) und Q203 (Oberflaechenkoordinate) eintragen.

CALL LBL 1

L Z+100 R0 FMAX M5 M9                              ; Spindel HALT, Kuehlmittel AUS
M30


; ---------------------------------------------------------------
; Kontur-Label: Bearbeitungspositionen fuer Zyklus 201
; ---------------------------------------------------------------
LBL 1
L X+0 Y+0 R0 FMAX                                  ; Bohrung 1 = neuer Nullpunkt (aus Zyklus 412)
CYCL CALL
LBL 0

END PGM 6H7_TIEFER_REIBEN MM
