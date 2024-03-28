#ifndef COMPOSER_MIDI_STREAM_H
#define COMPOSER_MIDI_STREAM_H

#include <MidiEventList.h>
#include <MidiMessage.h>
#include <utility>
#include "midi_processor.h"

/// \file midi_stream.h
/// \brief Integration of cycfi::q::midi and smf::MidiEventList libraries.
///
/// This library uses the \ref midi_processor class to process an event list from smf::MidiEventList.
/// That way is possible to cycfi::q::midi to handle all MIDI messages to play the event list.

namespace composer {

    /// Converts a midi message from smf::MidiMessage to a midi raw message from cycfi::q::midi.
    cycfi::q::midi::raw_message to_raw_message(smf::MidiMessage &midi_message);

    /// Verifies if the event list contains harmonies.
    bool is_harmony(smf::MidiEventList &event_list);

    /// Iterates through the event list and dispatch the midi messages using the processor.
    void dispatch_midi_messages(midi_processor &processor, smf::MidiEventList &event_list);
}

#endif // COMPOSER_MIDI_STREAM_H
