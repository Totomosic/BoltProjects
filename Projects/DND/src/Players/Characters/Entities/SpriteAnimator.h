#pragma once
#include "dndpch.h"

namespace DND
{

	struct AnimationInfo
	{
	public:
		ResourcePtr<Texture2D> Texture;
		int Columns;
		int Rows;
	};

	class SpriteAnimator : public Component
	{
	public:
		struct AnimationData
		{
		public:
			AnimationInfo Animation;
			float RepeatTime;
			float RemainingTime;
		};

	private:
		Mesh* m_Mesh;
		ResourcePtr<Texture2D> m_DefaultTexture;
		AnimationData m_CurrentAnimation;

	public:
		SpriteAnimator();
		SpriteAnimator(const ResourcePtr<Texture2D>& defaultTexture);

		const ResourcePtr<Texture2D>& DefaultTexture() const;
		const AnimationInfo& CurrentAnimation() const;
		bool IsPlayingAnimation() const;
		float RemainingAnimationTime() const;
		bool IsPlayingUntilStopped() const;
		
		void Start() override;
		void Update() override;
		void StopAnimation();

		void SetDefaultTexture(const ResourcePtr<Texture2D>& texture);
		void PlayAnimation(const AnimationInfo& animation, float repeatTime, float animationDuration);
		void PlayAnimation(const AnimationInfo& animation, float repeatTime);
		void PlayAnimationUntilStopped(const AnimationInfo& animation, float repeatTime);

		std::unique_ptr<Component> Clone() const override;

	private:
		void UseDefaultTexture();
		void UseAnimation();

	};

}