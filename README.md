This is my first ever audio plugin, “grow.” I hope it will be the first of many.

While I have made audio-related apps before using Python, I decided to try to up my game a little. In my lead-up to creating the project skeleton in VS Code, I read about DPF and DaisySP, an inspiring signal processing library that I hoped would work with what I was doing. As it turned out, Daisy’s free libraries constitute the core of “grow.”

![grow plugin screenshot](https://github.com/bertjerred/grow/blob/main/grow-screenshot.png)

The primary voice of this audio plugin is the Karplus-Strong implementation in the DaisySP physical modeling category. It is controlled by the three leftmost vertical sliders. This, proceeding to the right, is followed by an obligatory ADSR envelope, an LFO, a Moog-style ladder filter, and a spacious reverb effect. Once the engine was compiling properly and running without major glitches, I moved onto the UI design. The palette is inspired by my dining room, and perfected with Owen Earl’s original font, Jost*.

**Hear audio demos on [my Substack](https://open.substack.com/pub/bertjerred/p/grow-an-open-source-karplus-strong?r=2q7lra&utm_campaign=post&utm_medium=web).**

Currently, release v0.1.0 comes in a few flavors: VST, VST3 and AU. I invite you to [download them for free and test them out](https://github.com/bertjerred/grow/releases/tag/v0.1.0). Please keep in mind, I am new at this. I have much to learn about digital signal processing, thread management, and memory management. Still, I have been testing grow pretty much non-stop since I finished working on it, which I think means it is fun to use and a musically expressive piece of software.

If you decide to try it, I’d love to exchange presets with you. I look forward to seeing what others can do with this new instrument.

## License
This project is licensed under the [ISC License](https://www.isc.org/licenses/). 

- **DaisySP (LGPL)**: This plugin utilizes the DaisySP library for its reverb and DSP functions. 
- **DPF**: Built using the DISTRHO Plugin Framework (ISC). 
- **Font**: Utilizes the Jost* font (SIL Open Font License 1.1).
