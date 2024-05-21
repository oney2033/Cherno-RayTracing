#pragma once

#include <glm/glm.hpp>
#include <vector>

//按住鼠标右键移动相机或按住鼠标右键 再按w a s d q e 
#pragma once

#include <glm/glm.hpp>
#include <vector>

class Camera
{
public:
    // 构造函数，初始化垂直视野、近裁剪面和远裁剪面
    Camera(float verticalFOV, float nearClip, float farClip);

    // 更新摄像机状态
    bool OnUpdate(float ts);
    // 处理视口大小变化
    void OnResize(uint32_t width, uint32_t height);

    // 获取投影矩阵
    const glm::mat4& GetProjection() const { return m_Projection; }
    // 获取投影矩阵的逆矩阵
    const glm::mat4& GetInverseProjection() const { return m_InverseProjection; }
    // 获取视图矩阵
    const glm::mat4& GetView() const { return m_View; }
    // 获取视图矩阵的逆矩阵
    const glm::mat4& GetInverseView() const { return m_InverseView; }

    // 获取摄像机的位置
    const glm::vec3& GetPosition() const { return m_Position; }
    // 获取摄像机的前向方向
    const glm::vec3& GetDirection() const { return m_ForwardDirection; }

    // 获取预计算的光线方向
    const std::vector<glm::vec3>& GetRayDirections() const { return m_RayDirections; }

    // 获取摄像机的旋转速度
    float GetRotationSpeed();

private:
    // 重新计算投影矩阵
    void RecalculateProjection();
    // 重新计算视图矩阵
    void RecalculateView();
    // 重新计算光线方向
    void RecalculateRayDirections();

private:
    // 投影矩阵，初始值为单位矩阵
    glm::mat4 m_Projection{ 1.0f };
    // 视图矩阵，初始值为单位矩阵
    glm::mat4 m_View{ 1.0f };
    // 投影矩阵的逆矩阵，初始值为单位矩阵
    glm::mat4 m_InverseProjection{ 1.0f };
    // 视图矩阵的逆矩阵，初始值为单位矩阵
    glm::mat4 m_InverseView{ 1.0f };

    // 垂直视野，默认值为45度
    float m_VerticalFOV = 45.0f;
    // 近裁剪面，默认值为0.1
    float m_NearClip = 0.1f;
    // 远裁剪面，默认值为100
    float m_FarClip = 100.0f;

    // 摄像机的位置，初始值为原点
    glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
    // 摄像机的前向方向，初始值为(0, 0, 0)
    glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

    // 缓存的光线方向
    std::vector<glm::vec3> m_RayDirections;

    // 上一次鼠标位置，初始值为(0, 0)
    glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };

    // 视口的宽度和高度，初始值为0
    uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};
