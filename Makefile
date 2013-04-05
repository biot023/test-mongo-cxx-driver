RM  := rm -f

INCLUDE_DIRS := include /usr/local/include /opt/local/include /home/ubuntu/work/src/mongo-cxx-driver-nightly/src
CXXFLAGS      = -std=c++11 -Wall -g $(addprefix -I,$(INCLUDE_DIRS))
SRC           = $(wildcard src/*.cpp)
OBJ           = $(subst .cpp,.o,$(SRC))
CLEANLIST     = bin/runner $(OBJ)
LDFLAGS       = /home/ubuntu/work/src/mongo-cxx-driver-nightly/build/libmongoclient.a -lboost_thread -lboost_filesystem -lboost_program_options
LD_LIBRARY_PATH += /usr/local/lib

all : bin/tutorial

.PHONY : all clean

clean :
	$(RM) $(CLEANLIST)

bin/tutorial : src/tutorial.o
	$(CXX) $+ -o $@ $(LDFLAGS)
