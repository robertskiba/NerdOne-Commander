# Changelog

Alle nennenswerten Änderungen an NerdOne Commander werden in dieser Datei dokumentiert.
Das Format orientiert sich an [Keep a Changelog](https://keepachangelog.com/de/1.1.0/).
Bei jedem Release wird der Abschnitt „Unveröffentlicht“ zur neuen Versionsnummer.

## [Unveröffentlicht]

Basis: Open Salamander 5.0 (Stand `a201fa3`).

### Geändert

- Build auf Visual Studio 2026 umgestellt: alle Projekte nutzen Platform Toolset v145.
- `build.cmd`, `rebuild.cmd` und `sfxmake.bat` finden MSBuild über `vswhere` (VS2026 oder neuer) statt über einen fest eingetragenen VS2022-Pfad.
- CI-Build (GitHub Actions) läuft auf `windows-2025` mit VS2026.
- Absturzmelder (Bug Reporter): Berichte werden nur noch lokal als 7z-Archiv gespeichert und der Ordner wird geöffnet. Es wird nichts mehr an einen Server gesendet.
- Links auf altap.cz (Über-Dialog, Forum-Menü, Beta-Hinweis, weitere Sprachen, Hinweistexte) zeigen auf das GitHub-Repository von NerdOne Commander.
- Standard-Viewer für HTML/XML/MHT zeigt nach dem Wegfall des PAK-Plugins weiterhin auf den IE-Viewer (Index der Standard-Plugins angepasst).

### Entfernt

- WinSCP-Plugin (Borland C++ Builder 6, nicht mit MSVC baubar, nicht in der Solution).
- Windows-Mobile-Plugin (ActiveSync/RAPI).
- PAK-Plugin (Quake-Archive) samt Standard-Archivzuordnung.
- Demo-Plugins (demomenu, demoplug, demoview) aus Solution und Hilfe; die Quellen bleiben als Beispiele erhalten.
- Veraltete Projektkopie `plugins/portables/vcproj2015_manison`.
- Upload von Absturzberichten an `reports.altap.cz` (unverschlüsseltes HTTP).
- Import von Konfigurationen aus Salamander-Versionen vor 4.0 (81 alte Registry-Pfade).
- Werkzeuge für die abgeschlossene Übersetzung tschechischer Kommentare (`tools/comments`, Python-Projektdateien) und die Skripte `!clean_all_interm.cmd` / `!clean_src_interm.cmd`.
