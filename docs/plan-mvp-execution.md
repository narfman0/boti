# MVP Execution Plan

Concrete sprint-by-sprint breakdown for building the MVP. MCP handles Blueprint wiring, actor placement, and material setup. Human reviews output and handles animation retargeting, FBX imports, and feel tuning.

---

## Assets Available Now (srv)

No Synty Samurai pack yet. Substitute plan:

| Need | Use Now | Replace Later |
|------|---------|---------------|
| Characters | UE5 Mannequin | Synty Samurai (purchase) |
| Buildings / props | POLYGON_Fantasy_Kingdom | Synty Samurai |
| Outdoor nature | POLYGON_NatureBiomes_MeadowForest | Synty Samurai / Nature |
| Character placeholder meshes | POLYGON_Fantasy_Characters | Synty Samurai |
| Blockout geometry | POLYGON_Prototype | Keep for blockout phase |

**Action item:** Buy Synty Samurai pack. All systems built on Mannequin will retarget cleanly.

---

## Sprint Plan

### Sprint 1 — Character Foundation (Days 1–2)

**Goal:** Manji runs, dodges, and swings. Feels responsive.

Tasks:
- [ ] Set up `BP_Manji` from UE5 `BP_ThirdPersonCharacter` base
- [ ] Configure camera: over-shoulder offset, FOV 75, spring arm length 300
- [ ] Implement lock-on system: `AC_LockOn` component, nearest-enemy scan, camera blend
- [ ] Locomotion state machine: Idle / Walk / Run / Dodge (8-directional)
- [ ] Dodge roll: 12-frame i-frame window, root-motion distance ~180cm, 0.4s cooldown
- [ ] Light attack: 3-hit combo chain using Animation Montage sections (A1→A2→A3)
- [ ] Melee trace: `UAnimNotify_MeleeTrace` fires weapon sweep per hit frame
- [ ] Heavy attack: single montage, 1.2s startup, high posture damage multiplier

**MCP tasks:** `BP_Manji` blueprint wiring, `AC_LockOn` component setup, animation notify placement

---

### Sprint 2 — Combat Core (Days 3–5)

**Goal:** Parry, riposte, and posture break all work and feel satisfying.

Tasks:
- [ ] `AC_Posture` component: float PostureMax/Current, regen timer, break state
- [ ] Parry input: 8-frame window (Input → `bParryActive` flag)
- [ ] Parry success: detect incoming hit during window → enemy enters `Stagger` state (1.5s)
- [ ] Stagger VFX: parry clash spark Niagara burst at impact point
- [ ] Riposte: available input during stagger window → high-damage single strike
- [ ] Posture break: `AC_Posture` hits zero → `E_CombatState::PostureBreak` → 2s execution window
- [ ] Visceral kill: execution montage plays on target, deals lethal damage
- [ ] Block: hold input reduces damage 70%, costs posture per hit
- [ ] Enemy hit reactions: light flinch / heavy stagger driven by hit magnitude

**MCP tasks:** `AC_Posture` wiring, parry detection logic in `BP_Manji`, enemy hit reaction state machine

---

### Sprint 3 — Immortality Loop (Days 6–7)

**Goal:** Dying, going down, and reviving feels tense, not frustrating.

Tasks:
- [x] `AC_Health` component: HP float, death/downed delegates (`Components/AC_Health.h/.cpp`)
- [x] HP → 0: trigger `OnDowned` → 5s countdown → auto-revive (C++ timer in AC_Health)
- [ ] Bloodworm VFX: Niagara system, worm-crawl ribbons across wound — assign `BloodwormVFX` in BP_Manji (placeholder particle in editor)
- [x] Revive: at 5s expiry → HP set to 30% → `OnRevived` delegate broadcast; Blueprint plays get-up montage
- [ ] Enemy downed-react: `EnemyBTTask_DownedReact` — grunt backs off 1s, elite closes in (Sprint 4 BT work)
- [ ] Downed HUD: full-screen darkening vignette + countdown widget — bind `WBP_DownedOverlay` to `HealthComponent->OnDowned`; poll `GetDownedTimeRemaining()` for the timer display
- [x] Death stub: `bOutOfBloodworms` flag present on AC_Health; `OnDeath` delegate wired but never fired in MVP

**MCP tasks (editor):** Assign `BloodwormVFX` placeholder in BP_Manji details panel; create `WBP_DownedOverlay` widget (vignette + countdown text, bound to HealthComponent delegates)

---

### Sprint 4 — Enemy AI (Days 8–11)

**Goal:** Grunts feel like credible threats; Shira and Anotsu feel like bosses.

Tasks:

**Grunt (`BP_Enemy_Grunt`):**
- [ ] BT_Grunt: Patrol → Detect (perception) → Approach → AttackLoop (2 hits, back off 1.5s)
- [ ] Attack montage: 2-hit katana pattern, telegraph windup (0.3s red flash on weapon)
- [ ] Spear variant: longer attack range, slower combo

**Shira (`BP_Enemy_Shira`):**
- [ ] BT_Shira: inherits elite tree + `DownedAggression` task (closes distance on downed event)
- [ ] Phase: single phase, aggressive close-range heavy attacks
- [ ] Arm loss cosmetic: if riposted on posture break → detach arm mesh, continue fight (one attack set)
- [ ] Defeat: ragdoll + blood burst Niagara

