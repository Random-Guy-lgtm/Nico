BEGIN PGM 6H7_TIEFER_REIBEN MM

; ================================================================
; Hermle C250U / Steuerung TNC7
; Bauteil: Aufnahme, KMW Zg.-Nr. 10114-200120.002.0
;
; Ausgangslage: 2x Bohrung 6H7 (Positionstoleranz zueinander Ø0,01,
; siehe Ansicht E-E der Zeichnung).
;
; Ablauf:
;   1) Bohrung 1 UND Bohrung 2 (beide 6H7) antasten -> Grunddrehung
;      ueber die Verbindungslinie beider Bohrungen (Zyklus 401)
;   2) Bohrung 1 (6H7) erneut antasten -> wird neuer Werkstueck-
;      Nullpunkt X0/Y0 (Zyklus 412)
;   3) Bohrung 1 (jetzt bei X0 Y0) tiefer reiben (Zyklus 201)
;
; ACHTUNG - vor dem ersten Abarbeiten:
;   - Alle Q-Parameter im Kopf des Programms (Q1...Q11) mit den
;     realen Werten aus Zeichnung/CAM belegen. Aktuell sind es
;     Platzhalter!
;   - Q-Parameter-Belegung der Zyklen 401/412 im Klartext-Editor der
;     TNC7 gegenpruefen (Hilfefenster zeigt die fuer den installierten
;     NC-Software-Stand aktuellen Feldnamen/Reihenfolge).
;   - Erstlauf im Testlauf-Grafikmodus bzw. im Einzelsatz mit
;     reduziertem Vorschub und vergroessertem Sicherheitsabstand
;     pruefen. Bei Ø0,01-Toleranz unbedingt Messprotokoll nach dem
;     Antasten pruefen, bevor gerieben wird.
; ================================================================

; ---------------------------------------------------------------
; Q-Parameter-Tabelle - hier die realen Werte eintragen
; ---------------------------------------------------------------
Q1  = +0     ;BOHRUNG 1 (6H7, wird Nullpunkt + tiefer gerieben): X-SOLLKOORDINATE   *** eintragen
Q2  = +0     ;BOHRUNG 1: Y-SOLLKOORDINATE                                          *** eintragen
Q3  = +0     ;BOHRUNG 2 (6H7, nur fuer Grunddrehung): X-SOLLKOORDINATE             *** eintragen
Q4  = +0     ;BOHRUNG 2: Y-SOLLKOORDINATE                                          *** eintragen
Q5  = +50    ;SICHERE HOEHE Z (abs.)                                               *** pruefen
Q6  = -5     ;MESSHOEHE Z (inkr. zur Werkstueckoberkante)                          *** pruefen
Q7  = 6      ;NENN-DURCHMESSER BOHRUNG (6H7)
Q10 = +0     ;NEUE REIBTIEFE Z (inkr., negativ = "tiefer reiben")                  *** eintragen
Q11 = +0     ;KOORDINATE OBERFLAECHE Z (abs.)                                      *** eintragen


; ---------------------------------------------------------------
; 1) Antasttaster einwechseln
; ---------------------------------------------------------------
TOOL CALL "TS_MESSTASTER" Z                        ; *** Tastername/-nummer anpassen


; ---------------------------------------------------------------
; 2) Grunddrehung ueber die 2x 6H7-Bohrungen antasten
;    (Zyklus tastet beide Bohrungen selbststaendig an, bezogen auf
;     den aktuell aktiven, noch nicht korrigierten Nullpunkt)
; ---------------------------------------------------------------
CYCL DEF 401 ROT-KOMPENSATION 2 BOHRUNGEN
   Q263=+Q1    ;1. BOHRUNGSMITTE 1.ACHSE (X) = Bohrung 1
   Q264=+Q2    ;1. BOHRUNGSMITTE 2.ACHSE (Y) = Bohrung 1
   Q265=+Q3    ;2. BOHRUNGSMITTE 1.ACHSE (X) = Bohrung 2
   Q266=+Q4    ;2. BOHRUNGSMITTE 2.ACHSE (Y) = Bohrung 2
   Q261=+Q6    ;MESSHOEHE
   Q260=+Q5    ;SICHERE HOEHE
   Q301=1      ;FAHREN AUF SICHERE HOEHE ZWISCHEN BOHRUNGEN (1=ja)
   Q307=-1     ;GRUNDDREHUNG IN AKTIVEN BEZUGSPUNKT SCHREIBEN (-1=aktiver BP)

; Ab hier ist die Grunddrehung aktiv.


; ---------------------------------------------------------------
; 3) Bohrung 1 (6H7) erneut antasten -> wird neuer Nullpunkt X0/Y0
; ---------------------------------------------------------------
CYCL DEF 412 BOHRUNG ALS BEZUG
   Q321=+Q1    ;BOHRUNGSMITTE 1.ACHSE (Sollwert Bohrung 1)
   Q322=+Q2    ;BOHRUNGSMITTE 2.ACHSE (Sollwert Bohrung 1)
   Q262=Q7     ;NENN-DURCHMESSER
   Q325=+0     ;STARTWINKEL
   Q247=+90    ;WINKELSCHRITT
   Q261=+Q6    ;MESSHOEHE
   Q260=+Q5    ;SICHERE HOEHE
   Q301=1      ;FAHREN AUF SICHERE HOEHE ZWISCHEN PUNKTEN (1=ja)
   Q305=0      ;NUMMER IN TABELLE (0 = aktiver Bezugspunkt)             *** pruefen
   Q331=+0     ;NEUER BEZUGSPUNKT 1.ACHSE (0 = Bohrung 1 wird X0)
   Q332=+0     ;NEUER BEZUGSPUNKT 2.ACHSE (0 = Bohrung 1 wird Y0)
   Q303=1      ;MESSWERTUEBERTRAGUNG (1=aktiv)                         *** pruefen

; Bohrung 1 liegt jetzt bei X+0 Y+0.


; ---------------------------------------------------------------
; 4) Bohrung 1 (6H7) tiefer reiben
; ---------------------------------------------------------------
TOOL CALL "REIBAHLE_D6H7" Z S1200                  ; *** Werkzeugname/-nummer, Drehzahl anpassen

L Z+100 R0 FMAX
L X+0 Y+0 R0 FMAX M3                               ; Bohrung 1 = neuer Nullpunkt

CYCL DEF 201 REIBEN
   Q200=+2     ;SICHERHEITSABSTAND
   Q201=+Q10   ;TIEFE (inkr., neuer/groesserer Wert = "tiefer reiben")
   Q206=100    ;VORSCHUB TIEFENZUSTELLUNG
   Q211=0.5    ;VERWEILZEIT UNTEN
   Q208=250    ;VORSCHUB RUECKZUG
   Q203=+Q11   ;KOORDINATE OBERFLAECHE
   Q204=+50    ;2. SICHERHEITSABSTAND

CYCL CALL

L Z+100 R0 FMAX M9
M30

END PGM 6H7_TIEFER_REIBEN MM
