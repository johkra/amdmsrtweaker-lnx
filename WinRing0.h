/*
 * Copyright (c) Martin Kinkelin
 *
 * See the "License.txt" file in the root directory for infos
 * about permitted and prohibited uses of this code.
 */

#pragma once

#include <stdint.h>

#include <exception>
#include <string>

struct CpuidRegs {
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
};

static const uint32_t AMD_CPU_DEVICE = 0x18; // first AMD CPU

uint32_t ReadPciConfig(uint32_t device, uint32_t function, uint32_t regAddress);
void WritePciConfig(uint32_t device, uint32_t function, uint32_t regAddress, uint32_t value);

uint64_t Rdmsr(uint32_t index);
void Wrmsr(uint32_t index, const uint64_t& value);

CpuidRegs Cpuid(uint32_t index);


template <typename T> uint32_t GetBits(T value, unsigned char offset, unsigned char numBits) {
    const T mask = (((T)1 << numBits) - (T)1); // 2^numBits - 1; after right-shift
    return (uint32_t)((value >> offset) & mask);
}

template <typename T> void SetBits(T& value, uint32_t bits, unsigned char offset, unsigned char numBits) {
    const T mask = (((T)1 << numBits) - (T)1) << offset; // 2^numBits - 1, shifted by offset to the left
    value = (value & ~mask) | (((T)bits << offset) & mask);
}

class ExceptionWithMessage: public std::exception {
    const char* what;

public:

    ExceptionWithMessage(std::string msg) {
        this->what = msg.c_str();
    }
};
