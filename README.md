ADS1220 Library für STM32 NUCLEO-WL55JC1/2

Dieses Repository enthält eine leichtgewichtige, modular aufgebaute Library für den ADS1220 ADC (Texas Instruments), angepasst für das STM32 NUCLEO-WL55JC1/2 Evaluierungsboard.
Die Bibliothek stellt alle wichtigen Funktionen bereit, um den ADS1220 zu initialisieren, zu konfigurieren und Messwerte auszulesen.

Voraussetzung: STM32Cube / STM32-Infrastruktur

Die Library setzt auf die STM32 HAL-Infrastruktur, die nicht direkt in diesem Repository enthalten ist.
Um das Projekt verwenden zu können, benötigst du:

die STM32CubeIDE oder ein eigenes Projekt mit korrekt generierten HAL-Treibern,

SPI-Konfiguration für den ADS1220,

passende GPIO-Definitionen (DRDY, RESET, CS etc.).

Die vollständige Infrastruktur lässt sich automatisch generieren, indem du in der STM32CubeIDE ein neues Projekt für das NUCLEO-WL55JC1/2 anlegst und deine gewünschten Peripherie-Einstellungen vornimmst.

Projektaufbau

Das Repository enthält folgende zentrale Dateien:

ADS1220.h – Header-Datei der Library
Enthält Registerdefinitionen, Konstanten, Funktionsprototypen und Konfigurationsstrukturen.

ADS1220.c – Implementierung der Library
Enthält die komplette Logik zur Initialisierung, Registerkonfiguration und Datenerfassung.

main.c – Minimalbeispiel
Eine sehr einfache main(), in der lediglich zu Testzwecken die ADS1220-Funktionen aufgerufen werden (z. B. Initialisierung und Messwertabfrage).

Test & Beispielverwendung

Die main.c dient nur als rudimentäres Testbeispiel, um die Grundfunktionalität zu prüfen.
Hier wird kein vollständiger Applikationscode implementiert – lediglich ein einfacher Funktionsaufruf der ADS1220-Library.

Ziel

Diese Library soll als Grundlage dienen, um den ADS1220 auf dem NUCLEO-WL55JC1/2 schnell und sauber in eigene Projekte einzubetten.
Der Fokus liegt auf:

einfacher Integration,

klar strukturierter Registerkommunikation,

minimalem Overhead,

guter Lesbarkeit.
