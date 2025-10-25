#pragma once
#include <glm/glm.hpp>

class Metaballs {
public:
    Metaballs() { for (int i = 0; i < 10; i++) radius[i] = 5.0f; }

    glm::vec3 positions[10] = {};
    glm::vec3 velocities[10] = {};
    float radius[10];

    float m_BoxL = 100.f;
    float m_BoxW = 100.f;
    float m_BoxH = 100.f;
    
    int m_Counts = 0;

    void OnUpdate(float time);

    void SetBoxDimension(float length)
    {
        m_BoxL = length; m_BoxW = length, m_BoxH = length;
    }
    void SetBoxDimension(float length, float width, float height)
    {
        m_BoxL = length; m_BoxW = width, m_BoxH = height;
    }

    float GetBoxLength() const { return m_BoxL; }
    float GetBoxWidth() const { return m_BoxW; }
    float GetBoxHeight() const { return m_BoxH; }

    void SetCounts(int c) { m_Counts = c; }
    int GetCounts() const { return m_Counts; }

private:

};
