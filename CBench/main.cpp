/*================================================================================
This software is open source software available under the BSD-3 license.

Copyright (c) 2017, Los Alamos National Security, LLC.
All rights reserved.

Authors:
 - Pascal Grosset
 - Jesus Pulido
 - Chris Biwer
==========================================================================*/
#include <iostream>
#include <sstream>
#include <vector>
#include <time.h>
#include <stdlib.h>

#include <mpi.h>

// Helper Functions
#include "json.hpp"
#include "timer.hpp"
#include "log.hpp"
#include "memory.hpp"
#include "utils.hpp"

// Interfaces
#include "dataLoaderInterface.hpp"
#include "compressorInterface.hpp"
#include "metricInterface.hpp"

// Factories
#include "compressorFactory.hpp"
#include "metricFactory.hpp"
#include "dataLoaderFactory.hpp"


// Global log
std::stringstream debugLog;


int main(int argc, char *argv[])
{
	//
	// init MPI
	int myRank, numRanks, threadSupport;
	MPI_Init_thread(NULL, NULL, MPI_THREAD_MULTIPLE, &threadSupport);
	MPI_Comm_size(MPI_COMM_WORLD, &numRanks);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);


	//
	// Validate input params
	if (!validateInput(argc, argv, myRank, numRanks))
	{
		MPI_Finalize();
		return 0;
	}



	//
	// Load input


	//
	// Pass JSON file to json parser and
	nlohmann::json jsonInput;
	std::ifstream jsonFile(argv[1]);
	jsonFile >> jsonInput;


	//
	// Load in the global input parameters


	// file timesteps
	std::string inputFilename = "";
	int minTimestep = 0;
	int maxTimestep = 1;
	std::vector<std::string> filenameTs;
	if (jsonInput["input"].contains("timesteps"))	// files in order
	{
		minTimestep = jsonInput["input"]["timesteps"][0];  	// inclusive
		maxTimestep = jsonInput["input"]["timesteps"][1];	// exclusive

		inputFilename = jsonInput["input"]["filename"];
	}
	else if (jsonInput["input"].contains("filename-timesteps"))	// arbitrary file names
		{
			maxTimestep = jsonInput["input"]["filename-timesteps"].size();
			for (int i=0; i<maxTimestep; i++)
				filenameTs.push_back( jsonInput["input"]["filename-timesteps"][i] );
		}
		else
			inputFilename = jsonInput["input"]["filename"];	// single timestep

	int numTimesteps = maxTimestep - minTimestep;


	// write out decompressed files + output name
	bool writeData = false;
	std::string outputFilename = "";
	if (jsonInput["data-reduction"]["cbench-output"].contains("output-decompressed"))
	{
		writeData = jsonInput["data-reduction"]["cbench-output"]["output-decompressed"];
	
		// Initial a random number in case output name is not provided
		srand(time(NULL));
	}


	// location of decompressed files
	std::string outputPath = ".";
	if ( jsonInput["data-reduction"]["cbench-output"].contains("output-decompressed-location"))
	{
		outputPath = jsonInput["data-reduction"]["cbench-output"]["output-decompressed-location"];
		if (myRank == 0)
			createFolder(outputPath);
	}


	// Log file name
	createFolder("logs");
	std::string outputLogFilename = "logs/" + jsonInput["data-reduction"]["cbench-output"]["log-file"].get<std::string>() + "_" + std::to_string(myRank);


	// Store compressors, scalars, and metrics to use
	std::vector<std::string> compressors;
	for (int i = 0; i < jsonInput["data-reduction"]["cbench-compressors"].size(); i++)
		compressors.push_back(jsonInput["data-reduction"]["cbench-compressors"][i]["name"]);

	std::vector<std::string> scalars;
	for (int i = 0; i < jsonInput["input"]["scalars"].size(); i++)
		scalars.push_back(jsonInput["input"]["scalars"][i]);

	std::vector<std::string> metrics;
	for (int i = 0; i < jsonInput["data-reduction"]["cbench-metrics"].size(); i++)
		metrics.push_back(jsonInput["data-reduction"]["cbench-metrics"][i]["name"]);


	//
	// For humans; all seems valid, let's start ...
	if (myRank == 0)
		std::cout << "Starting ... \nLook at the log for progress update ... \n" << std::endl;



	//
	// Create log and metrics files
	Timer clock;
	std::stringstream metricsInfo, csvOutputHeader;

	//overallClock.start();
	clock.start("overall");

	
	for (int ts=minTimestep; ts<maxTimestep; ts++)
	{
		//
		// Create header for metrics output
		std::stringstream csvOutput;
		csvOutput << "Compressor_field" << "__" << "params" << ", " << "name, ";
		for (int m = 0; m < metrics.size(); ++m)
			csvOutput << metrics[m] << ", ";
		csvOutput << "Compression Throughput(MB/s), DeCompression Throughput(MB/s), Compression Ratio" << std::endl;

		debugLog << "\n***************************************************** timestep: " << ts << " of " << numTimesteps << std::endl;


		//
		// Open data file
		std::string fileToLoad;
		DataLoaderInterface *ioMgr;
		{
			//
			// Get filename
			fileToLoad = inputFilename;
			if (numTimesteps > 1)
				if (filenameTs.size() > 0)
					fileToLoad = filenameTs[ts];
				else
				{
					std::string tempStr = inputFilename;
					fileToLoad = tempStr.replace( tempStr.find("%"), tempStr.find("%")+1, strConvert::toStr(ts) );
				}
				
			metricsInfo << "Input file: " << fileToLoad << std::endl;
			if (myRank == 0)
				std::cout << "\nReading " << fileToLoad << std::endl;
			

			//
			// Create Data Loader
			std::string inputFileType = jsonInput["input"]["filetype"];
			ioMgr = DataLoaderFactory::createLoader(inputFileType);

			if (ioMgr == NULL)
			{
				if (myRank == 0)
					std::cout << "Unsupported loader: " << inputFileType << " ... exiting!" << std::endl;
				
				debugLog << "Unsupported loader: " << inputFileType << " ... exiting!"<< std::endl;
				writeLog(outputLogFilename, debugLog.str());

				MPI_Finalize();
				return 0;
			}
			else if (inputFileType == "NYX")
			{
				if (jsonInput["input"].contains("group"))
					ioMgr->setParam("group", "string", jsonInput["input"]["group"]);
			}


			//
			// Check if the datainfo field exist for a dataset
			if (jsonInput["input"].contains("datainfo"))
			{
				// insert datainfo into loader parameter list
				for (auto it = jsonInput["input"]["datainfo"].begin(); it != jsonInput["input"]["datainfo"].end(); it++)
					ioMgr->loaderParams[it.key()] = strConvert::toStr(it.value());
			}

			ioMgr->init(fileToLoad, MPI_COMM_WORLD);
			ioMgr->setTimestep(ts);
			ioMgr->setSave(writeData);


			// Save parameters of input file to facilitate rewrite
			if (writeData)
				ioMgr->saveInputFileParameters();
		}

		writeLog(outputLogFilename, debugLog.str());



		// TODO: Compressor inner loop and scalar outer loop - only load data once//
		
		//
		// Cycle through compressors
		CompressorInterface *compressorMgr;
		for (int c = 0; c < compressors.size(); ++c)
		{
			// initialize compressor
			compressorMgr = CompressorFactory::createCompressor(compressors[c]);
			if (compressorMgr == NULL)
			{
				if (myRank == 0)
					std::cout << "Unsupported compressor: " << compressors[c] << " ... skipping!" << std::endl;

				debugLog << "Unsupported compressor: " << compressors[c] << " ... skipping!" << std::endl;
				continue;
			}


			// initialize compressor
			compressorMgr->init();


			// Apply parameter if same for all scalars, else delay for later
			bool sameCompressorParams = true;
			if (jsonInput["data-reduction"]["cbench-compressors"][c].find("compressor-params") != jsonInput["data-reduction"]["cbench-compressors"][c].end())
			{
				sameCompressorParams = false;
				//std::cout << "sameCompressorParams = false" << std::endl;
			}
			else
			{
				for (auto it = jsonInput["data-reduction"]["cbench-compressors"][c].begin(); it != jsonInput["data-reduction"]["cbench-compressors"][c].end(); ++it)
					if ((it.key() != "name") && (it.key() != "output-prefix"))
						compressorMgr->compressorParameters[it.key()] = strConvert::toStr(it.value());
				
				//std::cout << "sameCompressorParams = true" << std::endl;
			}


			// log
			metricsInfo << "\n---------------------------------------" << std::endl;
			metricsInfo << "Compressor: " << compressorMgr->getCompressorName() << std::endl;

			debugLog << "===============================================" << std::endl;
			debugLog << "Compressor: " << compressorMgr->getCompressorName() << std::endl;


			//
			// Cycle through scalars
			//std::cout << "\n"<< std::endl;
			for (int i = 0; i < scalars.size(); i++)
			{
				//Timer compressClock, decompressClock;
				Memory memLoad(true);

				// Check if parameter is valid before proceding
				if (!ioMgr->loadData(scalars[i]))
				{
					memLoad.stop();
					std::cout << "ioMgr->loadData(" << scalars[i] << ") failed!" << std::endl; 
					debugLog << "ioMgr->loadData(" << scalars[i] << ") failed!" << std::endl; 
					continue;
				}


				// Read in compressor parameter for this field
				bool scalarFound = false;
				if (!sameCompressorParams)
				{
				
					compressorMgr->compressorParameters.clear();  // reset compression param for each field
					int numdifferentParams = jsonInput["data-reduction"]["cbench-compressors"][c]["compressor-params"].size();

					for (int cp = 0; cp < numdifferentParams; cp++)
					{
						for (auto it = jsonInput["data-reduction"]["cbench-compressors"][c]["compressor-params"][cp]["scalar"].begin();
								it != jsonInput["data-reduction"]["cbench-compressors"][c]["compressor-params"][cp]["scalar"].end(); it++)
						{
							if (*it != scalars[i])
								continue;

							scalarFound = true;
							for (auto itt = jsonInput["data-reduction"]["cbench-compressors"][c]["compressor-params"][cp].begin();
									itt != jsonInput["data-reduction"]["cbench-compressors"][c]["compressor-params"][cp].end(); ++itt)
								if (itt.key() != "scalar")
									compressorMgr->compressorParameters[itt.key()] = strConvert::toStr(itt.value());
						}
					}
				}

				//std::cout << "Scalar: " << i << ": " << scalarFound << std::endl;

				// log stuff
				debugLog << ioMgr->getDataInfo();
				writeLog(outputLogFilename, debugLog.str());


				MPI_Barrier(MPI_COMM_WORLD);

				void *decompdata = NULL;

				if (!sameCompressorParams && scalarFound == false)
				{
					//std::cout << "tes1" << std::endl;
					metricsInfo << "None" << std::endl;
					csvOutput << "None" << "_" << scalars[i] << "__" << "None"
							<< ", " << jsonInput["data-reduction"]["cbench-compressors"][c]["output-prefix"].get<std::string>() << ", ";

					clock.start("compress");
					clock.start("decompress");

					clock.stop("compress");
					clock.stop("decompress");


					//std::cout << "tes2" << std::endl;


					size_t numel = ioMgr->getSizePerDim()[0];
					for (int i=1; i<5; i++)
						if (ioMgr->getSizePerDim()[i] != 0)
							numel *= ioMgr->getSizePerDim()[i];

					//std::cout << "tes3" << std::endl;
					//std::cout << "numel " << numel << std::endl;
					//std::cout << "ioMgr->getTypeSize() " << ioMgr->getTypeSize() << std::endl;
					//std::cout << "tes4" << std::endl;

					decompdata = malloc(numel*ioMgr->getTypeSize());
					//std::cout << "tes5" << std::endl;
					memcpy(decompdata, ioMgr->data, numel*ioMgr->getTypeSize() );
					compressorMgr->setCompressedSize(numel*ioMgr->getTypeSize());

					//std::cout << "testes" << std::endl;
				}
				else
				{
					metricsInfo << compressorMgr->getParamsInfo() << std::endl;
					csvOutput << compressorMgr->getCompressorName() << "_" << scalars[i] << "__" << compressorMgr->getParamsInfo()
							<< ", " << jsonInput["data-reduction"]["cbench-compressors"][c]["output-prefix"].get<std::string>() << ", ";

					
					//
					// compress
					void *cdata = NULL;

					clock.start("compress");
					compressorMgr->compress(ioMgr->data, cdata, ioMgr->getType(), ioMgr->getTypeSize(), ioMgr->getSizePerDim());
					clock.stop("compress");


					//
					// decompress
					//void *decompdata = NULL;

					clock.start("decompress");
					compressorMgr->decompress(cdata, decompdata, ioMgr->getType(), ioMgr->getTypeSize(), ioMgr->getSizePerDim());
					clock.stop("decompress");

				}
				//MPI_Barrier(MPI_COMM_WORLD);


				// Get compression ratio
				unsigned long totalCompressedSize;
				unsigned long compressedSize = (unsigned long) compressorMgr->getCompressedSize();
				MPI_Allreduce(&compressedSize, &totalCompressedSize, 1, MPI_UNSIGNED_LONG, MPI_SUM, MPI_COMM_WORLD);

				unsigned long totalUnCompressedSize;
				unsigned long unCompressedSize = ioMgr->getTypeSize() * ioMgr->getNumElements();
				MPI_Allreduce(&unCompressedSize, &totalUnCompressedSize, 1, MPI_UNSIGNED_LONG, MPI_SUM, MPI_COMM_WORLD);


				debugLog << "\n\ncompressedSize: " << compressedSize << ", totalCompressedSize: " << totalCompressedSize << std::endl;
				debugLog << "unCompressedSize: " << unCompressedSize << ", totalUnCompressedSize: " << totalUnCompressedSize << std::endl;
				debugLog << "Compression ratio: " << totalUnCompressedSize / (float) totalCompressedSize << std::endl;


				//
				// Cycle through metrics
				debugLog << "\n----- " << scalars[i] << " error metrics ----- " << std::endl;
				metricsInfo << "\nField: " << scalars[i] << std::endl;

				MetricInterface *metricsMgr;
				for (int m=0; m<metrics.size(); ++m)
				{
					metricsMgr = MetricsFactory::createMetric(metrics[m]);
					if (metricsMgr == NULL)
					{
						if (myRank == 0)
							std::cout << "Unsupported metric: " << metrics[m] << " ... Skipping!" << std::endl;

						debugLog << "Unsupported metric: " << metrics[m] << " ... Skipping!"<< std::endl;
						continue;
					}


					// Read in additional params for metrics
					for (auto it = jsonInput["data-reduction"]["cbench-metrics"][m].begin(); it != jsonInput["data-reduction"]["cbench-metrics"][m].end(); it++)
					{
						if (it.key() != "name")
							for (auto it2 = jsonInput["data-reduction"]["cbench-metrics"][m][it.key()].begin();
									it2 != jsonInput["data-reduction"]["cbench-metrics"][m][it.key()].end(); it2++)
								if (*it2 != scalars[i])
									continue;
								else
								{
									metricsMgr->parameters[it.key()] = strConvert::toStr(scalars[i]);
									break;
								}
					}


					// Launch
					metricsMgr->init(MPI_COMM_WORLD);
					metricsMgr->execute(ioMgr->data, decompdata, ioMgr->getNumElements(), ioMgr->getType());
					
					csvOutput << metricsMgr->getGlobalValue() << ", ";

					// draw histogram if needed
					if (myRank == 0)
						if (metricsMgr->additionalOutput != "")
						{
							createFolder("logs");
							std::string outputHistogramName = "logs/";
							outputHistogramName += extractFileName(fileToLoad) + "_" + compressors[c] + "_" + scalars[i];
							outputHistogramName += "_" + metrics[m] + "_" + compressorMgr->getParamsInfo() + "_hist.py";
							writeFile(outputHistogramName, metricsMgr->additionalOutput);
						}
					metricsMgr->close();
				} // metrics
				debugLog << "-----------------------------\n";
				debugLog << "\nMemory in use: " << memLoad.getMemoryInUseInMB() << " MB" << std::endl;


				//
				// Metrics Computation
				double compress_time = clock.getDuration("compress");
				double decompress_time = clock.getDuration("decompress");

				double compress_throughput   = ((double) (ioMgr->getNumElements() * ioMgr->getTypeSize()) / (1024.0 * 1024.0)) / compress_time;     // MB/s
				double decompress_throughput = ((double) (ioMgr->getNumElements() * ioMgr->getTypeSize()) / (1024.0 * 1024.0)) / decompress_time;	// MB/s


				double max_compress_throughput = 0;
				double max_decompress_throughput = 0;
				double min_compress_throughput = 0;
				double min_decompress_throughput = 0;
				double max_compress_time = 0;

				MPI_Reduce(&compress_throughput, &max_compress_throughput, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
				MPI_Reduce(&decompress_throughput, &max_decompress_throughput, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
				MPI_Reduce(&compress_time, &max_compress_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

				MPI_Reduce(&compress_throughput, &min_compress_throughput, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
				MPI_Reduce(&decompress_throughput, &min_decompress_throughput, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);


				//
				// Store the uncompressed data pointer, does not actually write yet
				if (writeData)
				{
					debugLog << "writing: " << scalars[i] << std::endl;
					ioMgr->saveCompData(scalars[i], decompdata);
				}


				//
				// deallocate
				std::free(decompdata);


				ioMgr->close();
				memLoad.stop();


				//
				// log stuff
				debugLog << "\nCompress time: " << compress_time << std::endl;
				debugLog << "Decompress time: " << decompress_time << std::endl;
				debugLog << "\nMemory leaked: " << memLoad.getMemorySizeInMB() << " MB" << std::endl;
				debugLog << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl << std::endl;

				writeLog(outputLogFilename, debugLog.str());


				//
				// log to metrics file and output metrics csv
				if (myRank == 0)
				{
					metricsInfo << "Max Compression Throughput: " << max_compress_throughput << " Mbytes/s" << std::endl;
					metricsInfo << "Max DeCompression Throughput: " << max_decompress_throughput << " Mbytes/s" << std::endl;
					metricsInfo << "Max Compress Time: " << max_compress_time << " s" << std::endl;

					metricsInfo << "Min Compression Throughput: " << min_compress_throughput << " Mbytes/s" << std::endl;
					metricsInfo << "Min DeCompression Throughput: " << min_decompress_throughput << " Mbytes/s" << std::endl;

					metricsInfo << "Compression ratio: " << totalUnCompressedSize / (float) totalCompressedSize << std::endl;
					csvOutput << min_compress_throughput << ", " << min_decompress_throughput << ", " 
							<< totalUnCompressedSize / (float) totalCompressedSize << "\n";

					{
						std::string metricsFile = jsonInput["data-reduction"]["cbench-output"]["metrics-file"];
						writeFile(metricsFile, metricsInfo.str());

						if (numTimesteps > 1)
							writeFile(metricsFile + std::to_string(ts) + ".csv", csvOutput.str());
						else
							writeFile(metricsFile + ".csv", csvOutput.str());
					}
				}

				MPI_Barrier(MPI_COMM_WORLD);
			}  // scalars


			//
			// write data to disk if requested in the json file
			if (writeData)
			{
				clock.start("write");

				ioMgr->loadUncompressedFields(jsonInput);


				// Pass through original data to preserve original file data structure
				for (int i = 0; i < ioMgr->inOutData.size(); i++)
				{
					if (!ioMgr->inOutData[i].doWrite)
					{
						debugLog << "writing uncoompressed" << std::endl;
						ioMgr->loadData(ioMgr->inOutData[i].name);
						ioMgr->saveCompData(ioMgr->inOutData[i].name, ioMgr->data);
						ioMgr->close();
					}
				}



				//
				// Get the name and path of the new file
				std::string decompressedOutputName, fileToOutput;
				if (jsonInput["data-reduction"]["cbench-compressors"][c].contains("output-prefix"))
					decompressedOutputName = jsonInput["data-reduction"]["cbench-compressors"][c]["output-prefix"];
				else
					decompressedOutputName = "__" + compressorMgr->getCompressorName() + "_" + std::to_string(rand());

				fileToOutput = extractFileName(fileToLoad);				

				// path for folders
				if (outputPath != ".")
					decompressedOutputName = outputPath + "/" + decompressedOutputName + "__" + fileToOutput;
				else
					decompressedOutputName = decompressedOutputName + "__" + fileToOutput;


				//
				// Write out uncompressed (lossy) data
				ioMgr->writeData(decompressedOutputName);


				clock.stop("write");

				if (myRank == 0)
					std::cout << "wrote out " << decompressedOutputName << "." << std::endl;

				debugLog << "Write output took: " << clock.getDuration("write") << " s " << std::endl;

				writeLog(outputLogFilename, debugLog.str());
			}  // write Data

			compressorMgr->close();
		} // compressors
	} // timesteps

	//overallClock.stop();
	clock.stop("overall");
	debugLog << "\nTotal run time: " << clock.getDuration("overall") << " s " << std::endl;
	writeLog(outputLogFilename, debugLog.str());


	// For humans
	if (myRank == 0)
		std::cout << "\nThat's all folks!" << std::endl;

	MPI_Finalize();

	return 0;
}


/*
Run:
mpirun -np 4 CBench ../inputs/hacc/HACC_all.json
mpirun -np 4 CBench ../inputs/nyx/nyx_all.json
*/



