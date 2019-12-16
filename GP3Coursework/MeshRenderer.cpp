#include "pch.h"
#include "MeshRenderer.h"
#include "Entity.h"
#include "Common.h"
#include "Application.h"
#include "Camera.h"

MeshRenderer::MeshRenderer(Model* model, ShaderProgram* program, Texture* texture)
{
	m_model = model;
	m_program = program;
	m_texture = texture;
}

void MeshRenderer::OnUpdate(float deltaTime)
{
	//LOG_DEBUG("I'm updating!");
}

void MeshRenderer::OnRender()
{
	m_program->Use();

	//set uniforms here!
	glm::mat4 model = m_entity->GetTransform()->GetTransformationMatrix();
	glm::mat4 mvp = Application::GetInstance()->GetCamera()->Get() * model;

	GLuint loc = glGetUniformLocation(m_program->Get(), "MVP");
	glUniformMatrix4fv(loc, 1, false, (const GLfloat*)glm::value_ptr(mvp));

	glm::vec3 oColor = glm::vec3(1.f, 1.f, 1.f);
	loc = glGetUniformLocation(m_program->Get(), "objectColor");
	glUniform3f(loc, oColor.x, oColor.y, oColor.z);

	glm::vec3 lColor = glm::vec3(1.f, 0.f, 0.f);
	loc = glGetUniformLocation(m_program->Get(), "lightColor");
	glUniform3f(loc, lColor.x, lColor.y, lColor.z);

	m_texture->Bind();

	for (Mesh* mesh : m_model->GetMeshes())
	{
		mesh->Bind();

		if (m_texture) m_texture->Bind();

		GL_ATTEMPT(glDrawElements(GL_TRIANGLES, mesh->GetIndicesCount(), GL_UNSIGNED_INT, 0));
	}
}