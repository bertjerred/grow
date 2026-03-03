#ifndef DISTRHO_PLUGIN_INFO_H_INCLUDED
#define DISTRHO_PLUGIN_INFO_H_INCLUDED

// -----------------------------------------------------------------------------------------------------------
// Plugin Details
// -----------------------------------------------------------------------------------------------------------

#define DISTRHO_PLUGIN_NAME "grow"
#define DISTRHO_PLUGIN_MAKER "Bert Jerred"
#define DISTRHO_PLUGIN_URI "https://bjmp.carrd.co/plugins/grow"

#define DISTRHO_PLUGIN_IS_SYNTH         1
#define DISTRHO_PLUGIN_WANT_MIDI_INPUT  1
#define DISTRHO_PLUGIN_HAS_UI           1
#define DISTRHO_PLUGIN_IS_RT_SAFE       1

// Define 0 audio inputs and 2 audio outputs (stereo instrument)
#define DISTRHO_PLUGIN_NUM_INPUTS       0
#define DISTRHO_PLUGIN_NUM_OUTPUTS      2

// Initial GUI size
#define DISTRHO_UI_DEFAULT_WIDTH        500
#define DISTRHO_UI_DEFAULT_HEIGHT       350

// It's good practice to define a version number
#define DISTRHO_PLUGIN_VERSION d_version(0, 1, 0)

#define DISTRHO_PLUGIN_UNIQUE_ID Grow
#define DISTRHO_PLUGIN_BRAND_ID Bert
#define DISTRHO_PLUGIN_MAC_CREATOR DISTRHO_PLUGIN_BRAND_ID
#define DISTRHO_PLUGIN_MAC_CREATOR_STRING "Bert Jerred"
#define DISTRHO_PLUGIN_CLAP_ID "https://bjmp.carrd.co/plugins/grow"

// -----------------------------------------------------------------------------------------------------------

#endif // DISTRHO_PLUGIN_INFO_H_INCLUDED
