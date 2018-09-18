#include "Simulation.h"

using namespace TexGen;

CSimulation::CSimulation(void)
{
}

CSimulation::~CSimulation(void)
{
}

const CLinearTransformation &CSimulation::GetDeformation() const
{
	if (!m_DeformationSteps.empty())
		return m_DeformationSteps.back();
	static CLinearTransformation Identity;
	return Identity;
}

void CSimulation::SetDeformation(const CLinearTransformation &Deformation)
{
	ClearDeformationSteps();
	AddDeformationStep(Deformation);
}

void CSimulation::ClearDeformationSteps()
{
	m_DeformationSteps.clear();
}

void CSimulation::AddDeformationStep(const CLinearTransformation &Deformation)
{
	m_DeformationSteps.push_back(Deformation);
}




