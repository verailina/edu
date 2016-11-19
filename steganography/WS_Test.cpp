//#define _SCL_SECURE_NO_WARNINGS
//#define _CRT_SECURE_NO_WARNINGS
//#include "WSMethod.h"
//#include "LSBReplacement.h"
//#include <iostream>
//
//const std::string image_folder = "D:\\Õ√”\\BOWS2OrigEp3\\BOWS2OrigEp3\\";
//
////const std::string image_folder = "C:\\Users\\vvego_000\\Downloads\\BOWS2OrigEp3\\BOWS2OrigEp3\\";
//
//void WS_Test(const std::string &file_name, double message_part)
//{
//    Image image;
//    image.Read(image_folder + file_name);
//    int message_length = message_part * (image.height() * image.width());
//    LSBReplacement::EmbedMessage(image, message_length);
//    //image.Print();
//    WSMethod analyzer;
//    std::cout << message_part << " " << analyzer.Analyze(image) << std::endl;
//}
//
//int main()
//{    
//    for (double i = 0; i <= 1; i += 0.1)
//    {
//        WS_Test("10.pgm", i);
//    }
//    return 0;
//}