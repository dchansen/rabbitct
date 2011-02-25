#define RCT_RUNNER_USE_CIMG 0

// PROJECT INCLUDES
// 
#include "rabbitctrunner.h" // ISA
#include "rct_timer.h"
#include "rct_module_loader.h"
#include "rabbit_progress.h"
#if RCT_RUNNER_USE_CIMG
    #include "CImg.h"
#endif //RCT_RUNNER_USE_CIMG

Ttime* s_runTimeInUs;
Ttime  s_runTimePrepareInUs;
bool s_module_has_prepare_function = false;


using namespace std;
#if RCT_RUNNER_USE_CIMG
    using namespace cimg_library;
#endif //RCT_RUNNER_USE_CIMG

//////////////////////////////////////////////////////////////


void cleanup(RabbitCtGlobalData& rctgdata, bool projBuffers_managedByModule, bool out_volume_managedByModule)
{
	// free timer variables
	if (s_runTimeInUs)
	{
		delete [] s_runTimeInUs;
	}

	// free projection matrix buffers
	if (NULL != rctgdata.adv_ppProjMatrixBuffers) {
		for (unsigned int i=1; i<rctgdata.adv_N; ++i) {
			rctgdata.adv_ppProjMatrixBuffers[i] = NULL;
		}

		// delete the actual buffer
		delete[] rctgdata.adv_ppProjMatrixBuffers[0];
		rctgdata.adv_ppProjMatrixBuffers[0] = NULL;

		// delete the pointers to the individual buffers
		if (NULL != rctgdata.adv_ppProjMatrixBuffers) {
			delete[] rctgdata.adv_ppProjMatrixBuffers;
			rctgdata.adv_ppProjMatrixBuffers = NULL;
		}
	}

	// if the projection buffer-memory is not managed by the module, free it
	if (!projBuffers_managedByModule)
	{
		// delete the previously allocated buffers
		for (unsigned int i=1; i<rctgdata.adv_numProjBuffers; ++i)
		{
			// clear buffer ptr
			rctgdata.adv_pProjBuffers[i] = NULL;
		}
		// delete buffer
		delete [] (rctgdata.adv_pProjBuffers[0]);
		// delete buffer ptrs
		delete [] (rctgdata.adv_pProjBuffers);
		rctgdata.adv_pProjBuffers = NULL;
	}

	// if the volume-memory is not managed by the module, free it
	if (!out_volume_managedByModule)
	{
		// delete the previously allocated volume
		delete [] (rctgdata.f_L);
		rctgdata.f_L = NULL;
	}

	s_fncUnloadAlgorithm(&rctgdata);
	releaseSharedLibrary();
}


void printHowTo(const char * pname)
{
	cout << "Usage: " << pname << " [Algorithm Library]" << " [Dataset File]" << " [Result File]" << " [ProblemSize: 128|256|512|1024]" << endl;
}


#if RCT_RUNNER_USE_CIMG
void displayVolume(const float* const rec_volume, const unsigned int problem_size)
{
	CImg<float> tvol(rec_volume, problem_size, problem_size, problem_size, true);
	tvol.display();
}
#endif //RCT_RUNNER_USE_CIMG

float volume_resolution(const unsigned int problemSize)
{
	float rv = 0.0f;
	switch (problemSize)
	{
		case 128:  rv = 2.0f;  break;
		case 256:  rv = 1.0f;  break;
		case 512:  rv = 0.5f;  break;
		case 1024: rv = 0.25f; break;
		default: cerr << "Wrong problem size, check again!" << endl; exit(1); break;
	}
	return rv;
}


