# SGBM minimal kernel bundle

This directory contains a **standalone** subset of the Vitis Vision library needed to build the Semi-Global Block Matching (SGBM) kernel. It mirrors the original headers and example configuration so you can drop this folder into a new repo and wire it into your Vitis HLS build.

## Contents

- `include/imgproc/xf_sgbm.hpp`: SGBM implementation header.
- `include/common/*`: required Vitis Vision common headers.
- `config/xf_config_params.h`: configuration used by the example wrapper.
- `src/sgbm_top.cpp`: top-level HLS wrapper that instantiates `SemiGlobalBM`.
- `CMakeLists.txt`: starter CMake file for building a library target.

## Notes

- You still need Vitis HLS headers (`hls_stream.h`, `ap_int.h`, `ap_axi_sdata.h`) available in your toolchain include path.
- Parameter constraints are enforced inside `xf_sgbm.hpp` (window size = 5, NDISP <= 256, etc.).

