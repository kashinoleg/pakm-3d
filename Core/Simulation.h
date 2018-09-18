#pragma once
#include "TextileDeformerVolumeMesh.h"

namespace TexGen {
	using namespace std;

	/// Represents a textile cell containing yarns
	class CLASS_DECLSPEC CSimulation {
	public:
		CSimulation(void);
		virtual ~CSimulation(void);

		/// Clear all deformation steps
		void ClearDeformationSteps();

		/// Add a deformation step
		void AddDeformationStep(const CLinearTransformation &Deformation);

		/// Get the list of deformation steps
		const vector<CLinearTransformation> &GetDeformationSteps() const { return m_DeformationSteps; }

		/// Set the list of deformation steps
		void SetDeformationSteps(const vector<CLinearTransformation> &DeformationSteps) { m_DeformationSteps = DeformationSteps; }

		/// Return the final deformation in the series or identity if none exist
		/**
		This function is useful if the simulation consists of a single deformation
		step otherwise use GetDeformationSteps.
		*/
		const CLinearTransformation &GetDeformation() const;

		/// Set the deformation, this will clear all existing deformations
		/**
		This function is useful if the simulation consists of a single deformation
		step otherwise use AddDeformationStep or SetDeformationSteps.
		*/
		void SetDeformation(const CLinearTransformation &Deformation);

	protected:
		vector<CLinearTransformation> m_DeformationSteps;
	};
}
