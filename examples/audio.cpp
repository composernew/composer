#include <composermidi/synth.h>
#include <q/support/notes.hpp>

using cycfi::q::literals::operator""_ms;
using cycfi::q::literals::operator""_s;
using cycfi::q::literals::operator""_dB;

int main(){

    // 6 string guitar frequencies:
    constexpr auto low_e   = cycfi::q::notes::E[2];
    constexpr auto a       = cycfi::q::notes::A[2];
    constexpr auto d       = cycfi::q::notes::D[3];
    constexpr auto g       = cycfi::q::notes::G[3];
    constexpr auto b       = cycfi::q::notes::B[3];
    constexpr auto high_e  = cycfi::q::notes::E[4];

    std::vector<std::tuple<int,cycfi::q::frequency,cycfi::q::duration>> notes = {{10, low_e, 2},{8, a, 2},{6, d, 2},
                                                      {4, g, 2},{2, b, 2},{0, high_e, 2}};

    // Synthesizer envelope config
    auto env_cfg = cycfi::q::envelope::config {
        100_ms    // attack rate
        , 1_s     // decay rate
        , -12_dB // sustain level
        , 5_s    // sustain rate
        , 1_s    // release rate
    };

    // Synthesizer timbres

    // sine timbre by default
    composer::synth _synth(env_cfg);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(composer::synth::timbre::triangle);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(composer::synth::timbre::square);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    _synth.set_timbre(composer::synth::timbre::saw);
    _synth.start();
    _synth.play(notes);
    _synth.stop();

    return 0;
}