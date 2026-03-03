#include "DistrhoPlugin.hpp"
#include "daisysp.h"
#include "../DaisySP-LGPL/Source/Effects/reverbsc.h"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

class GrowPlugin : public Plugin
{
public:
    // An enum to give our parameters easy-to-read names
    enum Parameters {
        kParamStructure,
        kParamBrightness,
        kParamDamping,
        kParamAttack,
        kParamDecay,
        kParamSustain,
        kParamRelease,
        kParamLfoRate,
        kParamLfoDepth,
        kParamFilterCutoff,
        kParamFilterResonance,
        kParamFilterLfoDepth,
        kParamReverbMix,
        kParamReverbTime,
        kParamReverbDamp,
        kParamCount // This must always be the last item
    };

    GrowPlugin()
        : Plugin(kParamCount, 0, 0), // 3 parameters, 0 programs, 0 states
          fStructure(0.2677f),
          fBrightness(0.2152f),
          fDamping(0.8499f),
          fAttack(0.1242f),
          fDecay(0.8579f),
          fSustain(0.3275f),
          fRelease(2.1669f),
          fLfoRate(4.2711f),
          fLfoDepth(0.0f),
          fFilterCutoff(1604.3546f),
          fFilterResonance(0.2286f),
          fFilterLfoDepth(0.7853f),
          fReverbMix(0.4f),
          fReverbTime(0.85f),
          fReverbDamp(12000.0f),
          currentVoice(0)
    {
        // Initialize with a default sample rate. This will be re-initialized
        // with the correct rate in sampleRateChanged, but ensures the object
        // is always in a valid state.
        for (int i = 0; i < kNumVoices; ++i)
        {
            voices[i].Init(48000.0);
            voices[i].SetFreq(110.0f);
            voices[i].SetStructure(fStructure);
            voices[i].SetBrightness(fBrightness);
            voices[i].SetDamping(fDamping);

            envelopes[i].Init(48000.0);
            envelopes[i].SetTime(daisysp::ADSR_SEG_ATTACK, fAttack);
            envelopes[i].SetTime(daisysp::ADSR_SEG_DECAY, fDecay);
            envelopes[i].SetSustainLevel(fSustain);
            envelopes[i].SetTime(daisysp::ADSR_SEG_RELEASE, fRelease);
            voiceGates[i] = false;
            voiceNotes[i] = 0;
        }

        globalLfo.Init(48000.0);
        globalLfo.SetWaveform(daisysp::Oscillator::WAVE_SIN);
        globalLfo.SetFreq(fLfoRate);
        globalLfo.SetAmp(1.0f);

        masterFilter.Init(48000.0);
        masterFilter.SetFreq(fFilterCutoff);
        masterFilter.SetRes(fFilterResonance);

        masterReverb.Init(48000.0);
        masterReverb.SetFeedback(fReverbTime);
        masterReverb.SetLpFreq(fReverbDamp);
    }

protected:
    // -------------------------------------------------------------------------------------------------------
    // Information

    const char* getLabel() const override
    {
        return "grow";
    }

    const char* getMaker() const override
    {
        return "Bert Jerred";
    }

    const char* getLicense() const override
    {
        return "ISC";
    }

    uint32_t getVersion() const override
    {
        return d_version(0, 1, 0);
    }

    int64_t getUniqueId() const override
    {
        return d_cconst('b', 'G', 'r', 'o');
    }

