cmake_minimum_required(VERSION 3.15)

project(MidiViewer VERSION 0.0.1)

set(BaseTargetName MidiViewer)
set(SOURCES_DIR src)
set(JUCE_DIR ../JUCE)


# GLM
set(GLM_DIR external/glm)
add_subdirectory(${GLM_DIR})


add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../JUCE ${CMAKE_CURRENT_BINARY_DIR}/JUCE)

juce_add_plugin("${BaseTargetName}"
    # VERSION ...                               # Set this if the plugin version is different to the project version
    # ICON_BIG ...                              # ICON_* arguments specify a path to an image file to use as an icon for the Standalone
    # ICON_SMALL ...
    # COMPANY_NAME ...                          # Specify the name of the plugin's author
    IS_SYNTH TRUE                               # Is this a synth or an effect?
    NEEDS_MIDI_INPUT TRUE             			# Does the plugin need midi input?
    NEEDS_MIDI_OUTPUT TRUE              		# Does the plugin need midi output?
    IS_MIDI_EFFECT FALSE                		# Is this plugin a MIDI effect?
    EDITOR_WANTS_KEYBOARD_FOCUS FALSE   		# Does the editor need keyboard focus?
    COPY_PLUGIN_AFTER_BUILD FALSE        		# Should the plugin be installed to a default location after building?
    PLUGIN_MANUFACTURER_CODE Frac               # A four-character manufacturer id with at least one upper-case character
    PLUGIN_CODE FRmv                            # A unique four-character plugin id with at least one upper-case character
    FORMATS AU VST3 Standalone                  # The formats to build. Other valid formats are: AAX Unity VST AU AUv3
    PRODUCT_NAME "Midi Viewer"            # The name of the final executable, which can differ from the target name
)

juce_add_binary_data(binary_resources SOURCES
    resources/fifthsgon.svg
    resources/fifthstext.svg
)

juce_generate_juce_header(${BaseTargetName})

target_sources(${BaseTargetName} PRIVATE
    ${SOURCES_DIR}/PluginEditor.cpp
    ${SOURCES_DIR}/PluginProcessor.cpp
    ${SOURCES_DIR}/FifthsReactiveComponent.cpp
)
target_include_directories(${BaseTargetName} PRIVATE
	"include"
    PRIVATE ${GLM_DIR}/include
)

target_compile_definitions(${BaseTargetName} PUBLIC
    # JUCE_WEB_BROWSER and JUCE_USE_CURL would be on by default, but you might not need them.
    JUCE_WEB_BROWSER=0  # If you remove this, add `NEEDS_WEB_BROWSER TRUE` to the `juce_add_plugin` call
    JUCE_USE_CURL=0     # If you remove this, add `NEEDS_CURL TRUE` to the `juce_add_plugin` call
    JUCE_VST3_CAN_REPLACE_VST2=0
)
	
target_link_libraries(${BaseTargetName} PRIVATE
    glm
    binary_resources
    juce::juce_audio_utils
	juce::juce_recommended_config_flags
	juce::juce_recommended_lto_flags
	juce::juce_recommended_warning_flags
    juce::juce_core
    juce::juce_data_structures
    juce::juce_events
    juce::juce_graphics
    juce::juce_gui_basics
    juce::juce_gui_extra
    juce::juce_opengl
)