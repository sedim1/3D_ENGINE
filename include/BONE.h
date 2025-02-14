#pragma once
#include<glm/glm.hpp>
#include<vector>
#include<string>
#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include<assimp/Importer.hpp>
#include<assimp/scene.h>
#include<assimp/Importer.hpp>

struct KeyPosition{
	glm::vec3 position;
	float timeStamp;
};

struct KeyRotation{
	glm::quat orientation;
	float timeStamp;
};

struct KeyScale
{
	glm::vec3 scale;
	float timeStamp;
};

class Bone{
	private:
		std::vector<KeyPosition> m_Positions;
		std::vector<KeyRotation> m_Rotations;
		std::vector<KeyScale> m_Scales;
		int m_NumPositions;
		int m_NumRotations;
		int m_NumScales;
		glm::mat4 m_LocalTransform;
		std::string m_Name;
		int m_ID;
		//Gets normalized value for lerp and slerp
		float GetScaleFactor(float lastTimeStamp,float nextTimeStamp,float animationTime)
		{
			float scaleFactor = 0.0f;
			float midWayLenght = animationTime - lastTimeStamp;
			float framesDiff = nextTimeStamp - lastTimeStamp;
			scaleFactor = midWayLenght/framesDiff;
			return scaleFactor;
		}
		//Figures out wich positions key to interpolate between and performs the lerp to return the translation matrix
		glm::mat4 InterpolatePosition(float animationTime)
		{
			if(1 == m_NumPositions)
				return glm::translate(glm::mat4(1.0f),m_Positions[0].position);
			int p0Index = GetPositionIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Positions[p0Index].timeStamp,m_Positions[p1Index].timeStamp,animationTime);
			glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].position,m_Positions[p1Index].position, scaleFactor);
			return glm::translate(glm::mat4(1.0f),finalPosition);
		}
		//Same with the final rotation matrix
		glm::mat4 InterpolateRotation(float animationTime)
		{
			if(1==m_NumRotations)
			{
				auto rotation = glm::normalize(m_Rotations[0].orientation);
				return glm::toMat4(rotation);
			}
			int p0Index = GetRotationIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Rotations[p0Index].timeStamp,m_Rotations[p1Index].timeStamp,animationTime);
			glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].orientation,m_Rotations[p1Index].orientation, scaleFactor);
			finalRotation = glm::normalize(finalRotation);
			return glm::toMat4(finalRotation);
		}
		//Same with scaling matrix
		glm::mat4 InterpolateScaling(float animationTime)
		{
			if(1== m_NumScales)
				return glm::scale(glm::mat4(1.0f),m_Scales[0].scale);
			int p0Index = GetScaleIndex(animationTime);
			int p1Index = p0Index + 1;
			float scaleFactor = GetScaleFactor(m_Scales[p0Index].timeStamp,m_Scales[p1Index].timeStamp,animationTime);
			glm::vec3 finalScale = glm::mix(m_Scales[p0Index].scale,m_Scales[p1Index].scale, scaleFactor);
			return glm::scale(glm::mat4(1.0f),finalScale);
		}
	public:
		//Read keyframes
		Bone(const std::string& name, int ID, const aiNodeAnim* channel)
			:
				m_Name(name),
				m_ID(ID),
				m_LocalTransform(1.0)
		{
			m_NumPositions = channel->mNumPositionKeys;
			//Read position data
			for(int positionIndex = 0; positionIndex < m_NumPositions; positionIndex++)
			{
				aiVector3D aiPosition = channel->mPositionKeys[positionIndex].mValue;
				float timeStamp = channel->mPositionKeys[positionIndex].mTime;
				KeyPosition data;
				data.position = glm::vec3(aiPosition.x,aiPosition.y,aiPosition.z);
				data.timeStamp = timeStamp;
				m_Positions.push_back(data);
			}
			//Read rotation data
			m_NumRotations = channel->mNumRotationKeys;
			for(int rotationIndex = 0; rotationIndex < m_NumRotations; rotationIndex++)
			{
				aiQuaternion aiOrientation = channel->mRotationKeys[rotationIndex].mValue;
				float timeStamp = channel->mRotationKeys[rotationIndex].mTime;
				KeyRotation data;
				data.orientation = glm::quat(aiOrientation.w,aiOrientation.x,aiOrientation.y,aiOrientation.z);
				data.timeStamp = timeStamp;
				m_Rotations.push_back(data);
			}
			//Read scaling data
			m_NumScales = channel->mNumScalingKeys;
			for(int keyIndex = 0; keyIndex < m_NumScales; keyIndex++)
			{
				aiVector3D scale = channel->mScalingKeys[keyIndex].mValue;
				float timeStamp = channel->mScalingKeys[keyIndex].mTime;
				KeyScale data;
				data.scale = glm::vec3(scale.x,scale.y,scale.z);
				data.timeStamp = timeStamp;
				m_Scales.push_back(data);
			}
		}
		//Interpolate rotations, positions and scaling based on the time
		void Update(float animationTime)
		{
			glm::mat4 translation = InterpolatePosition(animationTime);
			glm::mat4 rotation = InterpolateRotation(animationTime);
			glm::mat4 scale = InterpolateScaling(animationTime);
			m_LocalTransform = translation * rotation * scale;
		}
		glm::mat4 GetLocalTransform(){return m_LocalTransform;}
		std::string GetBoneName() const {return m_Name;}
		int GetBoneID(){return m_ID;}
		//To interpolate based on the current animation ttime on position, scaling and rotations
		int GetPositionIndex(float animationTime)
		{
			for(int index = 0; index < m_NumPositions-1;++index)
			{
				if(animationTime < m_Positions[index+1].timeStamp)
					return index;
			}
			assert(0);
		}
		int GetRotationIndex(float animationTime)
		{
			for(int index = 0; index < m_NumRotations-1;++index)
			{
				if(animationTime < m_Rotations[index+1].timeStamp)
					return index;
			}
			assert(0);
		}
		int GetScaleIndex(float animationTime)
		{
			for(int index = 0; index < m_NumScales-1;++index)
			{
				if(animationTime < m_Scales[index+1].timeStamp)
					return index;
			}
			assert(0);
		}
};
