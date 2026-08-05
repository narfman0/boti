# Development Plan

Solo dev. Goal: a vertical slice (one level, two enemy types, full combat feel) before expanding.

## Phase 0 — Setup (Weeks 1–2)

- [ ] Install UE 5.8 via Epic launcher (Heroic on Linux)
- [ ] Create UE project in `~/workspace/boti`
- [ ] Enable UnrealMCP plugin, verify Claude Code connects via local MCP socket
- [ ] Push initial project structure to git

## Phase 1 — Core Combat Loop (Weeks 3–8)

**Goal: make the immortality mechanic feel right. Nothing else matters until this is fun.**

- [ ] Manji character — placeholder mesh, basic attack/parry/dodge
- [ ] HP system + downed state + revive loop (bloodworm VFX placeholder)
- [ ] Limb-loss: arm detachment, weapon drop, reattach timer
- [ ] Basic enemy AI: attacks, reacts to limb state
- [ ] Posture system (posture bar, break state, riposte)
- [ ] Camera: over-shoulder, lock-on, combat zoom

## Phase 2 — World & Enemy AI (Weeks 9–16)

- [ ] First environment: village outskirts / courtyard (combat arena scale)
- [ ] Enemy roster MVP: grunt + one named lieutenant with unique weapon style
- [ ] Behavior Trees: flanking, limb-state awareness, morale (flinch on revive)
- [ ] Rin companion: follow logic, idle reactions

## Phase 3 — Narrative & Rin (Weeks 17–22)

- [ ] Sequencer cutscenes for act 1 (rough animatic quality is fine)
- [ ] Dialogue system: in-world subtitles + voiced lines (placeholder VO)
- [ ] Act 1 complete: opening → one lieutenant fight → Anotsu escape

## Phase 4 — Vertical Slice Polish (Weeks 23–28)

- [ ] Animation pass: hit reactions, revive, limb crawl
- [ ] Audio: ambience, impact SFX, koto/shamisen score placeholder
- [ ] Performance pass: Nanite/Lumen scalability settings
- [ ] Playtest loop: iterate on combat feel until it's demo-ready

## MCP Usage Notes

The UE MCP plugin accelerates:
- Blueprint node wiring (combat state machine, AI behavior trees)
- Bulk actor placement and level dressing
- Material iteration via natural language
- Engine API lookups during development

Keep a manual fallback for everything — MCP is still experimental in 5.8.
