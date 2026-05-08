cmake_minimum_required(VERSION 3.22)

project(SimpleEQ VERSION 1.0.0)

# ============================================================
# Download JUCE automatically from GitHub
# This means you don't need to install anything!
# ============================================================
include(FetchContent)

FetchContent_Declare(
    JUCE
    GIT_REPOSITORY https://github.com/juce-framework/JUCE.git
    GIT_TAG        7.0.12
)

FetchContent_MakeAvailable(JUCE)

# ============================================================
# Define your plugin
# ============================================================
juce_add_plugin(SimpleEQ
    VERSION                 1.0.0
    COMPANY_NAME            "MyName"            # Change this to your name
    PLUGIN_MANUFACTURER_CODE MaNm               # 4 letters, unique to you
    PLUGIN_CODE             SEq1               # 4 letters, unique to your plugin
    FORMATS                 VST3               # We are building VST3
    PRODUCT_NAME            "SimpleEQ"

    IS_SYNTH                FALSE              # This is an effect, not a synth
    NEEDS_MIDI_INPUT        FALSE
    NEEDS_MIDI_OUTPUT       FALSE
    IS_MIDI_EFFECT          FALSE
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE              # Don't auto-install on the build machine
)

# ============================================================
# Tell CMake which source files to compile
# ============================================================
juce_generate_juce_header(SimpleEQ)

target_sources(SimpleEQ
    PRIVATE
        source/PluginProcessor.cpp
        source/PluginEditor.cpp
)

# ============================================================
# Settings
# ============================================================
target_compile_definitions(SimpleEQ
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0
)

# ============================================================
# Link JUCE modules your plugin needs
# ============================================================
target_link_libraries(SimpleEQ
    PRIVATE
        juce::juce_audio_utils
        juce::juce_dsp
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)