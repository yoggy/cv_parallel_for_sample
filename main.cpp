//
//	cv::parallel_for_() sample
//
#ifdef WIN32
	#include <SDKDDKVer.h>
	#include <Windows.h>
	#ifdef _DEBUG
		#pragma comment(lib, "opencv_core246d.lib")
		#pragma comment(lib, "opencv_highgui246d.lib")
	#else
		#pragma comment(lib, "opencv_core246.lib")
		#pragma comment(lib, "opencv_highgui246.lib")
	#endif
#endif

#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

class ConvertColorTest : public cv::ParallelLoopBody
{
    public:
        ConvertColorTest(const cv::Mat &src, cv::Mat &dst)
			: ParallelLoopBody(), src_(src), dst_(dst)
        {
        }

        void operator ()(const cv::Range& range) const
        {
            for (int y = range.start; y < range.end; ++y) {
				const uchar *src_p = src_.ptr<uchar>(y);
				uchar *dst_p       = dst_.ptr<uchar>(y);
				for (int x = 0; x < src_.cols; ++x) {
					int i = x * src_.channels();
					dst_p[i + 1] = src_p[i + 0];
					dst_p[i + 2] = src_p[i + 1];
					dst_p[i + 0] = src_p[i + 2];
				}
            }
        }

private:
        const cv::Mat &src_;
        cv::Mat &dst_;
};

int main(int argc, char* argv[])
{
	cv::Mat src, dst;
	src = cv::imread("nike.jpg", -1);
	if (src.empty()) {
		std::cerr << "cv::imread() failed...filename=nike.jpg" << std::endl;
		return -1;
	}

    dst.create(src.size(), CV_8UC3);

    ConvertColorTest body(src, dst);
    cv::parallel_for_(cv::Range(0, src.rows), body);

    cv::imwrite("result.jpg", dst);
	
	return 0;
}



