# Minimal SGBM-only repo sketch

This note sketches a minimal repository layout that contains **only** the Vitis Vision Semi-Global Block Matching (SGBM) kernel plus the common headers it depends on. It is intended as a starting point for creating a new, small repo focused on SGBM.

## Minimal folder layout

```
sgbm-minimal/
├── CMakeLists.txt
├── include/
│   ├── common/
│   │   ├── xf_common.hpp
│   │   ├── xf_params.hpp
│   │   ├── xf_structs.hpp
│   │   ├── xf_types.hpp
│   │   ├── xf_utility.hpp
│   │   └── xf_video_mem.hpp
│   └── imgproc/
│       └── xf_sgbm.hpp
├── config/
│   └── xf_config_params.h
└── src/
    └── sgbm_top.cpp
```

### What goes where

- `include/imgproc/xf_sgbm.hpp`: **SGBM implementation** (copied from `vision/L1/include/imgproc/xf_sgbm.hpp`).
- `include/common/*`: **Core Vitis Vision common headers** (copied from `vision/L1/include/common/`).
- `config/xf_config_params.h`: **Example configuration** (start from `vision/L1/examples/sgbm/config/xf_config_params.h`).
- `src/sgbm_top.cpp`: **Your HLS top** (a wrapper that instantiates `SemiGlobalBM` with your settings).

## Minimal `sgbm_top.cpp` sketch

This wrapper mirrors the example’s style but removes unrelated code. Adjust types, sizes, and parameters as needed.

```cpp
#include "ap_int.h"
#include "hls_stream.h"

#include "config/xf_config_params.h"

void semiglobalbm_accel(ap_uint<INPUT_PTR_WIDTH>* img_in_l,
                        ap_uint<INPUT_PTR_WIDTH>* img_in_r,
                        unsigned char penalty_small,
                        unsigned char penalty_large,
                        ap_uint<OUTPUT_PTR_WIDTH>* img_out,
                        int rows,
                        int cols) {
#pragma HLS INTERFACE m_axi port=img_in_l offset=slave bundle=gmem0
#pragma HLS INTERFACE m_axi port=img_in_r offset=slave bundle=gmem1
#pragma HLS INTERFACE m_axi port=img_out offset=slave bundle=gmem2
#pragma HLS INTERFACE s_axilite port=img_in_l bundle=control
#pragma HLS INTERFACE s_axilite port=img_in_r bundle=control
#pragma HLS INTERFACE s_axilite port=img_out bundle=control
#pragma HLS INTERFACE s_axilite port=penalty_small bundle=control
#pragma HLS INTERFACE s_axilite port=penalty_large bundle=control
#pragma HLS INTERFACE s_axilite port=rows bundle=control
#pragma HLS INTERFACE s_axilite port=cols bundle=control
#pragma HLS INTERFACE s_axilite port=return bundle=control

    xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_L> in_l(rows, cols);
    xf::cv::Mat<IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_R> in_r(rows, cols);
    xf::cv::Mat<OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_OUT> out(rows, cols);

    xf::cv::Ptr2xfMat<INPUT_PTR_WIDTH, IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_L>(img_in_l, in_l);
    xf::cv::Ptr2xfMat<INPUT_PTR_WIDTH, IN_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_R>(img_in_r, in_r);

    xf::cv::SemiGlobalBM<XF_BORDER_CONSTANT, WINDOW_SIZE, TOTAL_DISPARITY, PARALLEL_UNITS, NUM_DIR,
                         IN_TYPE, OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_L,
                         XF_CV_DEPTH_IN_R, XF_CV_DEPTH_OUT>(
        in_l, in_r, out, penalty_small, penalty_large);

    xf::cv::xfMat2Ptr<OUTPUT_PTR_WIDTH, OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_OUT>(out, img_out);
}
```

## Minimal `CMakeLists.txt` sketch

```cmake
cmake_minimum_required(VERSION 3.16)
project(sgbm_minimal LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

add_library(sgbm_kernel
    src/sgbm_top.cpp
)

target_include_directories(sgbm_kernel PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}
)
```

## Notes / caveats

- You still need the **Vitis HLS headers** (`hls_stream.h`, `ap_int.h`, `ap_axi_sdata.h`). These are provided by the Vitis toolchain, not by this repo.
- `WINDOW_SIZE` is fixed to `5` for this kernel.
- Ensure `TOTAL_DISPARITY` is divisible by `PARALLEL_UNITS` and within `[2, 256]`.
- `NUM_DIR` must be `2`, `3`, or `4`.

Use this as a starting point and trim further if you know exactly which utility functions/macros you need.