    // -------------------------------------------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override
    {
        // Set up our parameters (the "knobs")
        switch (index)
        {
        case kParamStructure:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Structure";
            parameter.symbol = "struct";
            parameter.ranges.def = 0.2677f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamBrightness:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Brightness";
            parameter.symbol = "bright";
            parameter.ranges.def = 0.2152f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamDamping:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Damping";
            parameter.symbol = "damp";
            parameter.ranges.def = 0.8499f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamAttack:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Attack";
            parameter.symbol = "attack";
            parameter.ranges.def = 0.1242f;
            parameter.ranges.min = 0.001f;
            parameter.ranges.max = 2.0f;
            break;
        case kParamDecay:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Decay";
            parameter.symbol = "decay";
            parameter.ranges.def = 0.8579f;
            parameter.ranges.min = 0.001f;
            parameter.ranges.max = 2.0f;
            break;
        case kParamSustain:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Sustain";
            parameter.symbol = "sustain";
            parameter.ranges.def = 0.3275f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamRelease:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Release";
            parameter.symbol = "release";
            parameter.ranges.def = 2.1669f;
            parameter.ranges.min = 0.001f;
            parameter.ranges.max = 5.0f;
            break;
        case kParamLfoRate:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "LFO Rate";
            parameter.symbol = "lfo_rate";
            parameter.ranges.def = 4.2711f;
            parameter.ranges.min = 0.1f;
            parameter.ranges.max = 20.0f;
            break;
        case kParamLfoDepth:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "LFO Depth";
            parameter.symbol = "lfo_depth";
            parameter.ranges.def = 0.0f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamFilterCutoff:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Cutoff";
            parameter.symbol = "cutoff";
            parameter.ranges.def = 1604.3546f;
            parameter.ranges.min = 20.0f;
            parameter.ranges.max = 20000.0f;
            break;
        case kParamFilterResonance:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Resonance";
            parameter.symbol = "resonance";
            parameter.ranges.def = 0.2286f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamFilterLfoDepth:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Filter LFO";
            parameter.symbol = "filter_lfo";
            parameter.ranges.def = 0.7853f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamReverbMix:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Reverb Mix";
            parameter.symbol = "rev_mix";
            parameter.ranges.def = 0.4f;
            parameter.ranges.min = 0.0f;
            parameter.ranges.max = 1.0f;
            break;
        case kParamReverbTime:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Reverb Time";
            parameter.symbol = "rev_time";
            parameter.ranges.def = 0.85f;
            parameter.ranges.min = 0.1f;
            parameter.ranges.max = 0.99f;
            break;
        case kParamReverbDamp:
            parameter.hints = kParameterIsAutomable;
            parameter.name = "Reverb Damp";
            parameter.symbol = "rev_damp";
            parameter.ranges.def = 12000.0f;
            parameter.ranges.min = 1000.0f;
            parameter.ranges.max = 20000.0f;
            break;
        }
    }

    // -------------------------------------------------------------------------------------------------------
    // Internal Callbacks

    // In the new DPF API, this is for non-realtime notifications (e.g., UI updates).
    // We handle parameter values in the `run` method, so this can be left empty.
    void setParameterValue(uint32_t index, float value) override
    {
        // This is called when a knob is turned by the user or DAW automation
        switch (index)
        {
        case kParamStructure:
            fStructure = value;
            for (int i = 0; i < kNumVoices; ++i) voices[i].SetStructure(fStructure);
            break;
        case kParamBrightness:
            fBrightness = value;
            for (int i = 0; i < kNumVoices; ++i) voices[i].SetBrightness(fBrightness);
            break;
        case kParamDamping:
            fDamping = value;
            for (int i = 0; i < kNumVoices; ++i) voices[i].SetDamping(fDamping);
            break;
        case kParamAttack:
            fAttack = value;
            for (int i = 0; i < kNumVoices; ++i) envelopes[i].SetTime(daisysp::ADSR_SEG_ATTACK, fAttack);
            break;
        case kParamDecay:
            fDecay = value;
            for (int i = 0; i < kNumVoices; ++i) envelopes[i].SetTime(daisysp::ADSR_SEG_DECAY, fDecay);
            break;
        case kParamSustain:
            fSustain = value;
            for (int i = 0; i < kNumVoices; ++i) envelopes[i].SetSustainLevel(fSustain);
            break;
        case kParamRelease:
            fRelease = value;
            for (int i = 0; i < kNumVoices; ++i) envelopes[i].SetTime(daisysp::ADSR_SEG_RELEASE, fRelease);
            break;
        case kParamLfoRate:
            fLfoRate = value;
            globalLfo.SetFreq(fLfoRate);
            break;
        case kParamLfoDepth:
            fLfoDepth = value;
            break;
        case kParamFilterCutoff:
            fFilterCutoff = value;
            break;
        case kParamFilterResonance:
            fFilterResonance = value;
            masterFilter.SetRes(fFilterResonance);
            break;
        case kParamFilterLfoDepth:
            fFilterLfoDepth = value;
            break;
        case kParamReverbMix:
            fReverbMix = value;
            break;
        case kParamReverbTime:
            fReverbTime = value;
            masterReverb.SetFeedback(fReverbTime);
            break;
        case kParamReverbDamp:
            fReverbDamp = value;
            masterReverb.SetLpFreq(fReverbDamp);
            break;
        }
    }
    float getParameterValue(uint32_t index) const override
    {
        switch (index)
        {
            case kParamStructure:  return fStructure;
            case kParamBrightness: return fBrightness;
            case kParamDamping:    return fDamping;
            case kParamAttack:     return fAttack;
            case kParamDecay:      return fDecay;
            case kParamSustain:    return fSustain;
            case kParamRelease:    return fRelease;
            case kParamLfoRate:    return fLfoRate;
            case kParamLfoDepth:   return fLfoDepth;
            case kParamFilterCutoff:    return fFilterCutoff;
            case kParamFilterResonance: return fFilterResonance;
            case kParamFilterLfoDepth:  return fFilterLfoDepth;
            case kParamReverbMix:       return fReverbMix;
            case kParamReverbTime:      return fReverbTime;
            case kParamReverbDamp:      return fReverbDamp;
        }
        return 0.0f;
    }
    void sampleRateChanged(double newSampleRate) override
    {
        // Update the DaisySP engine with the host's sample rate
        for (int i = 0; i < kNumVoices; ++i)
        {
            voices[i].Init(newSampleRate);
            // Re-apply parameters, as Init() may reset them
            voices[i].SetFreq(110.0f);
            voices[i].SetStructure(fStructure);
            voices[i].SetBrightness(fBrightness);
            voices[i].SetDamping(fDamping);

            envelopes[i].Init(newSampleRate);
            envelopes[i].SetTime(daisysp::ADSR_SEG_ATTACK, fAttack);
            envelopes[i].SetTime(daisysp::ADSR_SEG_DECAY, fDecay);
            envelopes[i].SetSustainLevel(fSustain);
            envelopes[i].SetTime(daisysp::ADSR_SEG_RELEASE, fRelease);
        }

        globalLfo.Init(newSampleRate);
        globalLfo.SetWaveform(daisysp::Oscillator::WAVE_SIN);
        globalLfo.SetFreq(fLfoRate);
        globalLfo.SetAmp(1.0f);

        masterFilter.Init(newSampleRate);
        masterFilter.SetFreq(fFilterCutoff);
        masterFilter.SetRes(fFilterResonance);

        masterReverb.Init(newSampleRate);
        masterReverb.SetFeedback(fReverbTime);
        masterReverb.SetLpFreq(fReverbDamp);
    }

