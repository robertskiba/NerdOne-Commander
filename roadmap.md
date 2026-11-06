# NerdOne Commander – Roadmap

Basis: Fork von Open Salamander 5.0 (GPLv2).

Zielplattform: Windows 11, wenn möglich auch Windows 10.

## Grundsätze

- Es wird ausschließlich quelloffene Software verwendet. Auch optionale Windows-Komponenten wie WIC werden nicht genutzt.
- Ausnahme: Es gibt keine vernünftige quelloffene Lösung.

## Phase 1 – Code prüfen

- [ ] Bestandsaufnahme von Codebasis, Projektstruktur und Build-System

## Phase 2 – Auf Visual Studio 2026 aktualisieren

- [x] Projekte auf Platform Toolset v145 (VS2026) umgestellt (Branch `vs2026`)
- [ ] Visual Studio 2026 mit C++-Workload und Windows SDK installieren
- [ ] Solution mit VS2026 bauen und Build-Fehler beheben
- [ ] Weiterentwicklung ab hier mit VS2026

## Phase 3 – Alte Zöpfe abschneiden

- [x] Ermitteln, was obsolet ist
- [x] Entscheiden, was entfernt wird (10 Punkte freigegeben am 06.11.2026; CheckVer bleibt)
- [ ] Freigegebene Punkte entfernen

## Cloud-Speicher und Fernverbindungen

- [ ] Einheitliche Anbindung von Cloud-Speichern, SFTP, FTP und weiteren
- [ ] Lokale Cloud-Clients (Dropbox, OneDrive usw.) erkennen und automatisch in den Panel-Listen anzeigen
- [ ] Fernverbindung zu Cloud-Speichern, wo möglich (z. B. Nextcloud per API/WebDAV)
- [ ] Zugriff per WebDAV auf Nextcloud und kompatible Clouds
- [ ] SSH und SFTP
- [ ] Nach und nach Treiber für weitere Systeme hinzufügen

## Neue Funktionen

- [ ] Update-Prüfung (CheckVer) auf das eigene GitHub-Repo umstellen
- [ ] Portabler Modus: ohne Installation lauffähig, möglichst als eine EXE
- [ ] Bildbetrachter: Ersatz für die fehlende PictView-Engine, ausschließlich OSS (ohne WIC)
- [ ] Bildbetrachter unterstützt alle aktuellen Bildformate, die ein Mobiltelefon liefern kann (z. B. HEIC)
- [ ] Viewer für PDF, HTML und Markdown mit PDFium, litehtml und md4c
- [ ] RAR-Plugin von der proprietären unrar.dll auf den quelloffenen RAR-Entpacker von 7-Zip umstellen

## Fehler prüfen

- [ ] Absturz beim Anzeigen von CSV-Dateien (bekannt aus dem letzten Altap Salamander)
- [ ] Hotpath 0 lässt sich belegen, aber nicht mit Strg+0 aufrufen

## Dateisystem-Kompatibilität

- [ ] Alle unter Windows 11 erlaubten Zeichen in Datei- und Ordnernamen
- [ ] Alle unter Windows 11 erlaubten Pfadlängen
- [ ] Symlinks zuverlässig unterstützen