////////////////////////////////////////////////////////////////
// BEGIN Load projection matrices
bool loadProjectionMatrices(RabbitCtHeader* p_rctheader, std::ifstream& ifsData, double* const pProjMatrixBuffers) {
	const std::streamoff oldStreamPos = ifsData.tellg();

	const unsigned int numel_img = p_rctheader->img_sz[0] * p_rctheader->img_sz[1];

	const unsigned int img_size = numel_img * sizeof(float);
	const unsigned int file_limit = (unsigned int) 1 << 31;
	float* const tmpProjBuf = new float[ numel_img ];

	for (unsigned int i=0; i<p_rctheader->glb_numImg; ++i)
	{
		const unsigned int cur_proj = i; // absolute projection number

		// read proj mx
		double* const pProjMx = pProjMatrixBuffers + cur_proj * 12;
		ifsData.read((char*) (pProjMx), 12 * sizeof(double));
		//ifsData.read((char*) (pProjMatrixBuffers[cur_proj]), 12 * sizeof(double));

#if 0
		// read image data
		ifsData.read((char*) (m_rctGData.adv_pProjBuffers[i]), numel_img * sizeof(float));
#else
		// skip image data
		unsigned long long pos_before = ifsData.tellg();
		unsigned long long seekTo = pos_before + img_size;

		// seekg does not work beyond 2GB, read does
		// Therefore, use read if we exceeded the file size limit 
		if (seekTo < file_limit) {
			ifsData.seekg(img_size, ios::cur); // skip actual reading of projection images
		} else {
			ifsData.read((char*) tmpProjBuf, numel_img * sizeof(float));
		}
#endif

		if (ifsData.fail() || !ifsData.good()) {
			cerr << "Failed to read projection matrix for image " << (cur_proj+1) << endl;
			delete[] tmpProjBuf;
			return false;
		}
	}

	// reset input stream position
	ifsData.seekg(oldStreamPos);

	delete[] tmpProjBuf;

	return true;
}
// END Load projection matrices
////////////////////////////////////////////////////////////////


