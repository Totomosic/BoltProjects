#pragma once
#include "SpriteAnimator.h"

namespace DND
{

	enum class Direction
	{
		Up,
		Down,
		Left,
		Right,
		MAX_DIRECTIONS
	};

	struct CharacterTextureSet
	{
	public:
		ResourcePtr<Texture2D> UpTexture;
		ResourcePtr<Texture2D> DownTexture;
		ResourcePtr<Texture2D> LeftTexture;
		ResourcePtr<Texture2D> RightTexture;

	};

	class CharacterAnimator : public Component
	{
	private:
		SpriteAnimator* m_Animator;
		CharacterTextureSet m_Textures;
		Direction m_CurrentDirection;

	public:
		CharacterAnimator(Direction currentDirection, const CharacterTextureSet& textures);

		const CharacterTextureSet& Textures() const;
		Direction CurrentDirection() const;
		const ResourcePtr<Texture2D>& CurrentTexture() const;
		const ResourcePtr<Texture2D>& GetTextureByDirection(Direction direction) const;

		void Start() override;
		void SetCurrentDirection(Direction direction);

		std::unique_ptr<Component> Clone() const override;

	};

}