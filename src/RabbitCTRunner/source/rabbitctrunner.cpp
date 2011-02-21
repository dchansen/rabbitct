#define RCT_RUNNER_USE_CIMG 0

// PROJECT INCLUDES
// 
#include "rabbitctrunner.h"
#include "rct_timer.h"
#include "rct_module_loader.h"
#include "rabbit_progress.h"
#if RCT_RUNNER_USE_CIMG
    #include "CImg.h"
#endif //RCT_RUNNER_USE_CIMG

Ttime * s_runTimeInUs;


using namespace std;
#if RCT_RUNNER_USE_CIMG
    using namespace cimg_library;
#endif //RCT_RUNNER_USE_CIMG

//////////////////////////////////////////////////////////////


void cleanup(RabbitCtGlobalData& rctgdata, bool projBuffers_managedByModule, bool out_volume_managedByModule)
{
	if (s_runTimeInUs)
	{
		delete [] s_runTimeInUs;
	}

	// if the projection buffer-memory is not managed by the module, allocate it
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
	switch (problemSize)
	{
		case 128:  return 2.0f;
		case 256:  return 1.0f;
		case 512:  return 0.5f;
		case 1024: return 0.25f;
	}
	cerr << "Wrong problem size, check again!" << endl;
	exit(1);
}


