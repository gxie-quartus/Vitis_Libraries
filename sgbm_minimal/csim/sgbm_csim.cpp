#include <cstdlib>
#include <iostream>
#include <vector>

#include "ap_int.h"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"

#include "config/xf_config_params.h"

void semiglobalbm_accel(ap_uint<INPUT_PTR_WIDTH>* img_in_l,
                        ap_uint<INPUT_PTR_WIDTH>* img_in_r,
                        unsigned char penalty_small,
                        unsigned char penalty_large,
                        ap_uint<OUTPUT_PTR_WIDTH>* img_out,
                        int rows,
                        int cols);

namespace {
void usage(const char* argv0) {
    std::cerr << "Usage: " << argv0
              << " <left_image> <right_image> <output_image> [penalty_small] [penalty_large]\n";
}
} // namespace

int main(int argc, char** argv) {
    if (argc < 4) {
        usage(argv[0]);
        return 1;
    }

    const char* left_path = argv[1];
    const char* right_path = argv[2];
    const char* out_path = argv[3];
    unsigned char penalty_small = (argc > 4) ? static_cast<unsigned char>(std::atoi(argv[4])) : SMALL_PENALTY;
    unsigned char penalty_large = (argc > 5) ? static_cast<unsigned char>(std::atoi(argv[5])) : LARGE_PENALTY;

    cv::Mat left = cv::imread(left_path, cv::IMREAD_GRAYSCALE);
    cv::Mat right = cv::imread(right_path, cv::IMREAD_GRAYSCALE);

    if (left.empty() || right.empty()) {
        std::cerr << "Failed to load input images.\n";
        return 1;
    }
    if (left.size() != right.size()) {
        std::cerr << "Input images must have the same dimensions.\n";
        return 1;
    }
    if (left.rows > HEIGHT || left.cols > WIDTH) {
        std::cerr << "Input dimensions exceed configured HEIGHT/WIDTH (" << HEIGHT << "x" << WIDTH << ").\n";
        return 1;
    }

    const int rows = left.rows;
    const int cols = left.cols;
    const size_t count = static_cast<size_t>(rows) * static_cast<size_t>(cols);

    std::vector<ap_uint<INPUT_PTR_WIDTH> > in_left(count);
    std::vector<ap_uint<INPUT_PTR_WIDTH> > in_right(count);
    std::vector<ap_uint<OUTPUT_PTR_WIDTH> > out(count);

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const unsigned char lval = left.at<unsigned char>(r, c);
            const unsigned char rval = right.at<unsigned char>(r, c);
            in_left[r * cols + c] = lval;
            in_right[r * cols + c] = rval;
        }
    }

    semiglobalbm_accel(in_left.data(), in_right.data(), penalty_small, penalty_large, out.data(), rows, cols);

    cv::Mat disparity(rows, cols, CV_8UC1);
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            disparity.at<unsigned char>(r, c) = static_cast<unsigned char>(out[r * cols + c]);
        }
    }

    if (!cv::imwrite(out_path, disparity)) {
        std::cerr << "Failed to write output image.\n";
        return 1;
    }

    std::cout << "Wrote disparity image to " << out_path << "\n";
    return 0;
}
