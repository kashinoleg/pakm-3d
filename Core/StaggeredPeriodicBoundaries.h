#pragma once

namespace TexGen {
	using namespace std;

	/// Class used to generate Abaqus input file for periodic boundary conditions for a textile with staggered offset. Gives reduced unit cell size
	class CLASS_DECLSPEC CStaggeredPeriodicBoundaries : public CPeriodicBoundaries {
	public:
		CStaggeredPeriodicBoundaries(int NumEdges = 18, int NumVertices = 12);
		~CStaggeredPeriodicBoundaries(void);

		void SetFaceD( vector<int>& D1, vector<int>& D2 );
		void SetOffset( double Offset ) { m_Offset = Offset; }

	protected:
		void OutputFaceSets( ostream& Output );
		/// Output equations for staggered boundary conditions
		void OutputEquations( ostream& Output, int iBoundaryConditions );

		pair< vector<int>, vector<int> > m_FaceD;

		double m_Offset;
	};
}
