BEGIN PGM PLANFRAESEN_65X11 MM

; ================================================================
; Hermle C250U / Steuerung TNC7
; Planfraesen mit Schlichtaufmass 0,5 mm (Zyklus 232 STIRNFRAESEN)
;
; Teil: X65 x Y11, Fertighoehe Z-40 (Gesamthoehe ab Oberflaeche)
; Nullpunkt: X0/Y0 auf der FERTIGEN Oberflaeche (Z0), Ecke vorne-links
;
; ACHTUNG - vor dem ersten Abarbeiten:
;   - Rohteil-Aufmass oberhalb Z0 (BLK FORM 0.2 Z sowie Q227) an das
;     reale Rohteil anpassen - hier nur mit 1 mm angenommen!
;   - Werkzeugdaten (Name/-nummer, Durchmesser, Drehzahl, Vorschuebe)
;     an den tatsaechlichen Planfraeser anpassen.
;   - Q218/Q219 (Seitenlaengen) ggf. um ein Ueberlaufmass vergroessern,
;     falls der Fraeser die Kanten sonst nicht sauber freifaehrt.
;   - Erstlauf im Testlauf-Grafikmodus bzw. im Einzelsatz mit
;     reduziertem Vorschub pruefen.
; ================================================================

BLK FORM 0.1 Z X+0 Y+0 Z-40
BLK FORM 0.2 X+65 Y+11 Z+1                         ; *** Rohteil-Aufmass anpassen


; ---------------------------------------------------------------
; Werkzeug: Planfraeser
; ---------------------------------------------------------------
TOOL CALL "PLANFRAESER" Z S4000                    ; *** Werkzeugname/-nummer, Drehzahl anpassen
L Z+50 R0 FMAX M3                                  ; Spindel EIN
M8                                                  ; Kuehlmittel EIN


; ---------------------------------------------------------------
; Planfraesen: schruppt automatisch bis Q369 (Schlichtaufmass) und
; schlichtet danach mit Vorschub Q385 auf Endmass Q228
; ---------------------------------------------------------------
CYCL DEF 232 STIRNFRAESEN ~
   Q225=+0 ;STARTPUNKT 1.ACHSE ~
   Q226=+0 ;STARTPUNKT 2.ACHSE ~
   Q227=+1 ;STARTPUNKT WERKZEUGACHSE (Rohteil-Oberkante) ~
   Q218=65 ;1. SEITENLAENGE (X) ~
   Q219=11 ;2. SEITENLAENGE (Y) ~
   Q228=+0 ;ENDMASS WERKZEUGACHSE (fertige Oberflaeche) ~
   Q202=1 ;MAX. ZUSTELLUNG ~
   Q369=0.5 ;AUFMASS FUER SCHLICHTEN ~
   Q370=1 ;BAHNUEBERLAPPUNG ~
   Q207=800 ;VORSCHUB FRAESEN (Schruppen) ~
   Q385=500 ;VORSCHUB SCHLICHTEN ~
   Q253=2000 ;VORSCHUB VORPOSITIONIEREN ~
   Q200=+2 ;SICHERHEITSABSTAND ~
   Q204=+50 ;2. SICHERHEITSABSTAND ~
   Q351=+1 ;FRAESART (+1=Gleichlauf, -1=Gegenlauf)

; *** Q202/Q207/Q385/Q253 an Werkzeug und Material anpassen.

CYCL CALL

L Z+100 R0 FMAX M5                                 ; Spindel HALT
M9                                                  ; Kuehlmittel AUS
M30

END PGM PLANFRAESEN_65X11 MM
