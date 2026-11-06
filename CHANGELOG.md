# Changelog

Alle nennenswerten Änderungen an NerdOne Commander werden in dieser Datei dokumentiert.
Das Format orientiert sich an [Keep a Changelog](https://keepachangelog.com/de/1.1.0/).
Bei jedem Release wird der Abschnitt „Unveröffentlicht“ zur neuen Versionsnummer.

## [Unveröffentlicht]

Basis: Open Salamander 5.0 (Stand `a201fa3`).

### Geändert

- Programm heißt jetzt **NerdOne Commander** (Version 0.1.0-alpha), siehe `doc/naming.md`:
  - Programmdatei `nerdone.exe` statt `salamand.exe`.
  - Sichtbarer Name in Fenstertitel, Menüs, Dialogen, Meldungen und Versionsinfos; der Über-Dialog nennt Open Salamander als Grundlage.
  - Eigene interne Kennungen (Mutex, Events, Shared Memory, Fensterklasse) mit Präfix `NerdOneCommander.` und eine neue CLSID für die Shell-Erweiterung, damit NerdOne Commander und Open Salamander nebeneinander laufen.
  - Konfiguration liegt (bis zur Umstellung auf Dateien) unter `HKCU\Software\NerdOne Commander`; Einstellungen von Open Salamander 5.0 und Altap Salamander 4.0 lassen sich importieren.
  - Benutzerdaten (Absturzberichte, Konfigurations-Export, Skripte) liegen unter `%APPDATA%\NerdOne Commander` bzw. `%LOCALAPPDATA%\NerdOne Commander`.
  - Intern bleiben die Plugin-API-Version (Salamander 5.0, API 103) und die Konfigurationsversion unverändert, damit vorhandene Plugins weiter laden.
- Konfigurationen von Open Salamander und Altap Salamander werden beim Import nie gelöscht; entfernt werden können nur noch eigene Konfigurationen.
- Build auf Visual Studio 2026 umgestellt: alle Projekte nutzen Platform Toolset v145.
- `build.cmd`, `rebuild.cmd` und `sfxmake.bat` finden MSBuild über `vswhere` (VS2026 oder neuer) statt über einen fest eingetragenen VS2022-Pfad.
- CI-Build (GitHub Actions) läuft auf `windows-2025` mit VS2026, nur noch für x64, und startet jetzt auch bei jedem Push.
- CI baut Debug und Release und stellt aus dem Release-Build ein portables ZIP samt Debug-Symbolen als Download bereit.

### Hinzugefügt

- `tools/make_portable.ps1` erstellt aus einem Release-Build das portable ZIP: Programmdateien, Visual-C++-Laufzeit (app-lokal), Zeichensatz-Tabellen, Toolbar-Grafiken, Beispielskripte, Lizenz und Changelog. PictView und UnRAR sind nicht enthalten, weil ihnen die (nicht quelloffenen) Engines fehlen.
- Mindestversion ist Windows 10: `WINVER`, `_WIN32_WINNT` und `_WIN32_IE` stehen in allen Projekten auf `0x0A00` (Ausnahme: der weiterhin 32-bittige SFX-Stub des ZIP-Plugins).
- Nur noch 64-Bit-Builds: Die Plattform Win32 ist aus der Solution und allen Projekten entfernt. tserver, translator, sfx7zip und zip2sfx werden jetzt als x64 gebaut.
- Build-, Hilfe- und Kopier-Skripte bedienen nur noch die Zielordner `Debug_x64` und `Release_x64`.
- Programmstart verlangt Windows 10 oder neuer (vorher Windows 7).
- Die Shell-Erweiterung bringt eigene `strcpy`/`strlen` mit, weil sie ohne C-Laufzeitbibliothek gelinkt wird und VS2026 diese Funktionen nicht mehr immer als Intrinsics einsetzt.
- Debug-Builds verzichten auf die Laufzeitprüfung `/RTCc` (Smaller Type Check), die die Standardbibliothek von VS2026 nicht mehr unterstützt.
- APIs, die es auf jedem Windows 10 gibt, werden direkt aufgerufen statt per `GetProcAddress` geladen (u. a. `ShutdownBlockReasonCreate`, `GetNativeSystemInfo`, `SHGetKnownFolderPath`, `SHOpenFolderAndSelectItems`, `SHGetStockIconInfo`, WTS-Funktionen).
- Wiederverbinden von Netzlaufwerken nutzt nur noch den System-Dialog (`CONNECT_INTERACTIVE`) statt des alten CredUI-Pfads.
- Der OneDrive-Pfad wird direkt über `SHGetKnownFolderPath` ermittelt.
- Kopien im selben Ordner heißen immer nach dem Schema „Name - Kopie (n)“; das Windows-XP-Schema „Kopie von …“ entfällt.
- Nur noch die 64-Bit-Shell-Erweiterung `salextx64.dll` wird registriert.
- Quellcode bricht beim Übersetzen für 32 Bit mit einer Fehlermeldung ab (`precomp.h`).
- Absturzmelder (Bug Reporter): Berichte werden nur noch lokal als 7z-Archiv gespeichert und der Ordner wird geöffnet. Es wird nichts mehr an einen Server gesendet.
- Links auf altap.cz (Über-Dialog, Forum-Menü, Beta-Hinweis, weitere Sprachen, Hinweistexte) zeigen auf das GitHub-Repository von NerdOne Commander.
- Standard-Viewer für HTML/XML/MHT zeigt nach dem Wegfall des PAK-Plugins weiterhin auf den IE-Viewer (Index der Standard-Plugins angepasst).

### Entfernt

- WinSCP-Plugin (Borland C++ Builder 6, nicht mit MSVC baubar, nicht in der Solution).
- Windows-Mobile-Plugin (ActiveSync/RAPI).
- PAK-Plugin (Quake-Archive) samt Standard-Archivzuordnung.
- Demo-Plugins (demomenu, demoplug, demoview) aus Solution und Hilfe; die Quellen bleiben als Beispiele erhalten.
- Veraltete Projektkopie `plugins/portables/vcproj2015_manison`.
- Code für Windows XP, Vista, 7, 8 und 8.1: Versions-Flags (`WindowsVistaAndLater` usw.) und alle davon abhängigen Altpfade, im Kern und in den Plugins (ftp, regedt, undelete, nethood, checksum, automation, renamer, diskmap, filecomp u. a.).
- Win7-WOW64-Workaround `EnableExceptionsOn64` (`Get/SetProcessUserModeExceptionPolicy`) in allen Hilfsprogrammen außer dem PictView-Envelope.
- 32-Bit-Codepfade: `#ifndef _WIN64`-Blöcke, x86-Inline-Assembler, x86-Registerausgabe im Absturzbericht und die WOW64-Umleitung von System32/SysWOW64/Sysnative.
- Sonderbehandlung für das in x64 fehlende WinSCP-Plugin und der Registry-Wert „Add x86-Only Plugins“.
- 32-Bit-Shell-Erweiterung `salextx86`, das 32-Bit-Hilfsprogramm `salpvenv` (PictView), `x86.props` und `signslgs_release_x86.cmd`.
- Upload von Absturzberichten an `reports.altap.cz` (unverschlüsseltes HTTP).
- Import von Konfigurationen aus Salamander-Versionen vor 4.0 (81 alte Registry-Pfade).
- Werkzeuge für die abgeschlossene Übersetzung tschechischer Kommentare (`tools/comments`, Python-Projektdateien) und die Skripte `!clean_all_interm.cmd` / `!clean_src_interm.cmd`.
