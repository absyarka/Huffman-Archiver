add_executable(
        archiver
        archiver.cpp
        ParseCmd.cpp ParseCmd.h Libraries.h Writer.cpp Writer.h Encoder.cpp Encoder.h HuffmanTree.cpp HuffmanTree.h Reader.cpp Reader.h Decoder.cpp Decoder.h Libraries.cpp PriorityQueue.h)
add_catch(tester test.cpp
        ParseCmd.cpp ParseCmd.h Libraries.h Writer.cpp Writer.h Encoder.cpp Encoder.h HuffmanTree.cpp HuffmanTree.h Reader.cpp Reader.h Decoder.cpp Decoder.h Libraries.cpp PriorityQueue.h)