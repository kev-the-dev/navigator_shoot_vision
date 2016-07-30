#include "FrameProc.h"

const int FrameProc::blur_kernel_size = 3;
const int FrameProc::erode_kernel_size = 3;
const int FrameProc::dilate_kernel_size = 3;

FrameProc::FrameProc()
{

	cap = VideoCapture(0);

	erode_element = getStructuringElement(MORPH_RECT,Size(2*erode_kernel_size + 1,2*erode_kernel_size+1), Point(erode_kernel_size,erode_kernel_size)) ;

	dilate_element = getStructuringElement(MORPH_RECT,Size( 2* dilate_kernel_size + 1, 2* dilate_kernel_size+1 ), Point(dilate_kernel_size, dilate_kernel_size));
	red = ColorThresh{cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255)};
	blue = ColorThresh{Scalar(0,0,0),Scalar(255,255,255)};
	green = ColorThresh{Scalar(0,0,0),Scalar(255,255,255)};
	rgb_frame = Mat();
	hsv_frame = Mat();
	binary_blue_frame = Mat();
	binary_red_frame = Mat();
	binary_green_frame = Mat();
	
	namedWindow("blured",CV_WINDOW_AUTOSIZE);
	namedWindow("hsv",CV_WINDOW_AUTOSIZE);
}
void FrameProc::GetFrame()
{
	if (!cap.read(rgb_frame))
	{
		std::cerr << "Could not read frame" << std::endl;
	}

}
void FrameProc::ErodeDilate()
{
	erode(rgb_frame,rgb_frame,erode_element);
	dilate(rgb_frame,rgb_frame,dilate_element);

}
void FrameProc::ConvertHSV()
{
	cvtColor(rgb_frame,hsv_frame,CV_BGR2HSV);

}
void FrameProc::ThresholdColors()
{
	inRange(hsv_frame,red.low,red.high,binary_red_frame);
	inRange(hsv_frame,blue.low,blue.high,binary_blue_frame);
	inRange(hsv_frame,green.low,green.high,binary_green_frame);
}
void FrameProc::Prepare(Mat frame)
{
	rgb_frame = frame;
	ErodeDilate();
	ConvertHSV();
	ThresholdColors();

}
void FrameProc::Prepare()
{
	GetFrame();
	DebugWindow::UpdateColor(rgb_frame);
	ErodeDilate();
	ConvertHSV();
	ThresholdColors();
	
	imshow("blured",rgb_frame);
	imshow("hsv",hsv_frame);
	DebugWindow::UpdateRed(binary_red_frame);
	DebugWindow::UpdateBlue(binary_blue_frame);
	DebugWindow::UpdateGreen(binary_green_frame);
}
Mat FrameProc::GetRed()
{
	return binary_red_frame;
}
Mat FrameProc::GetBlue()
{
	return binary_blue_frame;
}
Mat FrameProc::GetGreen()
{
	return binary_green_frame;
}
