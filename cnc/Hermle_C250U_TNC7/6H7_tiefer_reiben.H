BEGIN PGM 6H7_TIEFER_REIBEN MM

; ================================================================
; Hermle C250U / Steuerung TNC7
;
; Ablauf:
;   1) Antasten von 2 Referenzbohrungen -> Grunddrehung (Zyklus 401)
;   2) 6H7-Bohrung im ausgerichteten System tiefer reiben (Zyklus 201)
;
; ACHTUNG - vor dem ersten Abarbeiten:
;   - Alle mit *** markierten Werte an das reale Werkstueck/die
;     Zeichnung anpassen (Bohrungs-Sollkoordinaten, Bohrungsnummer/
;     Werkzeugdaten, Tiefe, Vorschuebe).
;   - Q-Parameter-Belegung von Zyklus 401 im Klartext-Editor der TNC7
;     gegenpruefen (Hilfefenster zeigt die fuer den installierten
;     NC-Software-Stand aktuellen Feldnamen).
;   - Erstlauf im Testlauf-Grafikmodus bzw. im Einzelsatz mit
;     reduziertem Vorschub und vergroessertem Sicherheitsabstand
;     pruefen.
; ================================================================

BLK FORM 0.1 Z X-100 Y-100 Z-50
BLK FORM 0.2 X+200 Y+150 Z+0


; ---------------------------------------------------------------
; 1) Antasttaster einwechseln
; ---------------------------------------------------------------
TOOL CALL "TS_MESSTASTER" Z                        ; *** Tastername/-nummer anpassen


; ---------------------------------------------------------------
; 2) Grunddrehung ueber 2 Bohrungen antasten
;    (Zyklus tastet beide Bohrungen selbststaendig an)
; ---------------------------------------------------------------
CYCL DEF 401 ROT-KOMPENSATION 2 BOHRUNGEN
   Q263=+30    ;1. BOHRUNGSMITTE 1.ACHSE (X)        *** Sollkoordinate Bohrung 1
   Q264=+30    ;1. BOHRUNGSMITTE 2.ACHSE (Y)        *** Sollkoordinate Bohrung 1
   Q265=+170   ;2. BOHRUNGSMITTE 1.ACHSE (X)        *** Sollkoordinate Bohrung 2
   Q266=+30    ;2. BOHRUNGSMITTE 2.ACHSE (Y)        *** Sollkoordinate Bohrung 2
   Q261=-5     ;MESSHOEHE (inkr. zur Werkstueckoberkante)
   Q260=+50    ;SICHERE HOEHE (abs.)
   Q301=1      ;FAHREN AUF SICHERE HOEHE ZWISCHEN BOHRUNGEN (1=ja)
   Q307=-1     ;GRUNDDREHUNG IN AKTIVEN BEZUGSPUNKT SCHREIBEN (-1=aktiver BP)

; Ab hier ist die Grunddrehung aktiv - alle nachfolgenden
; Koordinaten beziehen sich auf das ausgerichtete Werkstueck.


; ---------------------------------------------------------------
; 3) 6H7-Bohrung tiefer reiben
; ---------------------------------------------------------------
TOOL CALL "REIBAHLE_D6H7" Z S1200                  ; *** Werkzeugname/-nummer, Drehzahl anpassen

L Z+100 R0 FMAX
L X+100 Y+30 R0 FMAX M3                            ; *** Sollposition der 6H7-Bohrung

CYCL DEF 201 REIBEN
   Q200=+2     ;SICHERHEITSABSTAND
   Q201=-35    ;TIEFE (inkr., neuer/groesserer Wert = "tiefer reiben")   *** anpassen
   Q206=100    ;VORSCHUB TIEFENZUSTELLUNG
   Q211=0.5    ;VERWEILZEIT UNTEN
   Q208=250    ;VORSCHUB RUECKZUG
   Q203=+0     ;KOORDINATE OBERFLAECHE                                  *** anpassen
   Q204=+50    ;2. SICHERHEITSABSTAND

CYCL CALL

L Z+100 R0 FMAX M9
M30

END PGM 6H7_TIEFER_REIBEN MM
