# arduino-rfid-scanner — Quick Reference

> Full context, architecture and rules: [AGENTS.md](AGENTS.md). Read it first.

## Run

```bash
# No CLI run command. Manual:
# 1. Copy mis/arduino_secrets.h.example to mis/arduino_secrets.h and fill in Wi-Fi + server host/port
# 2. Open mis/mis.ino in the Arduino IDE, board "Arduino UNO R4 WiFi", click Upload
# 3. Open Serial Monitor @ 9600 (set DEBUG_SERIAL 1 to see LOG_* output)
```

## Common commands

```bash
# Build/verify: Arduino IDE -> Verify (UNO R4 WiFi). No test or lint command exists.
# Optional, only if arduino-cli is installed:
arduino-cli compile --fqbn arduino:renesas_uno:unor4wifi mis
# Manual test: scan a card with yellow ON (POST /api/scan) and yellow OFF (POST /api/rfid/scan); check Serial Monitor + LEDs
```

## Ports

| Service | Port |
|---|---|
| arduino-rfid-scanner | none (device; no port of its own) |
| mis-backend (called by the device) | 8000 |

## Agents

- `rfid-developer` implements a plan: `claude --agent rfid-developer -p "Implement .claude/common/plans/active/<slug>/plan-rfid.md"`
- `rfid-auditor` audits staged changes before commit

Shared plans/rules/wiki come from the `mis` workspace via the `.claude/common` and `.claude/diagrams` symlinks. If they're broken, run `node tools/setup-links.js` in the workspace.
