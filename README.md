# TonUINO+
Die DIY Musikbox (nicht nur) für Kinder


# Change Log

## Version 1.0 
Portierung des TonUINO Projektes in der Version 2.01 zur Nutzung des BY8X01-16P Moduls und eines USB-Sticks.
Erweiterung um die folgenden Funktionen:
* Vor- und zurückspulen über die Lautstärketasten, wenn pausiert. Vorspulen 12-fach; Zurückspulen 8-fach
* Schutz vor unbeabsichtigter Betätigung der Lautstärketasten (nächster/vorheriger Track), wenn die aktuelle Position im Track größer 5 Minuten ist. Aktion erfolgt erst nach erneuter Bestätigung innerhalb von 3 Sekunden.

# Weitere Hinweise
## Struktur auf dem USB-Stick
Die Struktur der Ordner mit den Audiodateien ist grundsätzlich so wie aus dem TonUINO Projekt bekannt. Es stehen allerdings nur die Ordner 01 - 95 für "Nutzdaten" zur Verfügung. In den Ordnern können bis zu 255 Dateien abgelegt werden. Diese heißen entsprechend 001.mp3 bis 255.mp3.
Der mp3-Ordner wird durch die Ordner 96 - 99 ersetzt, siehe CONVERTED_MP3_FOLDER im Repository. Daher stehen diese nicht für "Nutzdaten" zur Verfügung.
Der advert-Ordner wird nicht mehr benötigt.


