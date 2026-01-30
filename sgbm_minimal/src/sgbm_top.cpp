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

    xf::cv::SemiGlobalBM<XF_BORDER_CONSTANT, WINDOW_SIZE, TOTAL_DISPARITY, PARALLEL_UNITS, NUM_DIR, IN_TYPE, OUT_TYPE,
                         HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_IN_L, XF_CV_DEPTH_IN_R, XF_CV_DEPTH_OUT>(
        in_l, in_r, out, penalty_small, penalty_large);

    xf::cv::xfMat2Ptr<OUTPUT_PTR_WIDTH, OUT_TYPE, HEIGHT, WIDTH, NPPCX, XF_CV_DEPTH_OUT>(out, img_out);
}
