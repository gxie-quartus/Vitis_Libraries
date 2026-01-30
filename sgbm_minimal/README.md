# SGBM minimal kernel bundle

This directory contains a **standalone** subset of the Vitis Vision library needed to build the Semi-Global Block Matching (SGBM) kernel. It mirrors the original headers and example configuration so you can drop this folder into a new repo and wire it into your Vitis HLS build.

## Contents

- `include/imgproc/xf_sgbm.hpp`: SGBM implementation header.
- `include/common/*`: required Vitis Vision common headers.
- `config/xf_config_params.h`: configuration used by the example wrapper.
- `src/sgbm_top.cpp`: top-level HLS wrapper that instantiates `SemiGlobalBM`.
- `csim/sgbm_csim.cpp`: simple C-sim harness that runs the kernel on your own images.
- `CMakeLists.txt`: starter CMake file for building a library target and optional C-sim executable.

## C-sim quick start

`sgbm_csim` is a lightweight C-simulation harness that loads a left/right grayscale image pair and writes a disparity image using the SGBM kernel.

```bash
mkdir -p build && cd build
cmake ..
cmake --build . --target sgbm_csim
./sgbm_csim <left.png> <right.png> <out.png> [penalty_small] [penalty_large]
```

Notes:
- The input images must be grayscale and smaller than or equal to `HEIGHT`/`WIDTH` in `config/xf_config_params.h`.
- The penalties default to `SMALL_PENALTY` and `LARGE_PENALTY` from the config header.
- `sgbm_csim` is only built when OpenCV is available.

## Notes

- You still need Vitis HLS headers (`hls_stream.h`, `ap_int.h`, `ap_axi_sdata.h`) available in your toolchain include path.
- Parameter constraints are enforced inside `xf_sgbm.hpp` (window size = 5, NDISP <= 256, etc.).

