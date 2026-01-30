# L1 SGBM minimal example bundle

This bundle contains the **L1 SGBM example** and the minimal set of Vitis Vision headers required to build and run it (including C-simulation). It mirrors the original `vision/L1/examples/sgbm` folder and adds only the `L1/include` headers it depends on.

## Layout

```
L1/examples/sgbm/   # copied example (Makefile, config, testbench, etc.)
L1/include/common/ # minimal common headers used by the example
L1/include/imgproc/xf_sgbm.hpp
```

## Running C-sim

From the `L1/examples/sgbm` directory:

```bash
make run TARGET=csim XPART=<your-part>
```

Notes:
- The testbench uses OpenCV; ensure your environment provides OpenCV headers/libs.
- The example expects left/right grayscale images. Update `xf_sgbm_tb.cpp` or provide appropriate images per the original example flow.
- The Makefile assumes the repo root is two levels above `L1/examples/sgbm` (to set `XF_PROJ_ROOT`). Keep this layout intact.