    void activate() override
    {
        // Reset voices on activation to prevent "instant note" from garbage state
        double rate = getSampleRate();
        if (rate <= 0) rate = 48000.0;

        for (int i = 0; i < kNumVoices; ++i)
        {
            voices[i].Init(rate);
            voices[i].SetFreq(110.0f);
            voices[i].SetStructure(fStructure);
            voices[i].SetBrightness(fBrightness);
            voices[i].SetDamping(fDamping);

            envelopes[i].Init(rate);
            envelopes[i].SetTime(daisysp::ADSR_SEG_ATTACK, fAttack);
            envelopes[i].SetTime(daisysp::ADSR_SEG_DECAY, fDecay);
            envelopes[i].SetSustainLevel(fSustain);
            envelopes[i].SetTime(daisysp::ADSR_SEG_RELEASE, fRelease);
            voiceGates[i] = false;
            voiceNotes[i] = 0;
        }
        currentVoice = 0;

        globalLfo.Init(rate);
        globalLfo.SetWaveform(daisysp::Oscillator::WAVE_SIN);
        globalLfo.SetFreq(fLfoRate);
        globalLfo.SetAmp(1.0f);

        masterFilter.Init(rate);
        masterFilter.SetFreq(fFilterCutoff);
        masterFilter.SetRes(fFilterResonance);

        masterReverb.Init(rate);
        masterReverb.SetFeedback(fReverbTime);
        masterReverb.SetLpFreq(fReverbDamp);

        // Silent flush: Run the DSP for a short period to clear garbage from delay lines.
        // This prevents the "ding" sound caused by uninitialized memory in the comb filters.
        const int flushSamples = static_cast<int>(rate * 0.25f);
        for (int s = 0; s < flushSamples; ++s)
        {
            for (int i = 0; i < kNumVoices; ++i)
            {
                voices[i].Process();
            }
        }
    }

    // -------------------------------------------------------------------------------------------------------
    // Process

