#include <composermidi/synth.h>
#include <composermidi/midi_stream.h>
#include <iostream>
#include <MidiFile.h>

using cycfi::q::literals::operator""_ms;
using cycfi::q::literals::operator""_s;
using cycfi::q::literals::operator""_dB;

using namespace composer;

int main() {

    auto env_cfg = cycfi::q::envelope::config {
            100_ms    // attack rate
            , 1_s     // decay rate
            , -12_dB // sustain level
            , 5_s    // sustain rate
            , 1_s    // release rate
    };

    synth synth_(env_cfg);

    smf::MidiFile midi_file;

    if (midi_file.read("resources/exp-01.midi")){
        std::cout << "file found" << std::endl;

        midi_file.doTimeAnalysis();

        if (midi_file.getTrackCount() > 1) {

            // Converts multi-track data into a single time sequence.
            // This makes possible to process the event list as a single track list
            midi_file.joinTracks();
        }
    }
    else std::cout << "file not found" << std::endl;

    // Creates a stream to pass the MIDI Event List to the midi_processor
    //midi_stream stream(midi_file[0]);

    // Creates a processor to play the MIDI Event List
    midi_processor processor{synth_};

    synth_.start();

    dispatch_midi_messages(processor, midi_file[0]);

    synth_.stop();

    return 0;
}