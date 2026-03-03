#ifndef PLUGIN_GROW_H
#define PLUGIN_GROW_H

#include "DistrhoPlugin.hpp"
#include "daisysp.h"
#include "../DaisySP-LGPL/Source/Effects/reverbsc.h"

START_NAMESPACE_DISTRHO

/**
 * @brief The main plugin class for the Grow synthesizer.
 */
class GrowPlugin : public Plugin {
public:
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
        kParamCount
    };

    // Constructor and Destructor
    GrowPlugin();
    ~GrowPlugin() override = default;

protected:
    // DPF API methods for plugin metadata
    const char* getLabel() const override;
    const char* getMaker() const override;
    const char* getLicense() const override;
    uint32_t getVersion() const override;
    int64_t getUniqueId() const override;

    // DPF API methods for parameter management
    void initParameter(uint32_t index, Parameter& parameter) override;
    void setParameterValue(uint32_t index, float value) override;
    float getParameterValue(uint32_t index) const override;

    // DPF API methods for state and audio processing
    void sampleRateChanged(double newSampleRate) override;
    void activate() override;
    void run(const float** inputs, float** outputs, uint32_t frames, const MidiEvent* midiEvents, uint32_t midiEventCount) override;

private:
    static const int kNumVoices = 8;

    // The DaisySP sound engine objects
    daisysp::StringVoice voices[kNumVoices];
    daisysp::Adsr envelopes[kNumVoices];
    daisysp::Oscillator globalLfo;
    daisysp::LadderFilter masterFilter;
    daisysp::ReverbSc masterReverb;
    
    // Voice and polyphony tracking
    int currentVoice;
    bool voiceGates[kNumVoices];
    uint8_t voiceNotes[kNumVoices];

    // Local parameter storage
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

END_NAMESPACE_DISTRHO

#endif // PLUGIN_GROW_H