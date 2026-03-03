#include "DistrhoPluginInfo.h"
#include "DistrhoUI.hpp"
#include "grow.hpp"
#include "Jost-Light.hpp"
#include <vector>
#include <cmath>
#include <string>
#include <cstdio>
#include <algorithm>

namespace DISTRHO {

#include <cstdlib>

#if defined(_WIN32)
#include <windows.h>
#include <shellapi.h>
#endif

void openURL(const char* url) {
#if defined(_WIN32)
    // Windows implementation
    ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
#elif defined(__APPLE__)
    // macOS implementation
    std::string command = "open " + std::string(url);
    std::system(command.c_str());
#else
    // Linux implementation
    std::string command = "xdg-open " + std::string(url);
    std::system(command.c_str());
#endif
}

struct SliderInfo {
    float x, y, width, height;
    float min, max, value;
    const char* label;
    bool isDragging;
};

class UIGrow : public UI {
public:
    UIGrow() : UI(1200, 800) {
        setupSliders();
    }

protected:
    void parameterChanged(uint32_t index, float value) override {
        if (index < fSliders.size()) {
            fSliders[index].value = value;
            repaint();
        }
    }

    void onNanoDisplay() override {
        const float w = getWidth();
        const float h = getHeight();

        if (!fFontLoaded) {
            createFontFromMemory("JostLight", Jost_Light_ttf, Jost_Light_ttf_len, 0); 
            fFontLoaded = true;
        }

        // Background
        beginPath();
        rect(0, 0, w, h);
        fillColor(Color(185, 226, 167)); 
        fill();

        fontFace("JostLight");

        // --- COLORS ---
        Color labelBlue( 80, 150, 190);
        Color labelPink(200, 120, 160);

        // Top Left: Title (96px, White)
        fillColor(Color(255, 255, 255));
        fontSize(96);
        textAlign(DGL::NanoVG::ALIGN_LEFT | DGL::NanoVG::ALIGN_TOP);
        text(40, 40, "grow", nullptr);

        // Top Right: Subtitle (32px, Blue)
        fillColor(labelBlue);
        fontSize(32);
        textAlign(DGL::NanoVG::ALIGN_RIGHT | DGL::NanoVG::ALIGN_TOP);
        text(w - 40, 90, "KARPLUS-STRONG SYNTH | LFO | LADDER FILTER | REVERB", nullptr);

        // Bottom Left: Footer (28px, Pink)
        fillColor(labelPink);
        fontSize(28);
        
        // GitHub Link
        textAlign(DGL::NanoVG::ALIGN_LEFT | DGL::NanoVG::ALIGN_BOTTOM);
        text(40, h - 70, "grow @ github", nullptr);
        beginPath();
        rect(40, h - 65, 170, 2); // Thicker underline for 2x scale
        fill();

        // Credits
        text(40, h - 30, "DESIGNED BY BERT JERRED USING DISTRHO", nullptr);

        // Bottom Right: Licenses Link (Pink)
        textAlign(DGL::NanoVG::ALIGN_RIGHT | DGL::NanoVG::ALIGN_BOTTOM);
        text(w - 40, h - 30, "LICENSES", nullptr);
        beginPath();
        rect(w - 170, h - 25, 130, 2);
        fill();

        // Draw all sliders
        for (size_t i = 0; i < fSliders.size(); ++i) {
            drawSlider(fSliders[i], i, labelBlue, labelPink);
        }
    }

    void drawSlider(const SliderInfo& slider, int index, const Color& blue, const Color& pink) {
        // Slider Track
        float trackX = slider.x + (slider.width * 0.5f);
        beginPath();
        rect(trackX - 2, slider.y, 4, slider.height);
        fillColor(Color(255, 255, 255, 120));
        fill();

        // Calculate thumb position
        float normalizedValue = (slider.value - slider.min) / (slider.max - slider.min);
        normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
        float thumbY = slider.y + slider.height - (slider.height * normalizedValue);

        // Slider Thumb
        beginPath();
        rect(slider.x, thumbY - 4, slider.width, 8);
        fillColor(Color(255, 255, 255));
        fill();

        // --- TOP-JUSTIFIED LABELS (32px) ---
        fillColor((index % 2 == 0) ? blue : pink);
        fontSize(32);
        
        // Use ALIGN_RIGHT so the end of the text string locks to the origin
        textAlign(DGL::NanoVG::ALIGN_RIGHT | DGL::NanoVG::ALIGN_MIDDLE);
        
        save();
        // Translate exactly to the TOP of the slider track
        translate(trackX + 16, slider.y); 
        rotate(-1.570796f); // -90 degree rotation
        text(0, 0, slider.label, nullptr);
        restore();

        // Value Display (22px)
        fontSize(22);
        fillColor(Color(255, 255, 255, 180));
        textAlign(DGL::NanoVG::ALIGN_CENTER | DGL::NanoVG::ALIGN_TOP);
        char valueStr[32];
        if (slider.max > 100.0f) {
            snprintf(valueStr, sizeof(valueStr), "%.0f", slider.value);
        } else {
            snprintf(valueStr, sizeof(valueStr), "%.2f", slider.value);
        }
        text(trackX, slider.y + slider.height + 20, valueStr, nullptr);
    }

