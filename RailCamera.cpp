#include "Vector3.h"
#include "RailCamera.h"

void RailCamera::Ini(Vector3 pos,Vector3 rot)
{
	//���[���h�g�����X�t�H�[��������
	worldTransform.position = pos;
	worldTransform.rotation = rot;
	//�r���[�v���W�F�N�V����������
	viewProjection.Ini();
}

void RailCamera::Update()
{
	worldTransform.AddPosition(0, 0, 0);
	worldTransform.AddRotation(0, 0, 0);
	worldTransform.Update(viewProjection);

	viewProjection.eye.x = worldTransform.position.x;
	viewProjection.eye.y = worldTransform.position.y;
	viewProjection.eye.z = worldTransform.position.z;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	forward = worldTransform.rotation;
	//���_����O���ɓK���ȋ����i�񂾈ʒu�����~�X
	viewProjection.target.x = viewProjection.eye.x + forward.x;
	viewProjection.target.y = viewProjection.eye.y + forward.y;
	viewProjection.target.z = viewProjection.eye.z + forward.z;
	//���[���h������x�N�g��
	Vector3 up(0, 1, 0);
	//���[���J�����̉�]�𔽉f�i���[���J�����̏�����x�N�g���j
	viewProjection.up.x = worldTransform.rotation.x;
	viewProjection.up.y = worldTransform.rotation.y;
	viewProjection.up.z = worldTransform.rotation.z;
	//�r���[�v���W�F�N�V�����̍X�V
	viewProjection.Update();
}
