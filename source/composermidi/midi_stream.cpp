#include "midi_stream.h"
#include <iostream>
#include <sstream>
#include <vector>

namespace composer {

    cycfi::q::midi::raw_message to_raw_message(smf::MidiMessage &midi_message) {

        int x = 0;
        cycfi::q::midi::raw_message message{};
        std::stringstream stream;

        // Converts messages to hexadecimal
        for(size_t i = midi_message.size(); i > 0; --i){
            stream << std::hex << static_cast<int>(midi_message[i-1]);
        }

        stream >> x;

        message.data = x;

        return message;
    }

    bool is_harmony(smf::MidiEventList &event_list) {

        constexpr int note_on = 0x90;

        if(event_list.size() == 0){
            return false;
        }

        for (int event = 1; event < event_list.size(); ++event) {
            if (event_list[event][0] == note_on && event_list[event-1][0] == note_on) {
                return true;
            }
        }

        return false;
    }

    void dispatch_midi_messages(midi_processor &processor, smf::MidiEventList &event_list) {

        if(is_harmony(event_list)){
            throw std::runtime_error("This MIDI file contains harmonies: not supported.");
        }

        event_list.linkNotePairs();

        for (int event = 0; event < event_list.size(); ++event) {

            cycfi::q::midi::raw_message message = to_raw_message(event_list[event]);
            cycfi::q::midi::dispatch(message, event_list[event].tick, processor);
            cycfi::q::sleep(event_list[event].getDurationInSeconds());
        }
    }
}