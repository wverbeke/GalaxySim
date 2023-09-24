import os
import sys

COMPILE_FLAGS = "-I/usr/include/hdf5/serial -L/usr/lib/x86_64-linux-gnu/hdf5/serial /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_hl_cpp.a /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_cpp.a /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5_hl.a /usr/lib/x86_64-linux-gnu/hdf5/serial/libhdf5.a -lcrypto -lcurl -lpthread -lsz -lz -ldl -lm -Wl,-rpath -Wl,/usr/lib/x86_64-linux-gnu/hdf5/serial -std=c++11"

if __name__ == "__main__":
    input_file = sys.argv[1]
    output_file = sys.argv[2] if len(sys.argv) >= 3 else "a.out"
    optimized = (sys.argv[3] == "opt") if len(sys.argv) >= 4 else False
    compile_command = f"g++ {input_file} -o {output_file} {COMPILE_FLAGS}"
    if optimized:
        compile_command += " -O3"
    os.system(compile_command)
