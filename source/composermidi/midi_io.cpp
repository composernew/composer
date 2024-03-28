//
// Created by Carla on 28/06/2022.
//

#include "midi_io.h"
#include "composerlib/melody_problem.h"
#include <sstream>
#include <iostream>
#include <utility>

namespace composer {

    [[maybe_unused]] void export_to_midi(const std::string &filename, const melody &melody) {

        smf::MidiFile midi_file;
        int track   = 0;
        int channel = 0;
        int instrument = 1; // Acoustic Grand Piano
        midi_file.addTimbre(track, 0, channel, instrument);

        midi_file.setTPQ(static_cast<int>(melody.get_rhythm()));
        int start_tick = 0;
        for (const auto &note : melody.get_melody()) {
            int end_tick = start_tick + std::get<1>(note);

            if (std::get<0>(note) == 20) {
                start_tick += std::get<1>(note);
            }
            else {
                midi_file.addNoteOn (track, start_tick, channel, std::get<0>(note), std::get<2>(note));
                midi_file.addNoteOff(track, end_tick,   channel, std::get<0>(note));
                start_tick = end_tick;
            }
        }
        midi_file.write(filename);
    }

    static std::vector<std::tuple<int,int,int>> import_melody(smf::MidiEventList event_list) {
        std::vector<std::tuple<int,int,int>> melody;
        std::stringstream stream_note;
        int note = 0;
        int pause_duration = 0;
        int duration = 0;
        int velocity = 0;

        if (event_list.size() != 0) {
            for (int event = 0; event < event_list.size(); ++event) {

                if (event_list[event].isNoteOn()) {

                    if (melody.empty()) duration = event_list[event+1].tick;
                    else duration = event_list[event].tick - event_list[event-2].tick;

                    if (!melody.empty()) {
                        pause_duration = event_list[event].tick - event_list[event-1].tick;
                        if (pause_duration > 5) {
                            melody.emplace_back(std::make_tuple(static_cast<int>(melody_problem::feature_type::pause), pause_duration, 0));
                        }
                    }

                    velocity = event_list[event].getVelocity();

                    stream_note << std::hex << static_cast<int>(event_list[event][1]);
                    stream_note >> note;
                    melody.emplace_back(std::make_tuple(note, (duration- pause_duration), velocity));

                    stream_note.str(std::string());
                    stream_note.clear();
                }
            }
        }

        return melody;
    }

    melody_problem to_midi_problem(const smf::MidiFile &midi_file) {

        using rhythm_type =  melody_problem::feature_type;

        melody_problem problem(melody_problem::problem_type::random);

        problem.set_rhythm(midi_file.getTicksPerQuarterNote());
        problem.set_melody(import_melody(midi_file[0]));

        if(problem.get_rhythm() > static_cast<int>(rhythm_type::fastest_tempo))
            problem.set_rhythm(static_cast<int>(rhythm_type::fastest_tempo));
        if(problem.get_rhythm() < static_cast<int>(rhythm_type::slowest_tempo))
            problem.set_rhythm(static_cast<int>(rhythm_type::slowest_tempo));

        return problem;
    }

    melody_problem import_midi_file(const std::string &filename) {

        smf::MidiFile midi_file;

        midi_file.read(filename);

        if (midi_file.getTrackCount() > 1) {
            midi_file.joinTracks();
        }

        melody_problem problem = to_midi_problem(midi_file);

        return problem;
    }
} // namespace composer
