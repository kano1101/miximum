CC = clang++
LD = clang++
CCFLAGS = -Wall -Wextra -c -std=c++11
LDFLAGS = 

SourceDir = src
BuildDir = build
BinDir = bin
ProductName = mix-test
UnixExecDir = $(BinDir)/unix-exec
UnixExec = $(UnixExecDir)/$(ProductName)

#ResourcesDir = resc
#Images = $(wildcard $(ResourcesDir)/imgs/*)

Sources = $(wildcard src/*.cpp)

Objects = $(patsubst $(SourceDir)/%.cpp,$(BuildDir)/%.o,$(Sources)) 


all: $(UnixExec)

unix: $(UnixExec)


$(UnixExec): $(Objects)# $(Images)
	$(CC) $(LDFLAGS) $(Objects) -o $(UnixExec)

$(Objects): $(BuildDir)/%.o: $(SourceDir)/%.cpp 
	$(CC) $(CCFLAGS) $< -o $@

test:
	cd $(UnixExecDir); ./$(ProductName)

.PHONY: clean
clean:
	rm -f $(BuildDir)/*o 
	rm -rf $(UnixExecDir)/*

.PHONY: run
run:
	make clean
	make all
	make test
