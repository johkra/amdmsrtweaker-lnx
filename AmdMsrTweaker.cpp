/*
 * Copyright (c) Martin Kinkelin
 *
 * See the "License.txt" file in the root directory for infos
 * about permitted and prohibited uses of this code.
 */

#include <cstdio>
#include <iostream>
#include "Info.h"
#include "Worker.h"
#include "WinRing0.h"

using std::cout;
using std::cerr;
using std::endl;


void PrintInfo(const Info& info);

/// <summary>Entry point for the program.</summary>
int main(int argc, const char* argv[]) {
    try {
        Info info;
        if (!info.Initialize()) {
            cout << "ERROR: unsupported CPU" << endl;
            return 2;
        }

        if (argc > 1) {
            Worker worker(info);

            if (!worker.ParseParams(argc, argv)) {
                return 3;
            }

            worker.ApplyChanges();
        } else {
            PrintInfo(info);
        }
    } catch (const std::exception& e) {
        cerr << "ERROR: " << e.what() << endl;
        return 10;
    }

    return 0;
}


void PrintInfo(const Info& info) {
    cout << endl;
    cout << "AmdMsrTweaker v1.1" << endl;
    cout << endl;

    cout << ".:. General" << endl << "---" << endl;
    cout << "  AMD family 0x" << std::hex << info.Family << ", model 0x" << info.Model << std::dec << " CPU, " << info.NumCores << " cores" << endl;
    cout << "  Default reference clock: " << info.multiScaleFactor * 100 << " MHz" << endl;
    cout << "  Available multipliers: " << (info.MinMulti / info.multiScaleFactor) << " .. " << (info.MaxSoftwareMulti / info.multiScaleFactor) << endl;
    cout << "  Available voltage IDs: " << info.MinVID << " .. " << info.MaxVID << " (" << info.VIDStep << " steps)" << endl;
    cout << endl;

    cout << ".:. Turbo" << endl << "---" << endl;
    if (!info.IsBoostSupported)
        cout << "  not supported" << endl;
    else {
        cout << "  " << (info.IsBoostEnabled ? "enabled" : "disabled") << endl;
        cout << "  " << (info.IsBoostLocked ? "locked" : "unlocked") << endl;

        if (info.MaxMulti != info.MaxSoftwareMulti)
            cout << "  Max multiplier: " << (info.MaxMulti / info.multiScaleFactor) << endl;
    }
    cout << endl;

    cout << ".:. P-states" << endl << "---" << endl;
    cout << "  " << info.NumPStates << " of " << (info.Family == 0x10 ? 5 : 8) << " enabled (P0 .. P" << (info.NumPStates - 1) << ")" << endl;

    if (info.IsBoostSupported && info.NumBoostStates > 0) {
        cout << "  Turbo P-states:";
        for (int i = 0; i < info.NumBoostStates; i++)
            cout << " P" << i;
        cout << endl;
    }

    cout << "  ---" << endl;

    for (int i = 0; i < info.NumPStates; i++) {
        const PStateInfo pi = info.ReadPState(i);

        cout << "  P" << i << ": " << (pi.Multi / info.multiScaleFactor) << "x at " << info.DecodeVID(pi.VID) << "V" << endl;

        if (pi.NBPState >= 0) {
            cout << "      NorthBridge in NB_P" << pi.NBPState;
            if (pi.NBVID >= 0)
                cout << " at " << info.DecodeVID(pi.NBVID) << "V";
            cout << endl;
        }
    }

    if (info.Family == 0x15) {
        cout << "  ---" << endl;

        for (int i = 0; i < info.NumNBPStates; i++) {
            const NBPStateInfo pi = info.ReadNBPState(i);
            cout << "  NB_P" << i << ": " << pi.Multi << "x at " << info.DecodeVID(pi.VID) << "V" << endl;
        }
    }
}
