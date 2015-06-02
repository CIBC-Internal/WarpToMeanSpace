#ifndef INPUTPARAMS_H
#define INPUTPARAMS_H

#include "tinyxml.h"
#include "utils.h"
#include <sys/stat.h> // for mkdir

class InputParams
{
public:
    // correspondence model parameters
    int number_of_particles ;
    int do_procrustes_scaling;

    // where to save files for the template (DT, dense and sparse shapes) and warped DT
    std::string out_prefix;
    std::string out_path;

    // this is number of particles to be used to build the warp, their choice will be based on the ranking if point files are provided -->
    // otherwise, the choice will be random
    int number_of_correspondences_in_warp;

    // attributes that can be used to help quantifying the quality of each correspondence, e.g. sulcal depth for the brains
    int number_of_attributes;
    std::string attributes_prefix;
    std::vector< std::string> attribute_labels;

    // isosurface extraction parameters
    float levelsetValue ;
    float targetReduction;
    float featureAngle   ;
    int   lsSmootherIterations;
    int   meshSmootherIterations;
    bool  preserveTopology    ;
    bool  display             ;

    // input files
    std::vector< std::string > localPointsFilenames;
    std::vector< std::string > distanceTransformFilenames;
    std::vector< std::string > attributeFilenames;
    //std::vector< std::string > landmarksFilenames;

    InputParams(){

        // correspondence model parameters
        number_of_particles               = -1;
        do_procrustes_scaling             =  1;
        number_of_correspondences_in_warp = -1;
        number_of_attributes              =  0;

        // isosurface parameters
        levelsetValue          = 0.5; // to make sure no holes (don't use strict 0.0 since this will isosurface a mean distance transform - i.e. having non-zero isosurface)
        targetReduction        = 0.01; // percentage to decimate
        featureAngle           = 30; // in degrees
        lsSmootherIterations   = 1; // number of iterations to smooth the levelset
        meshSmootherIterations = 1; // number of iterations to smooth the initial mesh
        preserveTopology       = true;

        display                = true;

        localPointsFilenames.clear();
        distanceTransformFilenames.clear();
        attribute_labels.clear();
    }

