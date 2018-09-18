#pragma once

namespace TexGen {
	using namespace std;

	/// Class used to generate Abaqus input file for periodic boundary conditions for a textile with sheared domain. Note that equations are only valid for xy shear 
	class CLASS_DECLSPEC CShearedPeriodicBoundaries : public CPeriodicBoundaries {
	public:
		CShearedPeriodicBoundaries(void);
		~CShearedPeriodicBoundaries(void);

		/// Set up the domain size parameters used in the equations
		void SetDomainSize( const CMesh& Mesh );

	private:
		/// Output equations for sheared domain boundary conditions
		void OutputEquations( ostream& Output, int iBoundaryConditions );
		double m_Domyx;
		double m_Domyy;
	};
}
