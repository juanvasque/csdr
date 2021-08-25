#include "power.hpp"

#include <cstring>
#include <utility>

using namespace Csdr;

Power::Power(unsigned int decimation, std::function<void(float)> callback): decimation(decimation), callback(std::move(callback)) {}

bool Power::canProcess() {
    std::lock_guard<std::mutex> lock(processMutex);
    size_t length = getLength();
    return (reader->available() > length && writer->writeable() > length);
}

void Power::process() {
    std::lock_guard<std::mutex> lock(processMutex);
    complex<float>* input = reader->getReadPointer();
    size_t length = getLength();

    float acc = 0;
    for (size_t i = 0; i < length; i += decimation){
        acc += std::norm(input[i]);
    }
    float power = acc / ceilf((float) length / decimation);
    callback(power);
    // pass data
    forwardData(input, power);

    reader->advance(length);
}

size_t Power::getLength() {
    return 1024;
}

void Power::forwardData(complex<float>* input, float power) {
    complex<float>* output = writer->getWritePointer();
    size_t length = getLength();
    std::memcpy(output, input, length * sizeof(complex<float>));
    writer->advance(length);
}

void Squelch::setSquelch(float squelchLevel) {
    this->squelchLevel = squelchLevel;
}

void Squelch::forwardData(complex<float> *input, float power) {
    if (squelchLevel == 0 || power >= squelchLevel) {
        Power::forwardData(input, power);
        flushCounter = 0;
    } else if (flushCounter < 5) {
        // produce some 0s to flush any subsequent modules if they have any overhead (e.g. FIR filter delays)
        size_t length = getLength();
        complex<float>* output = writer->getWritePointer();
        std::memset(output, 0, sizeof(complex<float>) * length);
        writer->advance(length);
        // increment inside because an unsigned char would overflow soon...
        flushCounter++;
    }
}