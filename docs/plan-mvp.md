# Boti — Super MVP Plan

**Scope:** Single linear level, 5 encounters ending on a boss, fully playable loop.
**Goal:** Prove the core feel — parry timing, posture breaks, immortality tension — before expanding scope.

---

## Level Structure

One continuous environment: a winding path through a village outskirt, across a bridge, into a shrine courtyard, ending inside a dojo interior. All encounters are separated by short traversal corridors (no backtracking).

| # | Name | Location | Enemies | Purpose |
|---|------|----------|---------|---------|
| 1 | The Road | Dirt road / gate | 2 Ittō-ryū foot soldiers | Tutorial — teaches attack, dodge, basic timing |
| 2 | Ambush | Wooded bend | 3 soldiers (1 with spear reach) | Introduces range variance, forces positioning |
| 3 | The Courtyard | Shrine courtyard | 4 soldiers + 1 archer | Group management, environmental pressure |
| 4 | Bridge Duel | Narrow bridge | **Shira** (named mid-boss) | First true duel — posture break mechanic spotlight |
| 5 | The Dojo | Interior dojo | **Anotsu Kagehisa** (boss) | Final fight, 2-phase, story payoff |

Rin Asano is present as a silent witness at the dojo entrance — no gameplay role in MVP, just narrative anchor.

---

## Characters

### Manji (Player)
- Placeholder: UE5 Mannequin retargeted, Manji-silhouette proportions
- Wears tattered dark kimono
- Weapons: paired short blades (primary), hook-chain (secondary, unlocked after Bridge)
- Immortality: full system below

### Foot Soldiers (Ittō-ryū grunts)
- Shared mesh, two weapon loadouts: katana / naginata
- Simple AI: aggress, attack pattern (2 hit → back off), flinch on revive

### Shira (Mid-boss, Encounter 4)
- Sadistic brawler — wide, heavy attacks, no armor
- Gimmick: he presses limb-downed Manji relentlessly (AI specifically targets downed state)
- Posture breaks faster than Anotsu but has high HP
- Unique finisher: if you riposte his posture break, he loses his sword arm (ragdoll cosmetic)

### Anotsu Kagehisa (Final Boss)
- Phase 1: methodical, long-range halberd (Ko-nigiritō style), punishes aggression
- Phase 2 (triggered at 40% HP): abandons caution, flurry pressure, harder to parry
- Not killable in MVP ending — escape cutscene (matches act 1 canon)

---

## Core Systems (MVP Scope)

### 1. Locomotion & Camera
- Walk / sprint / dodge roll (i-frames: 12 frames)
- Lock-on: snaps camera to nearest enemy, toggle
- Over-shoulder offset, camera pull-back during combat

### 2. Combat
- **Light attack:** 3-hit combo chain, each hit cancellable into dodge
- **Heavy attack:** slower, high posture damage, breaks guard
- **Parry:** narrow window (~8 frames), successful parry staggers enemy, opens riposte
- **Riposte:** single high-damage follow-up after parry stagger
- **Hit detection:** melee sweep traces per animation notify
- **Enemy health:** standard depleting bar; death triggers ragdoll

### 3. Posture System
- Both player and enemies have a posture bar (hidden for player, visible for enemies)
- Posture depletes on: blocked attacks, parry attempts, taking hits while blocking
- Posture regens slowly when not being hit
- Posture break: enemy staggers, wide riposte window (~2s), guaranteed visceral kill on bosses

### 4. Immortality (MVP version)
- HP hits zero → **Downed state**: Manji collapses, 5-second timer, bloodworm VFX (placeholder particles)
- Revives at 30% HP
- Enemies react: grunts back off briefly (fear), Shira presses in (aggression flag)
- **Limb loss: deferred to post-MVP** — too risky to scope in v1; immortality revive alone carries the fantasy

### 5. Enemy AI
- **Grunt BT:** Patrol → Detect → Close range → Attack pattern loop → Back off after 2 hits
- **Elite/named BT:** Adds parry counter, posture-aware pressure, downed-state aggression
- **Boss BT:** Phase-gated, special attack telegraphs (red flash), phase 2 flurry mode