    // This `run` signature is for the new DPF `dev` branch API
    void run(const float**, float** outputs, uint32_t frames,
             const MidiEvent* midiEvents, uint32_t midiEventCount) override
    {
        // Get the stereo output buffers
        float* const outL = outputs[0];
        float* const outR = outputs[1];

        uint32_t currentFrame = 0;
        uint32_t eventIndex = 0;

        while (currentFrame < frames)
        {
            // Determine the frame of the next event, or the end of the buffer
            uint32_t nextEventFrame = frames;
            if (eventIndex < midiEventCount)
            {
                nextEventFrame = midiEvents[eventIndex].frame;
                if (nextEventFrame > frames) nextEventFrame = frames;
                if (nextEventFrame < currentFrame) nextEventFrame = currentFrame;
            }

            // Render audio until the next event
            for (; currentFrame < nextEventFrame; ++currentFrame)
            {
                // 1. Process the Global LFO once per frame
                // A sine wave outputs from -1.0 to 1.0.
                float lfoRaw = globalLfo.Process();

                // 2. Convert to unipolar (0.0 to 1.0) so it scales volume properly
                float lfoUnipolar = (lfoRaw * 0.5f) + 0.5f;

                // 3. Calculate Tremolo scalar based on Depth knob
                // If depth is 0, scalar is 1.0 (no change).
                float tremolo = 1.0f - (fLfoDepth * (1.0f - lfoUnipolar));

                // 3. Filter Sweep Math
                // We add the LFO to the base cutoff. A max LFO depth adds up to 10kHz of sweep.
                float currentCutoff = fFilterCutoff + (lfoUnipolar * fFilterLfoDepth * 10000.0f);
                
                if (currentCutoff > 20000.0f) currentCutoff = 20000.0f;
                if (currentCutoff < 20.0f) currentCutoff = 20.0f;
                masterFilter.SetFreq(currentCutoff);

                float out = 0.0f;
                for (int i = 0; i < kNumVoices; ++i) {
                    // Process the envelope with the current gate state
                    float envOut = envelopes[i].Process(voiceGates[i]);
                    
                    // Multiply the string voice by the envelope
                    out += voices[i].Process() * envOut;
                }

                // 5. Run the summed chord through the Moog Filter
                out = masterFilter.Process(out);

                // 4. Apply global Tremolo and polyphony gain compensation to our DRY signal
                float drySignal = out * tremolo * 0.2f;

                // Process the Reverb
                float wetL, wetR;
                // Feed the dry signal into both L and R inputs of the reverb
                masterReverb.Process(drySignal, drySignal, &wetL, &wetR);

                // Calculate the final Dry/Wet Mix using a linear crossfade
                outL[currentFrame] = (drySignal * (1.0f - fReverbMix)) + (wetL * fReverbMix);
                outR[currentFrame] = (drySignal * (1.0f - fReverbMix)) + (wetR * fReverbMix);
            }

            // Process all events at this frame
            while (eventIndex < midiEventCount && midiEvents[eventIndex].frame == currentFrame)
            {
                const MidiEvent& midiEvent = midiEvents[eventIndex];

                if (midiEvent.size >= 3)
                {
                    const uint8_t status = midiEvent.data[0] & 0xF0;
                    const uint8_t midiNote = midiEvent.data[1];
                    const uint8_t velocity = midiEvent.data[2];

                    // NOTE ON
                    if (status == 0x90 && velocity > 0) 
                    {
                        voices[currentVoice].SetFreq(daisysp::mtof(midiNote));
                        
                        const float vel = velocity / 127.0f;
                        voices[currentVoice].SetAccent(vel * vel);
                        voices[currentVoice].Trig();

                        // Open the gate and remember which note this voice is playing
                        voiceGates[currentVoice] = true;
                        voiceNotes[currentVoice] = midiNote;

                        currentVoice = (currentVoice + 1) % kNumVoices;
                    }
                    // NOTE OFF (0x80, or 0x90 with 0 velocity)
                    else if (status == 0x80 || (status == 0x90 && velocity == 0))
                    {
                        // Find the voice playing this note and close its gate
                        for (int i = 0; i < kNumVoices; ++i) {
                            if (voiceNotes[i] == midiNote && voiceGates[i]) {
                                voiceGates[i] = false; 
                            }
                        }
                    }
                }
                eventIndex++;
            }
        }
    }

private:
    static const int kNumVoices = 8;

    // The DaisySP sound engine
    daisysp::StringVoice voices[kNumVoices];
    daisysp::Adsr envelopes[kNumVoices];
    daisysp::Oscillator globalLfo;
    daisysp::LadderFilter masterFilter;
    daisysp::ReverbSc masterReverb;
    int currentVoice;

    // Voice tracking for Note Off handling
    bool voiceGates[kNumVoices];
    uint8_t voiceNotes[kNumVoices];

    // Local storage for our parameter values
    float fStructure;
    float fBrightness;
    float fDamping;
    float fAttack;
    float fDecay;
    float fSustain;
    float fRelease;
    float fLfoRate;
    float fLfoDepth;
    float fFilterCutoff;
    float fFilterResonance;
    float fFilterLfoDepth;
    float fReverbMix;
    float fReverbTime;
    float fReverbDamp;

};

// -----------------------------------------------------------------------------------------------------------

Plugin* createPlugin()
{
    return new GrowPlugin();
}

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