int main(int argc, const char* argv[])
{
	cout << "RabbitCTRunner http://www.rabbitct.com/" << endl;

	if (argc != 5)
	{
		cerr << "Error, wrong calling syntax: " << endl;
		printHowTo(argv[0]);
		return 1;
	}

	std::string module_file  = argv[1];
	std::string dataset_file = argv[2];
	std::string result_file  = argv[3];
	const unsigned int problem_size = atoi(argv[4]);

	const float vol_res = volume_resolution(problem_size);

	cout << "Module: " << module_file << endl;
	cout << "Dataset: " << dataset_file << endl;
	cout << "ProblemSize: " << problem_size << endl;

	// random numbers
	const int magicNum    = 591984;
	const int magicNumEnd = 489195;

	RabbitCtHeader rctheader;
	RabbitCtGlobalData rctgdata;
	struct timeval Tps, Tpf;

	// now we have all the required data so start processing


////////////////////////////////////////////////////////////////
// BEGIN LOAD_MODULE
	if (!loadSharedLibraryFunctions(module_file.c_str())) {
		return 1;
	}
// END LOAD_MODULE
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN OPEN_DATASET
//       now load the dataset
	std::ifstream ifsData(dataset_file.c_str(), ifstream::binary);

	if (!ifsData.is_open() || ifsData.fail())
	{
		cerr << "Could not open dataset file: " << dataset_file.c_str() << endl;
		return 1;
	}
// END OPEN_DATASET
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN READ_HEADER
//       read in the data set header
	ifsData.read((char*)(&rctheader), sizeof(RabbitCtHeader));
	if (ifsData.fail() || !ifsData.good())
	{
		cerr << "Could not read data header, your data may be damaged." << endl;
		return 1;
	}
// END READ_HEADER
////////////////////////////////////////////////////////////////

	s_runTimeInUs = new Ttime[rctheader.glb_numImg];

	const unsigned int num_projections = rctheader.glb_numImg;
	unsigned int num_proj_buffer_subsets = 4; // load N_P / n projections within one buffering iteration; 4 is a save value even for large volumes
	unsigned int num_proj_buffers = num_projections / num_proj_buffer_subsets; // 496 / 4 = 124

	const unsigned int numel_img = rctheader.img_sz[0] * rctheader.img_sz[1];
	const unsigned int numel_vol = problem_size * problem_size * problem_size;

	bool out_volume_managedByModule  = true;
	bool projBuffers_managedByModule = false;

////////////////////////////////////////////////////////////////
// BEGIN INIT_RCGD_DATA
//       fill the global data for the module
	memset(&rctgdata, 0, sizeof(rctgdata));
	rctgdata.L   = problem_size;
	rctgdata.R_L = vol_res;
	rctgdata.adv_N = rctheader.glb_numImg;

	rctgdata.f_L = NULL;
	rctgdata.S_x = rctheader.img_sz[0];
	rctgdata.S_y = rctheader.img_sz[1];

	// projection buffers
	rctgdata.adv_numProjBuffers = num_proj_buffers;
	rctgdata.adv_pProjBuffers   = NULL;

	rctgdata.I_n = NULL;
	rctgdata.A_n = NULL;

	rctgdata.O_L = -0.5f * rctgdata.R_L * ( (float)(rctgdata.L) - 1.0f );
// END INIT_RCGD_DATA
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN ALLOC_MXMEM
//       allocate proj-matrix memory
	double** pProjMatrixBuffers = new double* [rctheader.glb_numImg];	///< projection matrix buffers, managed by RabbitCT
	pProjMatrixBuffers[0] = new double[rctheader.glb_numImg * 12];	///< projection matrix buffers, managed by RabbitCT
	for (unsigned int i=1; i<rctheader.glb_numImg; ++i) {
		pProjMatrixBuffers[i] = pProjMatrixBuffers[0] + i * 12;
	}
	rctgdata.adv_ppProjMatrixBuffers = pProjMatrixBuffers;
// END ALLOC_MXMEM
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN LOAD_MX
//       load projection matrices
//       @depends READ_HEADER, ALLOC_MXMEM
	if (false == loadProjectionMatrices(&rctheader, ifsData, (double*) pProjMatrixBuffers[0])) {
		cerr << "Error pre-loading projection matrices." << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}
// END LOAD_MX
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN PREPARE_ALGORITHM
//       call module function prepareAlgorithm
//       @depends LOAD_MX
	if (0 != s_fncPrepareAlgorithm) {
		s_module_has_prepare_function = true;
		gettimeofday(&Tps, NULL);
		const bool ret = s_fncPrepareAlgorithm(&rctgdata); // PREPAREALGORITHM
		gettimeofday(&Tpf, NULL);

		if (!ret)
		{
			cerr << "Module returned error at execution of " << RCT_FNCN_PREPAREALGORITHM << endl;
			cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
			return 1;
		}

		s_runTimePrepareInUs  = Tpf.tv_sec  - Tps.tv_sec;
		s_runTimePrepareInUs *= 1000000;
		s_runTimePrepareInUs += Tpf.tv_usec - Tps.tv_usec;
	} else {
		s_module_has_prepare_function = false;
		s_runTimePrepareInUs = 0;
	}
// END PREPARE_ALGORITHM
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN LOAD_MODULE
//       initialize the module
	if (!s_fncLoadAlgorithm(&rctgdata)) // LOADALGORITHM
	{
		cerr << "Module returned error at execution of " << RCT_FNCN_LOADALGORITHM << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}
// END LOAD_MODULE
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN ALLOC_VOLMEM
//       if the volume-memory is not managed by the module, allocate it
//       @depends LOAD_MODULE
	out_volume_managedByModule = (NULL != rctgdata.f_L);
	if (!out_volume_managedByModule)
	{
		// allocate the required volume
		rctgdata.f_L = new float[numel_vol];
		if (NULL == rctgdata.f_L) {
			cerr << "Error: Failed to allocate volume memory." << endl;
		}
		// set the volume to zero
		memset(rctgdata.f_L, 0, numel_vol * sizeof(float));
	}
// END ALLOC_VOLMEM
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// BEGIN ALLOC_PROJMEM
//       if the projection buffer-memory is not managed by the module, allocate it
//       @depends LOAD_MODULE
	projBuffers_managedByModule = (NULL != rctgdata.adv_pProjBuffers);
	if (!projBuffers_managedByModule)
	{
		// allocate a temp buffer of 50 MB to reserve some memory for the rest of the program
		// and for the reference volume
		const unsigned int buf_size = numel_vol * sizeof(unsigned short) + 50 * 1024*1024;
		char* buf = new char[buf_size];

		if (1024 == problem_size) {
			// if the 1024^3 volume is used we assume to also have enough RAM to buffer all projections
			num_proj_buffers = rctheader.glb_numImg;
		}
		num_proj_buffer_subsets = (unsigned int) ceil((float) rctheader.glb_numImg / (float) num_proj_buffers);
		cout << "Info: using " << num_proj_buffer_subsets << " buffer subsets with " << num_proj_buffers << " projections each." << endl;

		// update data structure
		rctgdata.adv_numProjBuffers = num_proj_buffers;

		//cout << "Using " << m_numProjBuffers << " projection buffers." << endl;
		rctgdata.adv_pProjBuffers = new float* [num_proj_buffers];
		rctgdata.adv_pProjBuffers[0] = new float[num_proj_buffers * numel_img];

		// set it to zero
		memset(rctgdata.adv_pProjBuffers[0], 0, num_proj_buffers * numel_img * sizeof(float));
		for (unsigned int i=1; i<num_proj_buffers; ++i) {
			rctgdata.adv_pProjBuffers[i] = rctgdata.adv_pProjBuffers[0] + i * numel_img;
		}

		// delete temp buffer
		delete[] buf;
	}
// END ALLOC_PROJMEM
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN BP_LOOP
//       actual backprojection loop
#if RABBIT_PROGRESS
	RabbitProgress progress_bar(rctheader.glb_numImg, problem_size);
#endif
	// now start the processing pipeline
	cout << "Running ... this may take some time." << endl;
	unsigned int numProcessedProjs = 0;
	unsigned int numRemainingProjs = rctheader.glb_numImg;
	while (numRemainingProjs > 0)
	{
		const unsigned int numProjsInIteration = min(numRemainingProjs, num_proj_buffers);

		// load numProjsInIteration projections and proj matrices
#ifdef _DEBUG
		cout << "Buffering projections" << endl;
#endif // _DEBUG
		for (unsigned int i=0; i<numProjsInIteration; ++i)
		{
			const unsigned int cur_proj = numProcessedProjs + i; // absolute projection number

			// read proj mx
			ifsData.read((char*) (pProjMatrixBuffers[cur_proj]), 12 * sizeof(double));
			// read image data
			ifsData.read((char*) (rctgdata.adv_pProjBuffers[i]), numel_img * sizeof(float));

			if (ifsData.fail() || !ifsData.good())
			{
				cerr << "Failed to read projection data for view " << (cur_proj+1) << endl;
				cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
				return 1;
			}
#if ! RABBIT_PROGRESS
			cout << "*";
			if ((i+1) % 62 == 0)
				cout << endl;
#endif
		} // caching loop
#if ! RABBIT_PROGRESS
		cout << endl;
#endif

		// back-project
#if (! RABBIT_PROGRESS) || defined(_DEBUG)
		cout << "Back-projecting" << endl;
#endif
		for (unsigned int i=0; i<numProjsInIteration; ++i)
		{
			const unsigned int cur_proj = numProcessedProjs; // absolute projection number

			// choose correct projection buffer
			rctgdata.I_n = rctgdata.adv_pProjBuffers[i];
			rctgdata.A_n = pProjMatrixBuffers[cur_proj];

			// advanced variables
			rctgdata.adv_projNumber        = cur_proj;
			rctgdata.adv_numAvailProjs     = numProjsInIteration;
			rctgdata.adv_firstProjOfSubset = (0 == i);

			gettimeofday(&Tps, NULL);
			const bool ret = s_fncAlgorithmIteration(&rctgdata); // BACKPROJECT
			gettimeofday(&Tpf, NULL);

			s_runTimeInUs[cur_proj]  = Tpf.tv_sec  - Tps.tv_sec;
			s_runTimeInUs[cur_proj] *= 1000000;
			s_runTimeInUs[cur_proj] += Tpf.tv_usec - Tps.tv_usec;

			//cout << "Backprojection " << (cur_proj+1) << ", Time " << (long double)(s_runTimeInUs[cur_proj]) / 1000.0 << " ms" << endl;

			if (!ret)
			{
				cerr << "Module returned error at execution of " << RCT_FNCN_ALGORITHMBACKPROJ << endl;
				cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
				return 1;
			}
			++numProcessedProjs;
#if ! RABBIT_PROGRESS
			cout << ".";
			if ((i+1) % 62 == 0)
				cout << endl;
#else
			progress_bar.progress(numProcessedProjs);
#endif
		} // BP loop
#if ! RABBIT_PROGRESS
		cout << endl;
#endif
		numRemainingProjs -= numProjsInIteration;
	} // while
// END BP_LOOP
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN FINISH_ALGORITHM
//       call module function finishAlgorithm
//       @depends BP_LOOP
	bool ret = s_fncFinishAlgorithm(&rctgdata);
	if (!ret)
	{
		cerr << "Module returned error at execution of " << RCT_FNCN_FINISHALGORITHM << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}
// END FINISH_ALGORITHM
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN LOAD_REF
//       load reference volume
	unsigned short* const ref_volume = new unsigned short[numel_vol];
	
	// read in the volumes until we have read the final one
	// this should overcome the tell and seek problems for large files
	for (unsigned int tL=128; tL<=1024 && tL<=problem_size; tL*=2)
	{
		ifsData.read((char*) ref_volume, tL*tL*tL*sizeof(unsigned short));
	}

	if (ifsData.fail())
	{
		cerr << "Failed to read reference dataset." << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}

	ifsData.close();
// END LOAD_REF
////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////
// BEGIN CALC_ERR
//       check volume and find maximum error
	float* rec_volume = rctgdata.f_L;
	float* HUS = rctheader.glb_HUScale;

	double error = 0.0;
	double maxError = 0.0;
	const int NUM_BINS = 11;
	// 0-1, 1-2, 2-3, 3-4,  ... 9-10, >10
	unsigned int errorHist[NUM_BINS];
	memset(errorHist, 0, NUM_BINS*sizeof(unsigned int));

	for (unsigned int i=0; i<numel_vol; ++i)
	{
		float HU_ax = rec_volume[i] > 0.0f ? US_ROUND( (HUS[0] * rec_volume[i] + HUS[1]) ) : 0.0f;
		if (HU_ax > 4095.0f) HU_ax = 4095.0f;
		const float cerr = (HU_ax - (float)(ref_volume[i]));
		const float absErr = fabs(cerr);
		maxError = max( maxError, (double) absErr );
		error += cerr * cerr;
		rec_volume[i] = HU_ax;
		unsigned int histIdx = (unsigned int) floor(absErr); // truncate
		if (histIdx > NUM_BINS-1) histIdx = NUM_BINS-1;
		++errorHist[histIdx];
	}
	error /= (double)numel_vol;

	const double psnr = 10.0 * log10(4095.0 * 4095.0 / error);
// END CALC_ERR
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN WRITE_RES_VOL
//       write reconstructed volume, HU-scaled, to disk
//       @depends CALC_ERR
	std::string volfile = result_file;
	volfile += ".vol";

	ofstream ofsvol(volfile.c_str(), ofstream::binary);
	ofsvol.write((char*)(rec_volume), numel_vol * sizeof(float));
	ofsvol.close();
// END WRITE_RES_VOL
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN WRITE_RES_DATA
//       write timing and stuff to disk (.rctd file)
//       @depends CALC_ERR
	ofstream ofsresult(result_file.c_str(), ofstream::binary);

	ofsresult.write((char*)(&magicNum), sizeof(int));
	ofsresult.write((char*)(&problem_size), sizeof(int));
	ofsresult.write((char*)(&rctheader.glb_numImg), sizeof(int));
	ofsresult.write((char*)(s_runTimeInUs), rctheader.glb_numImg * sizeof(Ttime));
	ofsresult.write((char*)(rec_volume + (problem_size/2 - 1) * problem_size * problem_size), problem_size * problem_size * sizeof(float));
	ofsresult.write((char*)(&error), sizeof(double));
	ofsresult.write((char*)(&maxError), sizeof(double));
	ofsresult.write((char*)(errorHist), NUM_BINS * sizeof(unsigned int));
	ofsresult.write((char*)(&magicNumEnd), sizeof(int));
	ofsresult.close();
// END WRITE_RES_DATA
////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////
// BEGIN CALC_AVG_TIME
//       @depends BP_LOOP
	Ttime time_total = 0;
	for (unsigned int i=0; i<rctheader.glb_numImg; ++i) {
		time_total += s_runTimeInUs[i];
	}
	const long double time_mean  = (long double) time_total / (long double) rctheader.glb_numImg;
// END CALC_AVG_TIME
////////////////////////////////////////////////////////////////

	
////////////////////////////////////////////////////////////////
// BEGIN OUTPUT_METRICS
//       print results on screen
//       @depends CALC_ERR, CALC_AVG_TIME
	cout << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "Quality of reconstructed volume:" << endl;
	cout << "Root Mean Squared Error: " << sqrt(error) << " HU" << endl;
	cout << "Mean Squared Error: " << error << " HU^2" << endl;
	cout << "Max. Absolute Error: " << maxError << " HU" << endl;
	
	if (error > 1e-8)
		cout << "PSNR: " << psnr << " dB" << endl;
	else
		cout << "PSNR: Inf dB" << endl;

	cout << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "Runtime statistics:" << endl;
	cout << "Total:   " << ((long double)time_total / (long double)1000000.0) << " s" << endl;
	cout << "Average: " << (time_mean / (long double)1000.0) << " ms" << endl << endl;
	if (s_module_has_prepare_function) {
		cout << "(Prepare): " << ((long double)s_runTimePrepareInUs / (long double)1000000.0) << " s (" << ((long double)s_runTimePrepareInUs / (long double)1000.0) << " ms)" << endl;
	}
// END OUTPUT_METRICS
////////////////////////////////////////////////////////////////


#if RCT_RUNNER_USE_CIMG
	displayVolume(rec_volume, problem_size);
#endif //RCT_RUNNER_USE_CIMG


////////////////////////////////////////////////////////////////
// BEGIN CLEANUP
	cleanup(rctgdata,projBuffers_managedByModule,out_volume_managedByModule);
// END CLEANUP
////////////////////////////////////////////////////////////////

	return 0;
}
