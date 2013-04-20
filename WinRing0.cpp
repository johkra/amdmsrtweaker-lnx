/*
 * Copyright (c) Martin Kinkelin
 *
 * See the "License.txt" file in the root directory for infos
 * about permitted and prohibited uses of this code.
 */

#include <cstdio>

#include "WinRing0.h"
#include "StringUtils.h"

using std::exception;
using std::string;

uint32_t ReadPciConfig(uint32_t device, uint32_t function, uint32_t regAddress) {
    uint32_t result;
    char path[255]= "\0";
    sprintf(path, "/proc/bus/pci/00/%x.%x", device, function);

    FILE* pci = fopen(path, "r");
    fseek(pci, regAddress, SEEK_SET);
    fread(&result, sizeof(result), 1, pci);
    fclose(pci);

    return result;
}

void WritePciConfig(uint32_t device, uint32_t function, uint32_t regAddress, uint32_t value) {
    char path[255]= "\0";
    sprintf(path, "/proc/bus/pci/00/%d.%d", device, function);

    FILE* pci = fopen(path, "w");
    fseek(pci, function, SEEK_SET);
    fwrite(&value, sizeof(value), 1, pci);
    fclose(pci);
}


uint64_t Rdmsr(uint32_t index) {
    uint64_t result;

    FILE* msr = fopen("/dev/cpu/0/msr", "r");
    fseek(msr, index, SEEK_SET);
    fread(&result, sizeof(result), 1, msr);
    fclose(msr);

    return result;
}

int get_num_cpu() {
    CpuidRegs regs = Cpuid(0x80000008);
    return 1 + (regs.ecx&0xff);
}

void Wrmsr(uint32_t index, const uint64_t& value) {
    char path[255]= "\0";

    for (int i = 0; i < get_num_cpu(); i++) {
        sprintf(path, "/dev/cpu/%d/msr", i);
        FILE* msr = fopen(path, "w");
        fseek(msr, index, SEEK_SET);
        fwrite(&value, sizeof(value), 1, msr);
        fclose(msr);
    }
}


CpuidRegs Cpuid(uint32_t index) {
    CpuidRegs result;

    FILE* cpuid = fopen("/dev/cpu/0/cpuid", "r");
    fseek(cpuid, index, SEEK_SET);
    fread(&(result.eax), sizeof(result.eax), 1, cpuid);
    fread(&(result.ebx), sizeof(result.ebx), 1, cpuid);
    fread(&(result.ecx), sizeof(result.ecx), 1, cpuid);
    fread(&(result.edx), sizeof(result.edx), 1, cpuid);
    fclose(cpuid);

    return result;
}

