#include "stream_reassembler.hh"
#include <cstddef>
#include <iostream>
#include <limits>

// Dummy implementation of a stream reassembler.

// For Lab 1, please replace with a real implementation that passes the
// automated checks run by `make check_lab1`.

// You will need to add private members to the class declaration in `stream_reassembler.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

StreamReassembler::StreamReassembler(const size_t capacity) : buf(), unassembled(0), end_index(numeric_limits<size_t>::max()), _output(capacity), _capacity(capacity) {}

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
                if(buf[index].size() < data.size()){
                    unassembled -= buf[index].size();
                    buf[index] = data.substr(0,min(data.size(), max_idx));
                    unassembled += buf[index].size();
                } 
            } else {
                int idx = _output.bytes_written();
                buf.insert({idx, data.substr(idx-index, min(data.size(), max_idx))});
                unassembled += buf[idx].size();
            }
        }
    } else if(index > max_idx) return;
    else {
        if(buf.find(index)!=buf.end()){
            if(buf[index].size() < data.size()){
                unassembled -= buf[index].size();
                int sec_idx = min(data.size(), max_idx - index);
                buf[index] = data.substr(0, sec_idx);
                unassembled += buf[index].size();
            }
        } else {
            int sec_idx = min(data.size(), max_idx - index);
            buf.insert({index, data});
            buf[index] = buf[index].substr(0, sec_idx);
            unassembled += buf[index].size();
        }
    }
    
    for(auto i=buf.begin();next(i) != buf.end();i++){
        run:
            auto nxt = next(i);
            if(nxt == buf.end()) break;
            int fir = i->first + i->second.size() - nxt->first;
            if(fir > 0){
                int ret = i->first + i->second.size() - nxt->first - nxt->second.size();
                if(ret > 0){
                    unassembled -= nxt->second.size();
                    buf.erase(nxt);
                    goto run;
                } else {
                    int idx = nxt->first;
                    string tmp = nxt->second;
                    int l = i->first + i->second.size() - idx;
                    buf.insert({i->first + i->second.size(), tmp.substr(l)});
                    unassembled += buf[i->first + i->second.size()].size();
                    unassembled -= nxt->second.size();
                    buf.erase(nxt);
                }
            }
    }
    
    // re:
    while(static_cast<size_t>(buf.begin()->first)==_output.bytes_written()){
        _output.write(buf.begin()->second);
        size_t len = buf.begin()->second.size();
        unassembled -= len;
        buf.erase(buf.begin());
    }

    if(empty() && end_index == _output.bytes_written()) {
        _output.end_input();
    }
}

size_t StreamReassembler::unassembled_bytes() const { return unassembled; }

bool StreamReassembler::empty() const { return buf.empty(); }
