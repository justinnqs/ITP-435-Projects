# If you create new headers/cpp files, add them to these list!
set(HEADER_FILES
	SrcMain.h
    ParseFile.h
    AminoCounter.h
    PairWise.h
)

set(SOURCE_FILES
	SrcMain.cpp
    ParseFile.cpp
    AminoCounter.cpp
    PairWise.cpp
)

# Don't change this
add_library(src ${SOURCE_FILES} ${HEADER_FILES})
