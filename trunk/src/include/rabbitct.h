#ifndef __LME_RABBITCT__H
#define __LME_RABBITCT__H 1

#include <iostream>

/** \brief RabbitCT global data structure.

	This is the main structure describing the relevant dataset descriptions.
	The notation is adapted to the MedicalPhysics Technical Note.
*/
struct RabbitCtGlobalData
{
	///@{ Relevant data for the backprojection
	unsigned int	L;					///< problem size	\in {128, 256, 512, 1024}
	unsigned int	S_x;				///< projection image width
	unsigned int	S_y;				///< projection image height (detector rows)
	double *		A_n;				///< 3x4 projetion matrix
	float *			I_n;				///< projection image buffer
	float			R_L;				///< isotropic voxel size
	float			O_L;				///< position of the 0-index in the world coordinate system
	float *			f_L;				///< pointer to where the result volume should be stored, by default managed by module.
	///@}

	///@{ Relevant data for projection image memory management. Only required for advanced usage.
	unsigned int	adv_numProjBuffers;	///< number of projection buffers in RAM
	float **		adv_pProjBuffers;	///< projection image buffers, by default managed by RabbitCT
	///@}

	///@{ Some more variables for advanced usage.
	unsigned int	adv_N;                   ///< total number of projections
	unsigned int	adv_projNumber;          ///< current projection number [0..495]
	double**        adv_ppProjMatrixBuffers; ///< all N projection matrices
	unsigned int	adv_numAvailProjs;       ///< number of available (RAM-buffered in adv_pProjBuffers) projections (can be lower than adv_numProjBuffers)
	unsigned int	adv_firstProjOfSubset;   ///< is this the first projection of a RAM-buffer-subset?
	///@}

	friend std::ostream& operator<<(std::ostream& out, const RabbitCtGlobalData& r) {
		out << "L:   " << r.L   << "    S_x: " << r.S_x << "    S_y: " << r.S_y << std::endl;
		out << "A_n: " << r.A_n << "    I_n: " << r.I_n << std::endl;
		out << "R_L: " << r.R_L << "    O_L: " << r.O_L << "    f_L: " << r.f_L << std::endl;
		out << "adv_numProjBuffers:      " << r.adv_numProjBuffers << std::endl;
		out << "adv_pProjBuffers:        " << r.adv_pProjBuffers << std::endl;
		out << "adv_ppProjMatrixBuffers: " << r.adv_ppProjMatrixBuffers << std::endl;
		out << "adv_N:                   " << r.adv_N << std::endl;
		out << "adv_firstProjOfSubset:   " << r.adv_firstProjOfSubset << std::endl;
		out << "adv_projNumber:          " << r.adv_projNumber << std::endl;
		out << "adv_numAvailProjs:       " << r.adv_numAvailProjs /* no final endl */;
		return out;
	}
	friend std::ostream& operator<<(std::ostream& out, const RabbitCtGlobalData* pr) {
		return out << (*pr);
	}
};



// the source files need to be compiled as a dynamically loadable shared library.
// In windows this is called DLL (dynamic link library) and in the linux world
// a shared library. For using this feature a special prefix is required for the
// function definitions. On windows systems it is given by
//  	extern "C"__declspec(dllexport) 
// and on linux systems by
//	extern "C" 
#ifdef WIN32
#define FNCSIGN	extern "C"__declspec(dllexport) 
#elif WIN64
#define FNCSIGN	extern "C"__declspec(dllexport) 
#else
#define FNCSIGN	extern "C" 
#endif


#define RCT_FNCN_PREPAREALGORITHM		"RCTPrepareAlgorithm"
#define RCT_FNCN_LOADALGORITHM			"RCTLoadAlgorithm"
#define RCT_FNCN_ALGORITHMBACKPROJ		"RCTAlgorithmBackprojection"
#define RCT_FNCN_FINISHALGORITHM		"RCTFinishAlgorithm"
#define RCT_FNCN_UNLOADALGORITHM		"RCTUnloadAlgorithm"


#endif
