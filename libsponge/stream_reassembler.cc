#include "stream_reassembler.hh"
#include <cstddef>
#include <iostream>
#include <limits>
#include <stdexcept>
#include <system_error>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : buf(), end_index(numeric_limits<size_t>::max()), _output(capacity), _capacity(capacity) {}

//! \details This function accepts a substring (aka a segment) of bytes,
//! possibly out-of-order, from the logical stream, and assembles any newly
//! contiguous substrings and writes them into the output stream in order.
void StreamReassembler::push_substring(const string &data, const size_t index, const bool eof) {
    DUMMY_CODE(data, index, eof);
    if (eof) {
        end_index = index + data.size();
    }
    size_t blue = _output.bytes_written() - _output.bytes_read();
    if(blue==_capacity){
        return;
    }
    if(data.size()==0){
        if(buf.empty() && end_index == _output.bytes_written()){
            _output.end_input();
        }
        return;
    }

    size_t max_idx = _output.bytes_read() + _capacity;

    if(index < _output.bytes_written()){
        if(index + data.size() < _output.bytes_written()) return;
        else {
            if(buf.find(index)!=buf.end()){
                if(buf[index].size() > data.size()) goto deOverlap;
                else {
                    buf.erase(buf.find(index));
                }
            } 
            size_t idx = _output.bytes_written();
            buf.insert({idx, data.substr(idx-index, min(data.size(), max_idx))});
        }
    } else if(index > max_idx) return;
    else {
        if(buf.find(index)!=buf.end() && buf[index].size() < data.size()){
            if (buf[index].size() > data.size()) goto deOverlap;
            else {
                buf.erase(buf.find(index));
            }
        } 
        size_t sec_idx = min(data.size(), max_idx - index);
        buf.insert({index, data.substr(0, sec_idx)});
    }

    deOverlap:
    for(map<size_t, string>::iterator i=buf.begin();next(i) != buf.end();i++){
        run:
            auto nxt = next(i);
            if(nxt == buf.end()) break;
            if(i->first + i->second.size() > nxt->first){
                if(i->first + i->second.size() > nxt->first + nxt->second.size()){
                    buf.erase(nxt);
                    goto run;
                } else {
                    size_t ins_idx = i->first + i->second.size();
                    size_t idx = nxt->first;
                    string tmp = nxt->second;
                    size_t l = i->first + i->second.size() - idx;
                    if(buf.find(ins_idx)!=buf.end()){
                        if(buf[ins_idx].size() + ins_idx > idx + tmp.size() ) {
                            buf.erase(nxt);
                            goto re;
                        }
                        else {
                            buf.erase(buf.find(ins_idx));
                        }
                    }
                    buf.insert({ins_idx, tmp.substr(l)});
                    buf.erase(nxt);
                }
            }
    }

    re:
    while(buf.begin()->first==_output.bytes_written()){
        _output.write(buf.begin()->second);
        buf.erase(buf.begin());
    }

    if(empty() && end_index == _output.bytes_written()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { 
    size_t unassembled = 0;
    for(auto i: buf){
        unassembled += i.second.size();
    }
    return unassembled; 
}

bool StreamReassembler::empty() const { return buf.empty(); }
