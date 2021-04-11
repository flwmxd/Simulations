#pragma once

#include <opencv2/opencv.hpp>
#include <string>
#include <iostream>

class Recorder
{
public:
private:
	cv::VideoWriter video;
	int32_t width;
	int32_t height;
	cv::Mat colorsCV;
public:
	Recorder(const std::string & name, const std::string& codec, int32_t frameRate, int32_t width, int32_t height);
	~Recorder();

	auto end() -> void;
	auto getFrame() -> void;
	std::vector<uint8_t> data;
};