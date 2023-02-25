#include "Libraries.h"

#include "Decoder.h"
#include "Encoder.h"
#include "ParseCmd.h"

int main(int argc, char **argv) {
    ParseCmd parsed(argc, argv);
    if (parsed.result_ == ParseCmd::Result::Error) {
        std::cout << parsed.error_message_ << std::endl << std::endl;
        parsed.PrintHelp(false);
        return 111;
    } else if (parsed.result_ == ParseCmd::Result::Help) {
        parsed.PrintHelp();
        return 0;
    } else if (parsed.result_ == ParseCmd::Result::Encode) {
        std::cout << "Encoding..." << std::endl << std::endl;
        std::ofstream out(parsed.archive_name_, std::ios::binary);
        Encoder encoder(out);
        for (const auto &file : parsed.file_names_) {
            encoder.AddFile(file);
            std::cout << "Encoded file " << file << std::endl;
        }
        encoder.ArchiveEnd();
        out.close();
        std::cout << std::endl << "Archive encoded." << std::endl;
    } else {
        std::cout << "Decoding..." << std::endl << std::endl;
        std::ifstream in(parsed.archive_name_);
        try {
            Decoder decoder(in);
            decoder.Decode();
        } catch (const std::exception &error) {
            std::cerr << error.what();
            return 111;
        }
        in.close();
        std::cout << std::endl << "Archive decoded." << std::endl;
    }
    return 0;
}