    int readParams(char* filename)
    {
        // read parameters
        TiXmlDocument doc(filename);
        bool loadOkay = doc.LoadFile();

        if (loadOkay)
        {
            TiXmlHandle docHandle( &doc );
            TiXmlElement *elem;
            std::istringstream inputsBuffer;
            std::string filename("/dev/null\0");

            // Compile the list of input files.
            elem = docHandle.FirstChild( "local_point_files" ).Element();
            if (elem)
            {
                inputsBuffer.str(elem->GetText());
                while (inputsBuffer >> filename)
                {
                    localPointsFilenames.push_back(filename);
                }
                inputsBuffer.clear();
                inputsBuffer.str("");
            }
            else
            {
                std::cerr << "No local points to process!" << std::endl;
                return EXIT_FAILURE;
            }

            elem = docHandle.FirstChild( "distance_transform_files" ).Element();
            if (elem)
            {
                inputsBuffer.str(elem->GetText());
                while (inputsBuffer >> filename)
                {
                    distanceTransformFilenames.push_back(filename);
                }
                inputsBuffer.clear();
                inputsBuffer.str("");
            }
            else
            {
                std::cerr << "No distance transforms to process!" << std::endl;
                return EXIT_FAILURE;
            }

            elem = docHandle.FirstChild( "out_prefix" ).Element();
            if (elem)
            {
                inputsBuffer.str(elem->GetText());
                inputsBuffer >> out_prefix;
                inputsBuffer.clear();
                inputsBuffer.str("");
            }
            else
            {
                std::cerr << "No out_prefix provided ...!" << std::endl;
                return EXIT_FAILURE;
            }

            elem = docHandle.FirstChild( "number_of_particles" ).Element();
            if (elem)
            {
                number_of_particles = atoi( elem->GetText() );
            }

            elem = docHandle.FirstChild( "number_of_attributes" ).Element();
            if (elem)
            {
                number_of_attributes = atoi( elem->GetText() );
            }

            elem = docHandle.FirstChild( "attributes_prefix" ).Element();
            if (elem)
            {
                inputsBuffer.str(elem->GetText());
                inputsBuffer >> attributes_prefix;
                inputsBuffer.clear();
                inputsBuffer.str("");
            }
            else
            {
                if(number_of_attributes > 0)
                {
                    std::cerr << "No attributes_prefix provided ...!" << std::endl;
                    return EXIT_FAILURE;
                }
            }


            elem = docHandle.FirstChild( "attribute_labels" ).Element();
            if (!elem)
            {
                if(number_of_attributes > 0)
                {
                    std::cerr << "No attribute files have been specified" << std::endl;
                    return EXIT_FAILURE;
                }
            }
            else
            {
                inputsBuffer.str(elem->GetText());
                while (inputsBuffer >> filename)
                {
                    attribute_labels.push_back(filename);
                }
                inputsBuffer.clear();
                inputsBuffer.str("");
            }

            elem = docHandle.FirstChild( "number_of_correspondences_in_warp" ).Element();
            if (elem)
            {
                number_of_correspondences_in_warp = atoi( elem->GetText() );
            }
            else
            {
                std::cerr << "No number_of_correspondences_in_warp provided, all correspondences will be used to build the warp ...!" << std::endl;
            }

            elem = docHandle.FirstChild( "do_procrustes_scaling" ).Element();
            if (elem)
            {
                do_procrustes_scaling = atoi( elem->GetText() );
            }

            // read levelset value if given
            elem = docHandle.FirstChild( "levelsetValue" ).Element();
            if (elem)
            {
                levelsetValue = atof( elem->GetText() );
            }

            // read target reduction value if given
            elem = docHandle.FirstChild( "targetReduction" ).Element();
            if (elem)
            {
                targetReduction = atof( elem->GetText() );
            }

            // read feature angle value if given
            elem = docHandle.FirstChild( "featureAngle" ).Element();
            if (elem)
            {
                featureAngle = atof( elem->GetText() );
            }

            // read number of iterations for levelset smoother if given
            elem = docHandle.FirstChild( "lsSmootherIterations" ).Element();
            if (elem)
            {
                lsSmootherIterations = atoi( elem->GetText() );
            }

            // read number of iterations for mesh smoother if given
            elem = docHandle.FirstChild( "meshSmootherIterations" ).Element();
            if (elem)
            {
                meshSmootherIterations = atoi( elem->GetText() );
            }


            // check if topology changes are allowed
            elem = docHandle.FirstChild( "preserveTopology" ).Element();
            if (elem)
            {
                atoi(elem->GetText()) > 0 ? preserveTopology = true : preserveTopology = false;
            }

            // check if we want to display the computed template
            elem = docHandle.FirstChild( "display" ).Element();
            if (elem)
            {
                atoi(elem->GetText()) > 0 ? display = true : display = false;
            }

            // Make sure lists are the same size.
            if (localPointsFilenames.size() != distanceTransformFilenames.size())
            {
                std::cerr << "local points file list size does not match distance transform file list size!" << std::endl;
                return EXIT_FAILURE;
            }

            if (number_of_particles <= 0)
            {
                std::cerr << "Invalide number of particles, it should be > 0 ...!" << std::endl;
                return EXIT_FAILURE;
            }

            if(number_of_correspondences_in_warp <= 0)
                number_of_correspondences_in_warp = number_of_particles;

            if(number_of_correspondences_in_warp > number_of_particles)
            {
                number_of_correspondences_in_warp = number_of_particles;
                std::cerr << "number_of_correspondences_in_warp exceeds the total number of particles, all correspondences will be used to build the warp ...!" << std::endl;
            }

            // create the directory where the results should be, in case it is not there
            out_path = getPath(out_prefix);

            // remove the output if it already exists to remove any historical results, just in case
            std::string cmdStr = "rm -rf " + out_path;
            std::system(cmdStr.c_str());
            int mkdirStatus = mkdir(out_path.c_str(), S_IRWXU);

            // copy the current param file to the output to keep track of the parameters that generated this output
            cmdStr = "cp " + std::string(filename) + " " + out_path;
            std::system(cmdStr.c_str());
        }
    }
};

#endif // INPUTPARAMS_H
