#pragma once
#include <glm/gtc/type_ptr.hpp>
#include <cstring>
#include <iostream>
const int MAX = 3;

class Bone {
private:
	char * boneName;
	int boneID;
	glm::mat4 R;
	glm::mat4 T;
	Bone * childBone[MAX];
	int nChildren = 0;
	int nChannel = 0;
public:
	float x, y, z;	//THIS Should be translated for intial T
	int * channelOrder;
	unsigned int * VAOs;
	//unsigned int * VBOs;	//Is it okay if I delete ths VBO? Test it later. Should be in private:.
	Bone() {
		this->R = glm::mat4(1.0f);
		this->T = glm::mat4(1.0f);
	}
	void setName(const char * boneName) {
		size_t length = strlen(boneName) + 1;
		this->boneName = new char[length];
		strcpy(this->boneName, boneName);
	}

	void setnChannel(int nChannel) {
		this->nChannel = nChannel;
	}

	void setID(int boneIDCounter) {
		boneID = boneIDCounter;
	}

	int returnnChannel() {
		return nChannel;
	}

	void setVAOs();

	void setChild(Bone * child) {
		childBone[nChildren] = child;
		nChildren++;
	}

	void setR(glm::mat4 R) {
		this->R = R;
	}
	void setT(glm::mat4 T) {
		this->T = T;
	}
	void setT() {
		glm::mat4 temp = glm::mat4(1.0f);
		this->T = glm::translate(temp,glm::vec3(x,y,z));
	}
	glm::mat4 returnR() {
		return R;
	}
	glm::mat4 returnT() {
		return T;
	}
	const char * returnBoneName() {
		return boneName;
	}
	int returnBoneID() {
		return boneID;
	}
	int returnnChildren() {
		return nChildren;
	}
	Bone * i_thChild(int index) {
		return childBone[index];
	}
	unsigned int returnVAOs(int i) {
		if (i >= nChildren) {
			std::cout << "ERROR. THAT VAO DOES NOT EXIST" << std::endl;
			return NULL;
		}
		else {
			return VAOs[i];
		}
	}
};

