#include "Metaballs.h"

void Metaballs::OnUpdate(float time)
{
	for (int i = 0; i < m_Counts; i++)
	{
		positions[i] += velocities[i] * time;

		if (abs(positions[i].x) + radius[i] > m_BoxL / 2)
		{
			velocities[i].x = -1 * velocities[i].x;
			positions[i].x = positions[i].x > 0 ? m_BoxL / 2 - radius[i] : -m_BoxL / 2 + radius[i];
		}
		if (abs(positions[i].y) + radius[i] > m_BoxW / 2)
		{
			velocities[i].y = -1 * velocities[i].y;
			positions[i].y = positions[i].y > 0 ? m_BoxW / 2 - radius[i] : -m_BoxW / 2 + radius[i];
		}
		if (abs(positions[i].z) + radius[i] > m_BoxH / 2)
		{
			velocities[i].z = -1 * velocities[i].z;
			positions[i].z = positions[i].z > 0 ? m_BoxH / 2 - radius[i] : -m_BoxH / 2 + radius[i];
		}
	}
}