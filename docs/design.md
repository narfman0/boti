# Design Overview

## Core Premise

Manji is immortal. That's the mechanic, the narrative, and the tone all in one. Sacred bloodworms regenerate any wound — including severed limbs — but the process takes time. The player can never truly lose, but they can be crippled, humiliated, and worn down in ways that make fights feel genuinely dangerous.

Enemies are mortal. The player is not. The tension comes from that asymmetry.

## Immortality System

- **HP pool:** Manji has a health bar. At zero he enters a downed state (5s timer, visible wound animation).
- **Revive:** Bloodworms crawl across wounds; he revives at ~30% HP.
- **Limb loss:** Arms and legs can be severed. A severed arm drops the weapon held in that hand. The limb detaches, crawls back (or is retrieved), and reattaches over ~20 seconds.
- **Tactical pressure:** Enemy AI is aware of Manji's limb state and presses the advantage. A one-armed Manji is a much easier target.

## Combat Feel

Closest reference points: Sekiro (posture/parry focus, deliberate timing) with the weapon variety of Blade of the Immortal. Not a roll-spam game. Reads, parries, and weapon-range awareness matter.

- **Parry / deflect** — the primary defensive tool; successful parry opens a riposte window
- **Dodge cancel** — available but punished if overused (stamina cost)
- **Posture break** — sustained aggression or successful parries deplete enemy posture; posture break → visceral kill

## Weapon System

Manji's arsenal is unconventional: scythe-swords, blades mounted at odd angles, hidden tools. Each weapon slot is tied to a limb. Loadout shifts dynamically when limbs are lost or recovered.

Each named Ittō-ryū enemy has a unique weapon style defined as a data asset — their moveset, range, and exploitable weakness are all derived from it.

## Story Structure

Five acts. Each act ends with a named Ittō-ryū kill. Anotsu Kagehisa escapes until Act 5.

Rin is a non-combat companion: she follows, reacts, occasionally creates openings (distractions, environmental assists). Her presence is narrative weight, not a gameplay burden.

## Setting

Late Edo period Japan, ~1782. Environments: village outskirts, forest roads, river crossings, castle districts, dojo interiors. Lighting is dramatic — Lumen handles most of this. Color palette: desaturated with blood as the only strong accent.
