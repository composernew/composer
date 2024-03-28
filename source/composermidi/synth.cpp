#include <composermidi/synth.h>

namespace composer {

    void synth::process(const out_channels &out) {

        auto left = out[0];
        auto right = out[1];

        for (auto frame : out.frames()) {

            // Generate the ADSR envelope
            auto env_ = env();

            // Set the filter frequency
            filter.cutoff(env_);

            // Synthesize the sine wave
            auto val = cycfi::q::sin(phase++);

            switch (timbre_) {
            case timbre::triangle:
                val = cycfi::q::triangle(phase++);
                break;
            case timbre::square:
                val = cycfi::q::square(phase++);
                break;
            case timbre::saw:
                val = cycfi::q::saw(phase++);
                break;
            default:
                val = cycfi::q::sin(phase++);
            }

            // Apply the envelope (amplifier and filter) with soft clip
            val = clip(filter(val) * env_);

            //Output
            right[frame] = left[frame] = val;
        }
    }

    void synth::set(const cycfi::q::frequency &freq, const cycfi::q::duration &dur) {
        phase.set(freq, this->sampling_rate());
        env.trigger(0.3);
        cycfi::q::sleep(dur);
    }

    void synth::play(std::vector<std::tuple<int, cycfi::q::frequency, cycfi::q::duration>> &notes){

        using note_type = std::tuple<int, cycfi::q::frequency,cycfi::q::duration>;
        std::sort(notes.begin(),notes.end());

        for(note_type &note : notes){

            // plays the frequency with specified duration
            set(std::get<1>(note), std::get<2>(note));
        }
    }

    void synth::set_timbre(timbre timbre) {
        timbre_ = timbre;
    }
}
