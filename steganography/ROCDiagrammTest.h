#pragma once
#include "Analyzer.h"
#include <fstream>
#include "Image.h"
#include <string>
const std::string image_folder = "D:\\Õ√”\\BOWS2OrigEp3\\BOWS2OrigEp3\\";

class ROCDiagrammTest
{
public:
    ROCDiagrammTest();
    void CollectData(Analyzer *analyzer, int images_number, const std::string &result_file);
private:
    
    Analyzer *analyzer_;
    static const double kMessageLength;
    int images_number;

};

