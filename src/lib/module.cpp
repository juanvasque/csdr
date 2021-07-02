#include "module.hpp"

#include <algorithm>

using namespace Csdr;

template <typename T, typename U>
void Module<T, U>::setReader(RingbufferReader<T>* reader) {
    this->reader = reader;
}

template <typename T, typename U>
void Module<T, U>::setWriter(Writer<U>* writer) {
    this->writer = writer;
}

template <typename T, typename U>
void AnyLengthModule<T, U>::process() {
    size_t available;
    while (available = this->reader->available()) {
        size_t work_size = std::min({available, this->writer->writeable(), maxLength()});
        process(this->reader->getReadPointer(), this->writer->getWritePointer(), work_size);
        this->reader->advance(work_size);
        this->writer->advance(work_size);
    }
}

template <typename T, typename U>
void FixedLengthModule<T, U>::process () {
    size_t available;
    size_t length;
    while ((available = this->reader->available()) > (length = getLength())) {
        process(this->reader->getReadPointer(), this->writer->getWritePointer());
        this->reader->advance(length);
        this->writer->advance(length);
    }
}