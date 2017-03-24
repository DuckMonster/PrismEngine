#pragma once
#include <Prism/Resource/TextureResource.h>

class PR_IPostEffect {
public:
	virtual PR_CTextureResource* ApplyTo( PR_CTextureResource* texture ) = 0;
};