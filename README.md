# boti — Blade of the Immortal (fan game)

A dark samurai action RPG built in Unreal Engine 5.8. Third-person, souls-adjacent combat set in late Edo Japan (~1782). You play as Manji, a cursed immortal swordsman escorting Rin Asano as she hunts the Ittō-ryū — the rogue sword school that murdered her parents.

**Status:** Pre-production. Engine not yet installed; repo initialized.

## Concept

The immortality mechanic is the whole game. Manji cannot die — sacred bloodworms heal any wound — but limb loss is real and tactical. Lose an arm and you lose the weapon in that hand until the limb crawls back. Enemies know this and exploit it. Combat is brutal, deliberate, and asymmetric.

Tone: *Blade of the Immortal* manga by Hiroaki Samura. Dark, philosophical, morally grey. This is a fan project — non-commercial.

## Engine & Tools

- **Unreal Engine 5.8** — Nanite + Lumen for visuals, native MCP plugin for AI-assisted development
- **Claude Code via MCP** — drives the editor for Blueprint authoring, actor placement, material iteration
- **Git + Git LFS** — version control; binary assets tracked via LFS

## Docs

- [Design Overview](docs/design.md) — core mechanics, immortality system, combat
- [Development Plan](docs/plan.md) — phased roadmap from prototype to vertical slice

## Getting Started

Prerequisites: Unreal Engine 5.8 (via Epic Games Launcher / Heroic on Linux)

```bash
git clone <repo-url>
cd boti
# Open boti.uproject in the UE editor once the project file exists
```
