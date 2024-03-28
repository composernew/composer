/*=============================================================================
   Copyright (c) 2014-2021 Joel de Guzman. All rights reserved.
   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/

#ifndef COMPOSER_MIDI_PROCESSOR_H
#define COMPOSER_MIDI_PROCESSOR_H

#include <composermidi/synth.h>
#include <q/support/midi.hpp>

namespace composer {

    /// \brief Handle MIDI messages and play them with a synthesizer.
    ///
    /// The synthesizer process note-on and note-off MIDI events using operators from cycfi::q::midi::processor

    class midi_processor : cycfi::q::midi::processor {

      public:

        using cycfi::q::midi::processor::operator();

        explicit midi_processor(synth &synthesizer)
            : synthesizer_(synthesizer)
        {}

        void operator()(cycfi::q::midi::note_on msg, std::size_t time);

        void operator()(cycfi::q::midi::note_off msg, std::size_t time);

      private:

        synth &synthesizer_;
    };
}

#endif // COMPOSER_MIDI_PROCESSOR_H
