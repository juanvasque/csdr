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

#include "opus.hpp"

#include <iostream>

namespace Csdr {

    OpusEncoder::OpusEncoder(): Module<short, unsigned char>() {
        int errors;
        encoder = opus_encoder_create(12000, 1, OPUS_APPLICATION_AUDIO, &errors);
        opus_encoder_ctl(encoder, OPUS_SET_BITRATE(24000));
    }

    OpusEncoder::~OpusEncoder() {
        opus_encoder_destroy(encoder);
    }

    bool OpusEncoder::canProcess() {
        std::lock_guard<std::mutex> lock(this->processMutex);
        return reader->available() > frame_size;
    }

    void OpusEncoder::process() {
        std::lock_guard<std::mutex> lock(this->processMutex);
        int32_t encoded = opus_encode(encoder, reader->getReadPointer(), frame_size, writer->getWritePointer(), writer->writeable());
        reader->advance(frame_size);
        if (encoded > 0) {
            writer->advance(encoded);
        } else {
            std::cerr << "opus encoder error " << encoded << "\n";
        }
    }

}