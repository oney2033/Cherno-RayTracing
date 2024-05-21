#pragma once

#include <glm/glm.hpp>
#include <vector>

//��ס����Ҽ��ƶ������ס����Ҽ� �ٰ�w a s d q e 
#pragma once

#include <glm/glm.hpp>
#include <vector>

class Camera
{
public:
    // ���캯������ʼ����ֱ��Ұ�����ü����Զ�ü���
    Camera(float verticalFOV, float nearClip, float farClip);

    // ���������״̬
    bool OnUpdate(float ts);
    // �����ӿڴ�С�仯
    void OnResize(uint32_t width, uint32_t height);

    // ��ȡͶӰ����
    const glm::mat4& GetProjection() const { return m_Projection; }
    // ��ȡͶӰ����������
    const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
    // ��ȡ��ͼ����
    const glm::mat4& GetView() const { return m_View; }
    // ��ȡ��ͼ����������
    const glm::mat4& GetInverseView() const { return m_InverseView; }

    // ��ȡ�������λ��
    const glm::vec3& GetPosition() const { return m_Position; }
    // ��ȡ�������ǰ����
    const glm::vec3& GetDirection() const { return m_ForwardDirection; }

    // ��ȡԤ����Ĺ��߷���
    const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

    // ��ȡ���������ת�ٶ�
    float GetRotationSpeed();

private:
    // ���¼���ͶӰ����
    void RecalculateProjection();
    // ���¼�����ͼ����
    void RecalculateView();
    // ���¼�����߷���
    void RecalculateRayDirections();

private:
    // ͶӰ���󣬳�ʼֵΪ��λ����
    glm::mat4 m_Projection{ 1.0f };
    // ��ͼ���󣬳�ʼֵΪ��λ����
    glm::mat4 m_View{ 1.0f };
    // ͶӰ���������󣬳�ʼֵΪ��λ����
    glm::mat4 m_InverseProjection{ 1.0f };
    // ��ͼ���������󣬳�ʼֵΪ��λ����
    glm::mat4 m_InverseView{ 1.0f };

    // ��ֱ��Ұ��Ĭ��ֵΪ45��
    float m_VerticalFOV = 45.0f;
    // ���ü��棬Ĭ��ֵΪ0.1
    float m_NearClip = 0.1f;
    // Զ�ü��棬Ĭ��ֵΪ100
    float m_FarClip = 100.0f;

    // �������λ�ã���ʼֵΪԭ��
    glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
    // �������ǰ���򣬳�ʼֵΪ(0, 0, 0)
    glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

    // ����Ĺ��߷���
    std::vector<glm::vec3> m_RayDirections;

    // ��һ�����λ�ã���ʼֵΪ(0, 0)
    glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

    // �ӿڵĿ�Ⱥ͸߶ȣ���ʼֵΪ0
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};
