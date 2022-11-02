#include "Vector3.h"
#include <list>
#include <memory>
#include "WorldTransform.h"
#include "Collision.h"



bool RayCollision(WorldTransform ray, WorldTransform obj)
{
    //���C�̓����蔻��

    //���[���h���W����
    Vector3 objPos;
    Vector3 rayPos;
    objPos = obj.position;
    rayPos = ray.position;
#pragma region ���C�̓����蔻��
    //���C�̎n�_�ƏI�_����
    Vector3 rayStart;
    Vector3 rayEnd;
    rayStart = {
        ray.position.x,
        ray.position.y,
        ray.position.z - ray.scale.z - 11.0f,
    };
    rayEnd = {
        ray.position.x,
        ray.position.y,
        ray.position.z + ray.scale.z + 11.0f,
    };
    //�n�_�ƏI�_���烌�C�̃x�N�g��(a��)�����߂�
    Vector3 rayVec;
    rayVec = rayEnd - rayStart;
    float raySize;
    raySize = rayVec.length();
    //���K��(a��N)
    rayVec.normalize();
    //���C�ƃI�u�W�F�N�g�̃x�N�g��(b��)�����߂�
    // ���C�̎n�_�ƃI�u�W�F�N�g�ւ̃x�N�g��(b��)�����߂�
    Vector3 ABVec;
    ABVec = {
        objPos.x - rayStart.x,
        objPos.y - rayStart.y,
        objPos.z - rayStart.z
    };

    //b���Ea��N ��ray2ObjLength�Ɋi�[
    float ray2ObjectLength = ABVec.dot(rayVec);

    //Q�����߂� a��N * b���Ea��N + P
    Vector3 Q;
    //Q = rayVec * ABVec.dot(rayVec) + rayPos;
    Q = rayVec * ray2ObjectLength + rayStart;
    //�I�u�W�F�N�g���烌�C�̐���(obj-Q)�����߂�
    Vector3 line;
    line = objPos - Q;
    //�����̒��������߂�
    line.length();

    //�����̒��������a+���a���Z����Γ������Ă�
    bool isHit = false;
    if (line.length() <= obj.scale.x + 4.3f) {
        if (raySize >= ray2ObjectLength) {
            isHit = true;
        }
    }

    if (ray2ObjectLength <= 0) {
        isHit = false;
    }

    return isHit;

#pragma endregion
}
