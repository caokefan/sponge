#include "wrapping_integers.hh"
<<<<<<< HEAD
=======
#include <cstdint>
#include <iostream>
>>>>>>> lab2

// Dummy implementation of a 32-bit wrapping integer

// For Lab 2, please replace with a real implementation that passes the
// automated checks run by `make check_lab2`.

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

//! Transform an "absolute" 64-bit sequence number (zero-indexed) into a WrappingInt32
//! \param n The input absolute 64-bit sequence number
//! \param isn The initial sequence number
WrappingInt32 wrap(uint64_t n, WrappingInt32 isn) {
    DUMMY_CODE(n, isn);
    uint64_t mod = uint64_t{1};
    mod = mod << 32;
    uint64_t isn_64 = static_cast<uint64_t>(isn.raw_value());
    n = n % mod;
    uint32_t v = static_cast<uint32_t>((n+isn_64) % mod);
    return WrappingInt32{v};
}

//! Transform a WrappingInt32 into an "absolute" 64-bit sequence number (zero-indexed)
//! \param n The relative sequence number
//! \param isn The initial sequence number
//! \param checkpoint A recent absolute 64-bit sequence number
//! \returns the 64-bit sequence number that wraps to `n` and is closest to `checkpoint`
//!
//! \note Each of the two streams of the TCP connection has its own ISN. One stream
//! runs from the local TCPSender to the remote TCPReceiver and has one ISN,
//! and the other stream runs from the remote TCPSender to the local TCPReceiver and
//! has a different ISN.
uint64_t unwrap(WrappingInt32 n, WrappingInt32 isn, uint64_t checkpoint) {
    DUMMY_CODE(n, isn, checkpoint);
    int32_t steps = n - wrap(checkpoint, isn);
    int64_t res = checkpoint + steps;
    return res >= 0 ? res : res + (1ul << 32);
}