    bool onMouse(const MouseEvent& ev) override {
        if (ev.press) {
            const float w = getWidth();
            const float h = getHeight();

            // Link Hitboxes
            if (ev.pos.getX() >= 40 && ev.pos.getX() <= 210 && 
                ev.pos.getY() >= h - 100 && ev.pos.getY() <= h - 60) {
                
                openURL("https://github.com/bertjerred/grow");
                return true; 
            }

            // Check if clicked Bottom-Right (Licenses zone)
            if (ev.pos.getX() >= w - 180 && ev.pos.getX() <= w - 40 && 
                ev.pos.getY() >= h - 60 && ev.pos.getY() <= h - 20) {
                
                openURL("https://github.com/bertjerred/grow/blob/main/LICENSE.md");
                return true;
            }
        }

        // Slider interaction logic
        for (size_t i = 0; i < fSliders.size(); ++i) {
            SliderInfo& slider = fSliders[i];
            if (ev.pos.getX() >= slider.x - 10 && 
                ev.pos.getX() <= slider.x + slider.width + 10 &&
                ev.pos.getY() >= slider.y && 
                ev.pos.getY() <= slider.y + slider.height + 40) {
                
                if (ev.press) {
                    slider.isDragging = true;
                    updateSliderFromMouse(slider, ev.pos.getY());
                    return true;
                }
            }
        }

        if (!ev.press) {
            for (auto& slider : fSliders) slider.isDragging = false;
        }

        return false;
    }

    bool onMotion(const MotionEvent& ev) override {
        for (auto& slider : fSliders) {
            if (slider.isDragging) {
                updateSliderFromMouse(slider, ev.pos.getY());
                return true;
            }
        }
        return false;
    }

private:
    std::vector<SliderInfo> fSliders;
    bool fFontLoaded = false;

    void setupSliders() {
        fSliders.resize(GrowPlugin::kParamCount);

        const float sliderW = 32;
        const float sliderH = 250;
        const float startX = 100;
        const float spacing = 72;
        const float yPos = 160;

        // Map parameters to the single row
        fSliders[GrowPlugin::kParamStructure]  = {startX + (0 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.2677f, "STRUCT", false};
        fSliders[GrowPlugin::kParamBrightness] = {startX + (1 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.2152f, "BRIGHT", false};
        fSliders[GrowPlugin::kParamDamping]    = {startX + (2 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.8499f, "DAMP", false};
        fSliders[GrowPlugin::kParamAttack]     = {startX + (3 * spacing), yPos, sliderW, sliderH, 0.001f, 2.0f, 0.1242f, "ATTACK", false};
        fSliders[GrowPlugin::kParamDecay]      = {startX + (4 * spacing), yPos, sliderW, sliderH, 0.001f, 2.0f, 0.8579f, "DECAY", false};
        fSliders[GrowPlugin::kParamSustain]    = {startX + (5 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.3275f, "SUSTAIN", false};
        fSliders[GrowPlugin::kParamRelease]    = {startX + (6 * spacing), yPos, sliderW, sliderH, 0.001f, 5.0f, 2.1669f, "RELEASE", false};
        fSliders[GrowPlugin::kParamLfoRate]    = {startX + (7 * spacing), yPos, sliderW, sliderH, 0.1f, 20.0f, 4.2711f, "LFO RATE", false};
        fSliders[GrowPlugin::kParamLfoDepth]   = {startX + (8 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.0f, "LFO DEPTH", false};
        fSliders[GrowPlugin::kParamFilterCutoff]    = {startX + (9 * spacing), yPos, sliderW, sliderH, 20.0f, 20000.0f, 1604.3f, "CUTOFF", false};
        fSliders[GrowPlugin::kParamFilterResonance] = {startX + (10 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.2286f, "RES", false};
        fSliders[GrowPlugin::kParamFilterLfoDepth]  = {startX + (11 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.7853f, "LADDER", false};
        fSliders[GrowPlugin::kParamReverbMix]       = {startX + (12 * spacing), yPos, sliderW, sliderH, 0.0f, 1.0f, 0.4f, "MIX", false};
        fSliders[GrowPlugin::kParamReverbTime]      = {startX + (13 * spacing), yPos, sliderW, sliderH, 0.1f, 0.99f, 0.85f, "TIME", false};
        fSliders[GrowPlugin::kParamReverbDamp]      = {startX + (14 * spacing), yPos, sliderW, sliderH, 1000.0f, 20000.0f, 12000.0f, "RV DAMP", false};
    }

    void updateSliderFromMouse(SliderInfo& slider, int mouseY) {
        float relativeY = static_cast<float>(mouseY - slider.y);
        float normalizedValue = 1.0f - (relativeY / slider.height);
        normalizedValue = std::max(0.0f, std::min(1.0f, normalizedValue));
        slider.value = slider.min + normalizedValue * (slider.max - slider.min);
        for (uint32_t i = 0; i < fSliders.size(); ++i) {
            if (&fSliders[i] == &slider) {
                setParameterValue(i, slider.value);
                break;
            }
        }
        repaint();
    }
};

UI* createUI() {
    return new UIGrow();
}

} // namespace DISTRHO
