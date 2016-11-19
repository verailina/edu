#include "ROCDiagrammTest.h"
#include "LSBReplacement.h"
#include <iostream>

std::string kImageExtension = ".pgm";
const double ROCDiagrammTest::kMessageLength = 0.02;

ROCDiagrammTest::ROCDiagrammTest()
{
}

void ROCDiagrammTest::CollectData(Analyzer *analyzer, int images_number, const std::string &result_file)
{
    analyzer_ = analyzer;
    std::ofstream output_file(result_file);
    //std::ofstream output_file_f("f" + result_file);
    std::vector<double>cover_p(images_number);
    std::vector<double>emebedded_p(images_number);

    for (int i = 0; i < images_number; ++i)
    {
        std::string image_name = std::to_string(i + 5) + kImageExtension;
        Image current_image;
        current_image.Read(image_folder + image_name);
        cover_p[i] = analyzer_->Analyze(current_image);
        LSBReplacement::EmbedMessage(current_image, kMessageLength * (current_image.height() * current_image.width()));
        emebedded_p[i] = analyzer_->Analyze(current_image);
        std::cout << "image " << i << std::endl;
    }

    for (double epsilon = 0.0; epsilon < 0.25; epsilon += 0.0001)
    {
        int true_positive_rate = 0;
        int false_positive_rate = 0;
        for (int i = 0; i < images_number; ++i)
        {
            if (fabs(cover_p[i]) > epsilon)
            {
                false_positive_rate++;
            }
            if (emebedded_p[i] > epsilon)
            {
                true_positive_rate++;
            }
        }
        output_file << (double)true_positive_rate / images_number << " ";
        output_file<< (double)false_positive_rate / images_number << std::endl;
    }
    output_file.close();
}
