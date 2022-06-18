/*
Copyright (c) 2022 Jakob Ketterl <jakob.ketterl@gmx.de>

This file is part of libcsdr.

libcsdr is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

libcsdr is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with libcsdr.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once

#include "module.hpp"
#include <opus/opus.h>

namespace Csdr {

    class OpusEncoder: public Module<short, unsigned char> {
        public:
            OpusEncoder();
            ~OpusEncoder() override;
            bool canProcess() override;
            void process() override;
        private:
            ::OpusEncoder* encoder;
            int frame_size = 480;
    };

}