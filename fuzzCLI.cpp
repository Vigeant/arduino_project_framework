// afl-clang-fast++ -o bin/fuzzCLI aflHarnesses/fuzzCLI.cpp
// cd bin && afl-fuzz -i input_dir -o output_dir ./fuzzCLI

#include "CommandLine.h"
Settings settings;
extern "C" int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size) {
    CommandLine cli;
    
    // Loop over each character in the input data
    for (size_t i = 0; i < Size; ++i) {
        // Call the function to be fuzzed with each character
        cli.doCommandLine((char)Data[i]);
    }

    return 0;
}
