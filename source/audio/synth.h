#ifndef COMPOSER_SYNTH_H
#define COMPOSER_SYNTH_H

#include <q/synth/sin.hpp>
#include <q/synth/square.hpp>
#include <q/synth/triangle.hpp>
#include <q/synth/saw.hpp>
#include <q/synth/envelope.hpp>
#include <q/fx/lowpass.hpp>
#include <q/fx/waveshaper.hpp>
#include <q_io/audio_stream.hpp>
#include <vector>

// Based on Q 'sin_synth' example - https://github.com/cycfi/q/blob/master/example/sin_synth.cpp

class synth : public cycfi::q::port_audio_stream {

    void process(out_channels const& out) override;

    void set(cycfi::q::frequency const &freq, cycfi::q::duration const &dur);

    cycfi::q::reso_filter       filter; // The resonant filter
    cycfi::q::soft_clip         clip;   // Soft clip

  public:

    enum class timbre {sine, triangle, square, saw};

    timbre timbre_;

    explicit synth(cycfi::q::envelope::config &envelope_config, timbre timbre = timbre::sine)
        : port_audio_stream( 0, 2)
        , timbre_(timbre)
        , env(envelope_config, this->sampling_rate())
        , filter(0.5, 0.8) {}

    void play(std::vector<std::tuple<int,cycfi::q::frequency,cycfi::q::duration>> &notes);

    void set_timbre(timbre timbre);

    cycfi::q::phase_iterator    phase;  // The phase iterator
    cycfi::q::envelope          env;    // The envelope
};

#endif // COMPOSER_SYNTH_H