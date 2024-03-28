#define CATCH_CONFIG_MAIN

#include <catch2/catch.hpp>
#include <composermidi/midi_stream.h>
#include <MidiFile.h>

using cycfi::q::literals::operator""_ms;
using cycfi::q::literals::operator""_s;
using cycfi::q::literals::operator""_dB;

using namespace composer;

TEST_CASE ("MidiEventList contains harmony") {

    // Empty MIDI file
    smf::MidiFile midi_file;

    REQUIRE_FALSE(is_harmony(midi_file[0]));

    // MIDI file containing melody
    midi_file.read("resources/twinkle.midi");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE_FALSE(is_harmony(midi_file[0]));

    // MIDI file containing harmony
    midi_file.read("resources/twinkle_harmonic.mid");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE(is_harmony(midi_file[0]));
}

TEST_CASE ("Dispatch MIDI messages to synthesizer") {

    // Test resources
    auto env_cfg = cycfi::q::envelope::config {
        100_ms    // attack rate
        , 1_s     // decay rate
        , -12_dB // sustain level
        , 5_s    // sustain rate
        , 1_s    // release rate
    };

    synth synth_(env_cfg);
    midi_processor processor(synth_);
    smf::MidiFile midi_file;

    // MIDI file containing melody
    midi_file.read("resources/twinkle.midi");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    // Exception throws are not expected because the MIDI file contains only melodies
    REQUIRE_NOTHROW(dispatch_midi_messages(processor, midi_file[0]));

    midi_file.read("resources/twinkle_harmonic.mid");

    if (midi_file.getTrackCount() > 1) {
        midi_file.joinTracks();
    }

    REQUIRE_THROWS_WITH(dispatch_midi_messages(processor, midi_file[0]), "This MIDI file contains harmonies: not supported.");
}

TEST_CASE ("smf::MidiMessage to raw message") {

    WHEN ("The message is a valid note-on") {

        smf::MidiMessage message = {144, 48, 64};

        REQUIRE(to_raw_message(message).data == 4206736);
    }

    WHEN ("The message is a valid note-off") {

        smf::MidiMessage message = {128, 53, 64};

        REQUIRE(to_raw_message(message).data == 4208000);
    }

    WHEN ("The message is empty") {

        smf::MidiMessage message;

        REQUIRE(to_raw_message(message).data == 0);
    }
}