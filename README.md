# grow
A Karplus-Strong string synthesizer with a minimalist aesthetic. 

grow is a digital synthesizer designed by Bert Jerred. It utilizes physical modeling to create plucked-string sounds, processed through a global LFO, a resonant ladder filter, and a spacious SC-style reverb.

![grow plugin screenshot](https://github.com/bertjerred/grow/blob/main/grow-screenshot.png)

## Features
- **Polyphonic String Engine**: 8-voice polyphony using the DaisySP StringVoice implementation.

- **Ladder Filter**: A resonant filter labeled `LADDER` for tactile, hardware-style control.

- **Modulation**: Global tremolo and filter sweep controlled via a dedicated LFO section.

- **Boutique UI**: A custom-crafted, high-contrast interface using the Jost* font, designed for high-resolution displays.

## Parameters
### Voice

- **STRUCT**: Adjusts the physical modeling structure of the string.

- **BRIGHT**: Controls the harmonic content and brightness of the pluck.

- **DAMP**: Sets the string damping for shorter or longer decay.

### Envelope

- Standard ADSR (Attack, Decay, Sustain, Release) controls for shaping the volume of the string voices.

### Modulation & Filter

- **LFO RATE/DEPTH**: Controls the speed and intensity of the global tremolo.

- **CUTOFF/RES**: Standard frequency cutoff and resonance for the Ladder filter.

- **LADDER**: Adjusts the amount of LFO modulation applied specifically to the filter frequency.

### Reverb

- **MIX**: Dry/Wet balance of the reverb signal.

- **TIME**: The decay time (feedback) of the reverb tail.

- **RV DAMP**: High-frequency damping for the reverb space.

## Installation
1. Download the latest release (v0.1.0) for your operating system.

2. Place the `.vst3` or `.au` file in your standard plugin directory.

3. Rescan your DAW and look for `grow` by Bert Jerred.

## License
This project is licensed under the ISC License. 

- **DaisySP (LGPL)**: This plugin utilizes the DaisySP library for its reverb and DSP functions. 

- **DPF**: Built using the DISTRHO Plugin Framework (ISC). 

- **Font**: Utilizes the Jost* font (SIL Open Font License 1.1).
