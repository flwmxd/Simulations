#include "Recorder.h"
#include "opengl/OGL.h"

Recorder::Recorder(const std::string& name, const std::string& codec, int32_t frameRate, int32_t width, int32_t height)
	:width(width),height(height)
{
	video.open(name, 
		cv::VideoWriter::fourcc(codec[0], codec[1], codec[2], codec[3]), 
		frameRate, cv::Size(width, height),true);
	video.set(cv::VIDEOWRITER_PROP_QUALITY, 100.0);

	colorsCV = cv::Mat(width, height, CV_8UC3);
	data.resize(width * height * 3);
}

Recorder::~Recorder()
{

}

auto Recorder::end() -> void
{
	video.release();
}

auto Recorder::getFrame() -> void
{
	glReadPixels(0, 0, width, height, GL_BGR, GL_UNSIGNED_BYTE, data.data());
	cv::Mat tmp(height,width, CV_8UC3, data.data());
	cv::flip(tmp, tmp, 0);
	video.write(tmp);
}
