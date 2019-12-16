#pragma once
#include "Component.h"
#include "Mesh.h"
#include "ShaderProgram.h"
#include "Texture.h"
#include "Model.h"
#include "ShaderProgram.h"

class MeshRenderer : public Component
{
private:
	ShaderProgram* m_program;
	Texture* m_texture;
	Model* m_model;

public:
	//Inherited via Component
	MeshRenderer(Model* Model, ShaderProgram * program, Texture* texture);
	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
};