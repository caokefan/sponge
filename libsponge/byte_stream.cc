#include "byte_stream.hh"
#include <cstddef>
#include <string>

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) : _capacity(capacity),
    readCnt(0), writeCnt(0), inputEnd(false)
 { DUMMY_CODE(capacity); }

size_t ByteStream::write(const string &data) {
    // DUMMY_CODE(data);
    // return {};
    size_t remainder = remaining_capacity();
    size_t byteWrite = (data.size() >= remainder) ? remainder : data.size();
    size_t cnt = 0;
    while(cnt < byteWrite) {
        buffer.push_back(data[cnt++]);
    }

    writeCnt += byteWrite;
    return byteWrite;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    // DUMMY_CODE(len);
    // return {};
    string peekStr = "";
    size_t remainder = buffer_size();
    size_t bytePeek = len >= remainder ? remainder : len;
    size_t cnt = 0;
    while(cnt < bytePeek){
        peekStr.push_back(buffer[cnt++]);
    }
    return peekStr;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) { 
    // DUMMY_CODE(len); 
    size_t remainder = buffer_size();
    size_t byteRemoved = len >= remainder ? remainder : len;
    size_t cnt = 0;
    while(cnt++ < byteRemoved) {
        buffer.pop_front();
    }
    readCnt += byteRemoved;
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    // DUMMY_CODE(len);
    // return {};
    string peekStr = peek_output(len);
    pop_output(len);
    return peekStr;
}

void ByteStream::end_input() { inputEnd = true; }

bool ByteStream::input_ended() const { return inputEnd; }

size_t ByteStream::buffer_size() const { return buffer.size(); }

bool ByteStream::buffer_empty() const { return buffer.empty(); }

bool ByteStream::eof() const { return input_ended() && buffer_empty(); }

size_t ByteStream::bytes_written() const { return writeCnt; }

size_t ByteStream::bytes_read() const { return readCnt; }

size_t ByteStream::remaining_capacity() const { return _capacity - buffer.size(); }
