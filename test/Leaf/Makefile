CC = clang++
LD = clang++
CCFLAGS = -Wall -Wextra -c
LDFLAGS = -headerpad_max_install_names

SourceDir = src
BuildDir = build
BinDir = bin
ProductName = mix-test
UnixExecDir = $(BinDir)/unix-exec
UnixExec = $(UnixExecDir)/$(ProductName)

ResourcesDir = resc
Images = $(wildcard $(ResourcesDir)/imgs/*)

Sources = $(wildcard src/*.cpp)
#Headers += $(wildcard src/include/*.hpp)

Objects = $(patsubst $(SourceDir)/%.cpp,$(BuildDir)/%.o,$(Sources)) 


all: $(UnixExec)

unix: $(UnixExec)


$(UnixExec): $(Objects) $(Images)
	$(CC) $(LDFLAGS) $(Objects) -o $(UnixExec)
#	cp $(Images) $(UnixExecDir)

$(Objects): $(BuildDir)/%.o: $(SourceDir)/%.cpp 
	$(CC) $(CCFLAGS) $< -o $@

test:
	cd $(UnixExecDir); ./$(ProductName)

.PHONY: clean
clean:
	rm -f $(BuildDir)/*o 
	rm -rf $(UnixExecDir)/*
