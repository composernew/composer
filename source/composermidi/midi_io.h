//
// Created by Carla on 28/06/2022.
//

#ifndef COMPOSER_MIDI_IO_H
#define COMPOSER_MIDI_IO_H

#include "composerlib/melody.h"
#include <MidiFile.h>

namespace composer {

    [[maybe_unused]] void export_to_midi(const std::string &filename, const melody &melody);

    melody_problem import_midi_file(const std::string &filename);

    melody_problem to_midi_problem(const smf::MidiFile &midi_file);

    static std::vector<std::tuple<int,int,int>> import_melody(smf::MidiEventList event_list);
} // namespace composer

#endif // COMPOSER_MIDI_IO_H
