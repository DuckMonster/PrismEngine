#pragma once
class PR_IProgram {
public:
	virtual void Init( ) = 0;
	virtual void Update( double delta ) = 0;
	virtual void Render( double delta ) = 0;
};