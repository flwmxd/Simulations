#pragma once

#include "BoneForMotionData.h"
#include <string>
#include <fstream>
#include <iostream>

#define Xposition 1
#define Yposition 2
#define Zposition 3
#define Zrotation 4
#define Xrotation 5
#define Yrotation 6


class BVHParser
{
public:
	BVHParser(const char * BVHFilePath);
	void setHierarchy();
	~BVHParser();
	void readBoneMotion(double * frameTime);
	std::ifstream fileReader;
	Bone * root;
	Bone* settingBoneRelation(std::ifstream& fileReader);
	void setBoneVAOs(Bone * root);
	int boneIDCounter = 0;
	int * VISITED;
	float * motion;
	int totalChannels = 0;
	void setFrame(Bone * root, int frameIndex);
	void setMotion(Bone * root, int frameIndex);
	void clearVISITED();
	int Frames;
	void resetMatrices(Bone * root);
	int motionIndex = 0;
};
