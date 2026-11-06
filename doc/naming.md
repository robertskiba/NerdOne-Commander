# NerdOne Commander – Name und Kennungen

Festgelegt am 06.11.2026. Diese Datei ist verbindlich für alle Stellen, an denen das Programm einen
Namen oder eine technische Kennung verwendet. Die Umsetzung erfolgt schrittweise (siehe `roadmap.md`).

| Stelle | Bisher (Open Salamander) | NerdOne Commander |
|---|---|---|
| Anzeigename | Open Salamander | NerdOne Commander |
| Programmdatei | `salamand.exe` | `nerdone.exe` |
| Konfiguration (portabel) | – | Ordner `config\` neben der EXE |
| Konfiguration (installiert) | `HKCU\Software\Open Salamander\5.0` | `%APPDATA%\NerdOne Commander\` |
| Registry (nur Windows-Integration) | `HKCU\Software\Open Salamander\…` | `HKCU\Software\NerdOne Commander` |
| Mutex, Events, Shared Memory | `AltapSalamander3b…`, `AltapSalamanderFirstInstance`, `AltapSalamanderLoadSaveRegistry`, `SalExten_…` | Präfix `NerdOneCommander.`, z. B. `NerdOneCommander.FirstInstance` |
| Fensterklasse Hauptfenster | `SalamanderMainWindowVer25` | `NerdOneCommander.MainWindow` |
| Trace-Pipe | `altap_traces` | `nerdone_traces` |
| Shell-Erweiterung | `salextx64.dll`, alte CLSIDs | neuer DLL-Name und neue CLSIDs |
| AppUserModelID (Taskleiste, Jump List) | – | `NerdOne.Commander` |
| Plugin-Dateien | `.spl` | `.spl` (unverändert, kompatibel) |
| Sprachdateien | `.slg` | `.slg` (unverändert, kompatibel) |

Eigene Kennungen sorgen dafür, dass NerdOne Commander und Open Salamander nebeneinander laufen,
ohne sich gegenseitig zu stören.
