#include "Image.h"
#include "LSBReplacement.h"
#include "RSMethod.h"
#include "WSMethod.h"
#include "SamplePairMethod.h"
#include "ROCDiagrammTest.h"
#include <iostream>
#include <sstream>
#include <vector>

const std::string rs_string = "-rs";
const std::string sp_string = "-sp";
const std::string ws_string = "-ws";
const std::string roc_test_string = "-roc";
const std::string all_test_string = "-all";

void rs_test(const std::string &cover_image_file_name,
             const std::string &result_file_name)
{
    std::ofstream output_file(result_file_name);
    for (double message_part = 0; message_part <= 1; message_part += 0.05)
    {
        Image image;
        image.Read(image_folder + cover_image_file_name);
        int message_length = message_part * (image.height() * image.width());
        LSBReplacement::EmbedRandomMessage(image, message_length);
        RSMethod analyzer({ 0, 1, 1, 0 });
        output_file << analyzer.Analyze(image) << std::endl;
    }
}

void sp_test(const std::string &cover_image_file_name,
    const std::string &result_file_name)
{
    std::ofstream output_file(result_file_name);
    for (double message_part = 0; message_part <= 1; message_part += 0.05)
    {
        Image image;
        image.Read(image_folder + cover_image_file_name);
        int message_length = message_part * (image.height() * image.width());
        LSBReplacement::EmbedRandomMessage(image, message_length);
        SamplePairMethod analyzer;
        output_file << analyzer.Analyze(image) << std::endl;
    }
}

void ws_test(const std::string &cover_image_file_name,
    const std::string &result_file_name)
{
    std::ofstream output_file(result_file_name);
    for (double message_part = 0; message_part <= 1; message_part += 0.05)
    {
        Image image;
        image.Read(image_folder + cover_image_file_name);
        int message_length = message_part * (image.height() * image.width());
        LSBReplacement::EmbedMessage(image, message_length);
        WSMethod analyzer;
        output_file << analyzer.Analyze(image) << std::endl;
    }
}

double analyse(Analyzer * analyser, const std::string &file_name, double message_part)
{
    Image image;
    image.Read(image_folder + file_name);
    int message_length = message_part * (image.height() * image.width());
    LSBReplacement::EmbedMessage(image, message_length);
    return analyser->Analyze(image);
}

void total_test(int images_number, double step)
{
    int values_number = 1.0 / step;
    std::vector<double> rs_values(values_number, 0.0);
    std::vector<double> sp_values(values_number, 0.0);
    std::vector<double> ws_values(values_number, 0.0);

    for (int i = 0; i < images_number; ++i)
    {
        std::cout << "image " << i << std::endl;
        for (int j = 0; j < values_number; ++j)
        {
            Image image;
            std::string file_name = std::to_string(i) + ".pgm";
            double message_part = j * step;

            rs_values[j] += analyse(&RSMethod({ 0, 1, 1, 0 }), file_name, message_part);
            sp_values[j] += analyse(&SamplePairMethod(), file_name, message_part);
            ws_values[j] += analyse(&WSMethod(), file_name, message_part);
        }
    }

    std::ofstream out("total_tes.txt");
    for (int j = 0; j < values_number; ++j)
    {
        ws_values[j] /= images_number;
        rs_values[j] /= images_number;
        sp_values[j] /= images_number;
        out << j * step << " " << rs_values[j] << " " << sp_values[j] << " " << ws_values[j] << std::endl;
    }
    out.close();
}

int main(int argc, char *argv[])
{
    if (argc == 3)
    {
        std::string image_name = argv[1];
        std::string analyzer_type = argv[2];
        Image image;
        image.Read(image_name);

        if (analyzer_type == rs_string)
        {
            std::cout << RSMethod({ 0, 1, 1, 0 }).Analyze(image) << std::endl;
        }

        else
        {
            if (analyzer_type == sp_string)
            {
                std::cout << SamplePairMethod().Analyze(image) << std::endl;
            }
            else
            {
                if (analyzer_type == ws_string)
                {
                    std::cout << WSMethod().Analyze(image) << std::endl;
                }
                else
                {
                    std::cout << "Incorrect method type" << std::endl;
                    return 1;
                }
            }
        }
    }
    else
    {
        if (argc == 4)
        {
            if (argv[1] == roc_test_string)
            {
                std::stringstream ss(argv[3]);
                int images_number = 0;
                ss >> images_number;
                if (argv[2] == rs_string)
                {
                    ROCDiagrammTest().CollectData(&RSMethod({ 0, 1, 1, 0 }), images_number, "rs_roc.txt");
                    return 0;
                }
                if (argv[2] == sp_string)
                {
                    ROCDiagrammTest().CollectData(&SamplePairMethod(), images_number, "sp_roc.txt");
                    return 0;
                }
                if (argv[2] == ws_string)
                {
                    ROCDiagrammTest().CollectData(&WSMethod(), images_number, "ws_roc.txt");
                    return 0;
                }
            }
            else
            {
                if (argv[1] == all_test_string)
                {
                    std::stringstream ss(argv[2]);
                    int images_number = 0;
                    ss >> images_number;
                    std::stringstream ss1(argv[3]);
                    double step = 0.0;
                    ss1 >> step;

                    total_test(images_number, step);
                }
            }
        }
    }
    return 0;
}