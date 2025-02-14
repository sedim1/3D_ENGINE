#pragma once
#include"BONE.h"
#include"MODEL.h"
#include<vector>
#include<string>
#include<map>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

struct AssimpNodeData
{
	glm::mat4 transformation;
	std::string name;
	int childrenCount;
	std::vector<AssimpNodeData> children;
};

class Animation
{
	private:
		float mDuration;
		float mTicksPerSecond;
		std::vector<Bone> mBones;
		AssimpNodeData mRootNode;
		std::map<std::string,BoneInfo> mBoneInfoMap;
		void ReadMissingBones(const aiAnimation* animation, Model& model)
		{
			int size = animation->mNumChannels;
			auto& boneInfoMap = model.GetBoneInfoMap();
			int& boneCount = model.GetBoneCount();
			//Reading channels(bones enaged in an animation and their keyframes)
			for(int i = 0; i < size; i++)
			{
				auto channel = animation->mChannels[i];
				std::string boneName = channel->mNodeName.data;
				if(boneInfoMap.find(boneName)==boneInfoMap.end())
				{
					boneInfoMap[boneName].id = boneCount;
					boneCount++;
				}
				mBones.push_back(Bone(channel->mNodeName.data,boneInfoMap[channel->mNodeName.data].id,channel));
			}
			mBoneInfoMap = boneInfoMap;
		}
		void ReadHierarchyData(AssimpNodeData& dest, const aiNode* src)
		{
			assert(src);
			dest.name = src->mName.data;
			dest.transformation = ConvertMatrixToGLMFormat(src->mTransformation);
			dest.childrenCount = src->mNumChildren;
			for(int i = 0; i < src->mNumChildren; i++)
			{
				AssimpNodeData newData;
				ReadHierarchyData(newData,src->mChildren[i]);
				dest.children.push_back(newData);
			}
		}
	public:
		Animation() = default;
		Animation(const std::string& animationPath, Model* model)
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
			assert(scene && scene->mRootNode);
			auto animation = scene->mAnimations[0];
			mDuration = animation->mDuration;
			mTicksPerSecond = animation->mTicksPerSecond;
			ReadHierarchyData(mRootNode,scene->mRootNode);
			ReadMissingBones(animation,*model);
		}

		Bone* FindBone(const std::string& name)
		{
			auto iter = std::find_if(mBones.begin(),mBones.end(),
					[&](const Bone& Bone)
					{
						return Bone.GetBoneName()==name;
					});
			if(iter==mBones.end()) return nullptr;
			else return &(*iter);
		}

		inline float GetTicksPerSecond() {return mTicksPerSecond;}
		inline float GetDuration() {return mDuration;}
		inline const AssimpNodeData& GetRootNode() {return mRootNode;}
		inline const std::map<std::string,BoneInfo>& GetBoneIDMap(){return mBoneInfoMap;}
		~Animation(){}
};

class Animator{
	public:
		Animator(Animation* Animation)
		{
			mCurrentTime - 0.0f;
			mCurrentAnimation = Animation;
			mFinalBoneMatrices.reserve(100);
			for(int i = 0; i < 100; i++)
				mFinalBoneMatrices.push_back(glm::mat4(1.0f));
		}
		void UpdateAnimation(float dt)
		{
			mDeltaTime = dt;
			if(mCurrentAnimation)
			{
				mCurrentTime += mCurrentAnimation->GetTicksPerSecond() * dt;
				mCurrentTime = fmod(mCurrentTime,mCurrentAnimation->GetDuration());
				CalculateBoneTransform(&mCurrentAnimation->GetRootNode(),glm::mat4(1.0f));
			}
		}
		void PlayAnimation(Animation* pAnimation)
		{
			mCurrentAnimation = pAnimation;
			mCurrentTime = 0.0f;
		}
		void CalculateBoneTransform(const AssimpNodeData* node,glm::mat4 parentTransform)
		{
			std::string nodeName = node->name;
			glm::mat4 nodeTransform = node->transformation;
			Bone* Bone = mCurrentAnimation->FindBone(nodeName);
			if(Bone!=NULL)
			{
				Bone->Update(mCurrentTime);
				nodeTransform = Bone->GetLocalTransform();
			}
			glm::mat4 globalTransformation = parentTransform * nodeTransform;
			auto boneInfoMap = mCurrentAnimation->GetBoneIDMap();
			if(boneInfoMap.find(nodeName) != boneInfoMap.end())
			{
				int index = boneInfoMap[nodeName].id;
				glm::mat4 offset = boneInfoMap[nodeName].offset;
				mFinalBoneMatrices[index]=globalTransformation*offset;
			}
			for(int i = 0;i < node->childrenCount; i++)
				CalculateBoneTransform(&node->children[i],globalTransformation);
		}

		std::vector<glm::mat4> GetFinalBoneMatrices()
		{
			return mFinalBoneMatrices;
		}
	private:
		std::vector<glm::mat4> mFinalBoneMatrices;
		Animation* mCurrentAnimation;
		float mCurrentTime;
		float mDeltaTime;
};
