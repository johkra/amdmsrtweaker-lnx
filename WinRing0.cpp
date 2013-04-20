/*
 * Copyright (c) Martin Kinkelin
 *
 * See the "License.txt" file in the root directory for infos
 * about permitted and prohibited uses of this code.
 */

#include <cstdio>
#include <cstdlib>

#include <fcntl.h>
#include <unistd.h>

#include "WinRing0.h"
#include "StringUtils.h"

using std::exception;
using std::string;

uint32_t ReadPciConfig(uint32_t device, uint32_t function, uint32_t regAddress) {
    uint32_t result;
    char path[255]= "\0";
    sprintf(path, "/proc/bus/pci/00/%x.%x", device, function);

    int pci = open(path, O_RDONLY);
    if (pci == -1) {
        perror("Failed to open pci device for reading");
        exit(-1);
    }
    pread(pci, &result, sizeof(result), regAddress);
    close(pci);

    return result;
}

void WritePciConfig(uint32_t device, uint32_t function, uint32_t regAddress, uint32_t value) {
    char path[255]= "\0";
    sprintf(path, "/proc/bus/pci/00/%x.%x", device, function);

    int pci = open(path, O_WRONLY);
    if (pci == -1) {
        perror("Failed to open pci device for writing");
        exit(-1);
    }
    if(pwrite(pci, &value, sizeof(value), regAddress) != sizeof(value)) {
        perror("Failed to write to pci device");
    }
    close(pci);
}


uint64_t Rdmsr(uint32_t index) {
    uint64_t result;

    int msr = open("/dev/cpu/0/msr", O_RDONLY);
    if (msr == -1) {
        perror("Failed to open msr device for reading");
        exit(-1);
    }
    pread(msr, &result, sizeof(result), index);
    close(msr);

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
        int msr = open(path, O_WRONLY);
        if (msr == -1) {
            perror("Failed to open msr device for writing");
            exit(-1);
        }
        if(pwrite(msr, &value, sizeof(value), index) != sizeof(value)) {
            perror("Failed to write to msr device");
        }
        close(msr);
    }
}


CpuidRegs Cpuid(uint32_t index) {
    CpuidRegs result;

    FILE* cpuid = fopen("/dev/cpu/0/cpuid", "r");
    if (cpuid == NULL) {
        perror("Failed to open cpuid device for reading");
        exit(-1);
    }
    fseek(cpuid, index, SEEK_SET);
    fread(&(result.eax), sizeof(result.eax), 1, cpuid);
    fread(&(result.ebx), sizeof(result.ebx), 1, cpuid);
    fread(&(result.ecx), sizeof(result.ecx), 1, cpuid);
    fread(&(result.edx), sizeof(result.edx), 1, cpuid);
    fclose(cpuid);

    return result;
}

