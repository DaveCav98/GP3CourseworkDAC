#include "pch.h"
#include "RigidBody.h"
#include "Physics.h"
#include "Entity.h"

void RigidBody::OnUpdate(float deltaTime)
{

}

void RigidBody::OnRender()
{

}

void RigidBody::Init(CollisionShape* shape, float mass, const glm::vec3 localIntertia)
{
	btTransform bT = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());
	m_mState = new btDefaultMotionState(bT);
	m_shape = shape;
	btVector3 li = btVector3(localIntertia.x, localIntertia.y, localIntertia.z);
	m_shape->Get()->calculateLocalInertia(mass, li);
	m_rigidBody = new btRigidBody(btScalar(mass), m_mState, m_shape->Get(), li);
	Physics::GetInstance()->AddRigidbody(this);
	m_rigidBody->setSleepingThresholds(0, 0);
	m_rigidBody->setFriction(1);
}

void RigidBody::UpdateParent()
{
	//TODO: Get the transform from the rigidbody's motionnstate and apply it to the game objects transform
	//m_rigidBody->setWorldTransform(Physics::ConvertTransformToBtTransform((*m_entity->GetTransform())));
	//m_entity->AddComponent(m_rigidBody->getWorldTransform);

	/*btTransform btTrans;
	m_rigidBody->getMotionState()->getWorldTransform(btTrans);
	btVector3 btPos = btTrans.getOrigin();
	btQuaternion btQuat = btTrans.getRotation();
	btVector3 btRot = btQuat.getAxis();
	m_entity->GetTransform()->SetPosition(glm::vec3(btPos.x(), btPos.y(), btPos.z()));
	m_entity->GetTransform()->SetRotation(glm::vec3(btRot.x(), btRot.y(), btRot.z()));*/

	btTransform bt;
	m_rigidBody->getMotionState()->getWorldTransform(bt);

	Physics::ConvertBtTransformToTransform(bt, m_entity->GetTransform());

}

void RigidBody::UpdateRigidBody()
{
	btTransform t = Physics::ConvertTransformToBtTransform(*m_entity->GetTransform());

	m_rigidBody->setWorldTransform(t);
	m_rigidBody->getMotionState()->setWorldTransform(t);
}