**Anotsu (`BP_Enemy_Anotsu`):**
- [ ] BT_Anotsu Phase 1: methodical, halberd range exploitation, punishes early aggression
- [ ] Phase 2 trigger at 40% HP: `SetPhase(2)` → flurry combo unlocked, attack speed +30%
- [ ] Escape trigger at HP → 0: stop BT, play escape montage → fade to cutscene

**Shared:**
- [ ] `AC_EnemyPerception` (sight + sound)
- [ ] `AC_Posture` on all enemies (same component, tuned per enemy)
- [ ] Death ragdoll + blood decal stamp

**MCP tasks:** all behavior tree construction, perception component wiring, phase-gate logic

---

### Sprint 5 — Level Blockout (Days 12–15)

**Goal:** Playable path through all 5 encounters.

Arenas (BSP/Geometry brush blockout first, dress second):

| Arena | Size | Key feature |
|-------|------|-------------|
| The Road | 30×8m corridor | Entry gate, 2 grunt spawns |
| Ambush | 20×20m widening | Rock cover, tree line, 3+1 spawns |
| Courtyard | 40×30m open | Shrine walls, raised archer platform |
| Bridge | 6×25m narrow | No dodge room, forces parry |
| Dojo | 25×15m interior | Pillars for phase 2 flanking |

Tasks:
- [ ] Blockout all 5 arenas + 4 connecting corridors in single persistent level
- [ ] Arena lock gates: `BP_ArenaGate` — closes on encounter trigger, opens on all enemies dead
- [ ] Patrol paths: `BP_PatrolPath` splines assigned per enemy
- [ ] Encounter triggers: `BP_EncounterTrigger` volumes, spawn wave on player enter
- [ ] Import POLYGON_Fantasy_Kingdom / MeadowForest to srv → migrate to project
- [ ] Dress: road props, shrine architecture, dojo interior columns, lanterns
- [ ] Skybox: UE5 default sky + directional light, desaturated post-process (low saturation, high contrast)
- [ ] Rin placeholder: static mannequin at dojo entrance, no interaction

**MCP tasks:** bulk actor placement, patrol path assignment, material instance creation for desaturated look

---

### Sprint 6 — Boss & Ending (Days 16–19)

**Goal:** Anotsu fight plays through both phases; game ends cleanly.

Tasks:
- [ ] Anotsu arena activation: cinematic camera push-in on enter, then gameplay resumes
- [ ] Phase 2 VFX: screen shake + red vignette flash on phase transition
- [ ] Escape cutscene (Sequencer):
  - Anotsu HP → 0 → Manji riposte montage starts
  - Mid-riposte: Anotsu breaks free (camera cut)
  - Anotsu runs to back wall, smashes through shoji screen
  - Manji stumbles forward, kneels
  - Rin appears beside him, rests hand on shoulder
  - Fade to black → "To be continued." white text on black → credits (name only)
  - Total runtime: ~30 seconds
- [ ] Win state: cutscene plays, level unloads, main menu returns

**MCP tasks:** Sequencer track setup, camera cut timing, fade-to-black material

---

### Sprint 7 — HUD, VFX, Polish (Days 20–23)

**Goal:** Screen reads clearly; hits feel punchy.

Tasks:
- [ ] `WBP_HUD`: Manji HP bar (top-left, red), Posture bar (below HP, gold), appears only when damaged
- [ ] Enemy posture bar: centered top, visible only for Shira/Anotsu
- [ ] Lock-on reticle: diamond widget tracks locked target in 3D
- [ ] Downed overlay: vignette + countdown (replaces full HUD)
- [ ] Hit impact VFX: blood splatter Niagara on flesh hit, spark on armor/weapon clash
- [ ] Parry clash: large spark burst + 0.05s hit-stop (time dilation spike)
- [ ] Sound: placeholder impact SFX (UE starter content), parry ring, downed heartbeat
- [ ] Ambient audio: wind/crickets loop for outdoor, reverb for dojo interior
- [ ] Post-process volume: desaturate -0.4, contrast +0.3, vignette 0.3

**MCP tasks:** WBP_HUD widget creation, post-process parameter setup, Niagara impact spawning in hit notify

---

### Sprint 8 — Playthrough & Tuning (Days 24–26)

**Goal:** One full playthrough without breaking. Combat feels fair and punchy.

Tasks:
- [ ] Full playthrough: road → ambush → courtyard → bridge → dojo → cutscene
- [ ] Difficulty tuning: parry window, posture values, enemy aggression timers
- [ ] Encounter pacing: spawn counts, patrol density, arena lock timing
- [ ] Bug pass: stuck patrol paths, missed animation transitions, camera pops
- [ ] Frame budget check: target 60fps on dev machine
- [ ] Package and run as standalone (not editor play)

---

## GitHub Remote

No remote configured yet. Set one up before Sprint 2:

```bash
gh repo create narfman0/boti --private --source=. --push
```

Or add an existing remote manually.

---

## MCP Session Pattern

Each sprint session:
1. `xdotool` to activate UE window → `spectacle -a` screenshot to verify editor state
2. MCP `initialize` → get session ID
3. `list_toolsets` → `describe_toolset` → `call_tool` for Blueprint work
4. Screenshot after each major change to verify visually
5. Commit working state at end of each sprint

Keep MCP session-scoped: don't reuse session IDs across days.