### 6. HUD
- Manji HP bar (top left)
- Manji posture bar (below HP, hidden when full)
- Enemy posture bar (centered top, appears only in boss/named fights)
- Lock-on reticle
- Downed state countdown (center screen)

---

## Assets

### Environment
- **Synty: Polygon Samurai** — primary world kit (terrain, buildings, props, foliage)
- **Synty: Polygon Nature** — trees, rocks, water for road/bridge sections
- Source via asset server (`srv`, 192.168.1.11) — get subscription to fill gaps

### Characters
- **UE5 Mannequin** as immediate placeholder for all characters
- **Synty: Polygon Warriors** or custom — Manji silhouette reference model
- Swap with proper meshes in post-MVP pass

### VFX
- UE5 Niagara templates: blood impact, sparks on parry clash, particle placeholder for bloodworms
- Custom parry clash spark (priority — it's the primary feel signal)

### Audio
- UE5 starter content + free impact SFX as placeholders
- Koto/shamisen ambient loop (royalty-free) for dojo scene
- No VO in MVP

---

## Task Breakdown

### Phase A — Foundation (Days 1–3)
- [ ] Import Synty Samurai pack, set up materials
- [ ] 3rd person character blueprint (locomotion, camera rig, lock-on)
- [ ] Dodge roll with i-frames
- [ ] Basic attack combo (trace-based hit detection)

### Phase B — Combat Core (Days 4–7)
- [ ] Parry window + stagger state on enemy
- [ ] Riposte follow-up
- [ ] Posture bar component (shared, both player and enemy)
- [ ] Posture break → stagger → visceral kill sequence
- [ ] Heavy attack implementation

### Phase C — Immortality (Days 8–9)
- [ ] HP → zero → downed state timer
- [ ] Bloodworm VFX placeholder (Niagara burst)
- [ ] Revive at 30% HP
- [ ] Grunt AI downed-state reaction flag

### Phase D — Enemy AI (Days 10–14)
- [ ] Grunt behavior tree (patrol, detect, attack loop)
- [ ] Elite behavior tree (parry counter, downed pressure)
- [ ] Anotsu phase-gated behavior tree (phase 1 / phase 2 at 40%)
- [ ] Shira mid-boss BT (aggressive, limb-press behavior)

### Phase E — Level (Days 15–19)
- [ ] Block out 5 arenas + connecting corridors (BSP/Geometry brush first)
- [ ] Dress with Synty assets (road, courtyard, bridge, dojo)
- [ ] Encounter triggers (patrol start, arena lock gates)
- [ ] Rin static mesh placeholder at dojo entrance

### Phase F — Boss & Closing (Days 20–23)
- [ ] Anotsu phase 2 trigger + behavior switch
- [ ] Anotsu escape cutscene (Sequencer, 20–30s, rough animatic quality)
- [ ] Win condition: cutscene plays → black screen → "To be continued" title card
- [ ] Full playthrough pass: pacing, encounter difficulty tuning

### Phase G — HUD & Polish (Days 24–26)
- [ ] HUD widget: HP, posture, enemy posture, lock-on reticle, downed timer
- [ ] Parry clash VFX (spark burst on successful parry)
- [ ] Blood impact decals on hits
- [ ] Death screen + continue prompt

---

## Out of Scope for MVP

- Limb loss / reattachment system
- Rin Asano gameplay interactions
- Dialogue / subtitle system
- More than 2 enemy weapon loadouts
- Weapon variety for Manji (hook-chain blocked, primary only)
- Narrative cutscenes beyond the Anotsu escape
- Main menu beyond a basic "Press Start"
- Save system

---

## MCP Usage Plan

The UE MCP plugin accelerates the most tedious parts:

- **Blueprint wiring:** combat state machine transitions, AI behavior tree nodes
- **Level dressing:** bulk Synty asset placement via natural language prompts
- **Material setup:** Synty material instances, terrain blending, decal placement
- **Encounter scripting:** trigger volumes, patrol path assignment, arena lock logic
- **Iteration:** rapid parameter tuning (i-frame timing, parry windows, posture values)

All MCP work gets reviewed before commit — MCP drives speed, human drives quality.
