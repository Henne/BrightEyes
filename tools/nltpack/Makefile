CXX=g++
CXX_CFLAGS=-W -Wall -Wextra -pedantic
CXX_LFLAGS=
.PHONY:clean
CMD_RM=rm -rf
LDFLAGS=

nltpack_OBJ=nltpack.o fileutils.o dsa1.o dsa2.o dsa3.o

nltpack: $(nltpack_OBJ)
	$(CXX) $(nltpack_OBJ) -o $@ $(CXX_LFLAGS) $(LDFLAGS)

%.o:%.cpp %.hpp
	$(CXX) -c $< -o $@ $(CXX_CFLAGS)

clean:
	$(CMD_RM) *.o nltpack