int main(int argc, const char* argv[])
{
	cout << "RabbitCT runner http://www.rabbitct.com/" << endl;

	if (argc != 5)
	{
		cerr << "Error, wrong calling syntax: " << endl;
		printHowTo(argv[0]);
		return 1;
	}

	std::string dll_path     = argv[1];
	std::string dataset_path = argv[2];
	std::string result_file  = argv[3];
	const unsigned int problem_size = atoi(argv[4]);

	const unsigned int num_proj_buffers = 496 / 4;
	const float vol_res = volume_resolution(problem_size);

	const int magicSum    = 591984;
	const int magicSumEnd = 489195;

	RabbitCtHeader rctheader;
	RabbitCtGlobalData rctgdata;
	struct timeval Tps, Tpf;

	// now we have all the required data so start processing

	// 1. Load the extension
	if (!loadSharedLibraryFunctions(dll_path.c_str()))
		return 1;

	// now load the dataset
	std::ifstream ifsData(dataset_path.c_str(), ifstream::binary);

	if (!ifsData.is_open() || ifsData.fail())
	{
		cerr << "Could not open dataset file: " << dataset_path.c_str() << endl;
		return 1;
	}


	// read in the header
	ifsData.read((char*)(&rctheader), sizeof(RabbitCtHeader));
	if (ifsData.fail() || !ifsData.good())
	{
		cerr << "Could not read data header, your data may be damaged." << endl;
		return 1;
	}

	// fill the global data for the module
	rctgdata.L   = problem_size;
	rctgdata.R_L = vol_res;

	const unsigned int numel_img = rctheader.img_sz[0] * rctheader.img_sz[1];
	const unsigned int numel_vol = problem_size * problem_size * problem_size;

	rctgdata.f_L = NULL;
	rctgdata.S_x = rctheader.img_sz[0];
	rctgdata.S_y = rctheader.img_sz[1];

	// projection buffers
	rctgdata.adv_numProjBuffers = num_proj_buffers;
	rctgdata.adv_pProjBuffers   = NULL;

	rctgdata.I_n = NULL;
	rctgdata.A_n = NULL;

	bool out_volume_managedByModule  = true;
	bool projBuffers_managedByModule = false;

	rctgdata.O_L = -0.5f * rctgdata.R_L * ( (float)(rctgdata.L) - 1.0f );


	// initialize the module
	if (!s_fncLoadAlgorithm(&rctgdata))
	{
		cerr << "Module returned error at execution of " << RCT_FNCN_LOADALGORITHM << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}

	// if the volume-memory is not managed by the module, allocate it
	out_volume_managedByModule = (NULL != rctgdata.f_L);
	if (NULL == rctgdata.f_L)
	{
		// allocate the required volume
		rctgdata.f_L = new float[numel_vol];
		// set the volume to zero
		memset(rctgdata.f_L, 0, numel_vol * sizeof(float));
	}

	// if the projection buffer-memory is not managed by the module, allocate it
	projBuffers_managedByModule = (NULL != rctgdata.adv_pProjBuffers);
	if (NULL == rctgdata.adv_pProjBuffers)
	{
		rctgdata.adv_pProjBuffers = new float* [num_proj_buffers];
		// allocate the required buffers
		rctgdata.adv_pProjBuffers[0] = new float[num_proj_buffers * numel_img];
		// set it to zero
		memset(rctgdata.adv_pProjBuffers[0], 0, num_proj_buffers * numel_img * sizeof(float));
		for (unsigned int i=1; i<num_proj_buffers; ++i)
			rctgdata.adv_pProjBuffers[i] = rctgdata.adv_pProjBuffers[0] + i * numel_img;
	}

	s_runTimeInUs = new Ttime[rctheader.glb_numImg];

	// allocate pmatrix buffers
	double pProjMatrixBuffers[num_proj_buffers][12];		///< projection matrix buffers, managed by RabbitCT

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
#if ! RABBIT_PROGRESS
		cout << "Caching projections" << endl;
#endif
		for (unsigned int i=0; i<numProjsInIteration; ++i)
		{
			ifsData.read((char*) (pProjMatrixBuffers[i]), 12 * sizeof(double));
			ifsData.read((char*) (rctgdata.adv_pProjBuffers[i]), numel_img * sizeof(float));

			if (ifsData.fail() || !ifsData.good())
			{
				cerr << "Failed to read projection data for image " << (i+1) << endl;
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
#if ! RABBIT_PROGRESS
		cout << "Back-projecting" << endl;
#endif
		for (unsigned int i=0; i<numProjsInIteration; ++i)
		{
			// choose correct projection buffer
			rctgdata.I_n = rctgdata.adv_pProjBuffers[i];
			rctgdata.A_n = pProjMatrixBuffers[i];

			gettimeofday(&Tps, NULL);
			const bool ret = s_fncAlgorithmIteration(&rctgdata);
			gettimeofday(&Tpf, NULL);

			s_runTimeInUs[numProcessedProjs]  = Tpf.tv_sec  - Tps.tv_sec;
			s_runTimeInUs[numProcessedProjs] *= 1000000;
			s_runTimeInUs[numProcessedProjs] += Tpf.tv_usec - Tps.tv_usec;

			//cout << "Backprojection " << (numProcessedProjs+1) << ", Time " << (long double)(s_runTimeInUs[numProcessedProjs]) / 1000.0 << " ms" << endl;

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


	bool ret = s_fncFinishAlgorithm(&rctgdata);

	if (!ret)
	{
		cerr << "Module returned error at execution of " << RCT_FNCN_FINISHALGORITHM << endl;
		cleanup(rctgdata, projBuffers_managedByModule, out_volume_managedByModule);
		return 1;
	}

	// now check the accuraccy
	unsigned short* ref_volume = new unsigned short[numel_vol];
	
	// read in the volumes until we have read the final one
	// this should overcome the tell and seek problems for large files
	for (unsigned int tL=128; tL<2000&&tL<=problem_size; tL*=2)
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


	// check volume and find maximum error
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

	std::string volfile = result_file;
	volfile += ".vol";

	ofstream ofsvol(volfile.c_str(), ofstream::binary);
	ofsvol.write((char*)(rec_volume), numel_vol * sizeof(float));
	ofsvol.close();


	ofstream ofsresult(result_file.c_str(), ofstream::binary);

	ofsresult.write((char*)(&magicSum), sizeof(int));
	ofsresult.write((char*)(&problem_size), sizeof(int));
	ofsresult.write((char*)(&rctheader.glb_numImg), sizeof(int));
	ofsresult.write((char*)(s_runTimeInUs), rctheader.glb_numImg * sizeof(Ttime));
	ofsresult.write((char*)(rec_volume + (problem_size/2 - 1) * problem_size * problem_size), problem_size * problem_size * sizeof(float));
	ofsresult.write((char*)(&error), sizeof(double));
	ofsresult.write((char*)(&maxError), sizeof(double));
	ofsresult.write((char*)(errorHist), NUM_BINS * sizeof(unsigned int));
	ofsresult.write((char*)(&magicSumEnd), sizeof(int));
	ofsresult.close();

	const double psnr = 10.0 * log10(4095.0 * 4095.0 / error);

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

	Ttime time_total = 0;
	long double time_mean = 0.0;

	// now start the processing pipeline
	for (unsigned int i=0; i<rctheader.glb_numImg; ++i)
		time_total += s_runTimeInUs[i];

	time_mean  = (long double) time_total;
	time_mean /= (long double) rctheader.glb_numImg;

	cout << endl;
	cout << "--------------------------------------------------------------" << endl;
	cout << "Runtime statistics:" << endl;
	cout << "Total:   " << ((long double)time_total / (long double)1000000.0) << " s" << endl;
	cout << "Average: " << (time_mean / (long double)1000.0) << " ms" << endl << endl;

#if RCT_RUNNER_USE_CIMG
	displayVolume(rec_volume, problem_size);
#endif //RCT_RUNNER_USE_CIMG
	cleanup(rctgdata,projBuffers_managedByModule,out_volume_managedByModule);

	return 0;
}
