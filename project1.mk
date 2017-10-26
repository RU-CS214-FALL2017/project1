##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=project1
ConfigurationName      :=Debug
WorkspacePath          :=C:/Users/kkash/Documents/SysProgramming
ProjectPath            :=C:/Users/kkash/Documents/SysProgramming/project1
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=kkash
Date                   :=26/10/2017
CodeLitePath           :=C:/CodeLite
LinkerName             :=C:/TDM-GCC-64/bin/g++.exe
SharedObjectLinkerName :=C:/TDM-GCC-64/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="project1.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/TDM-GCC-64/bin/windres.exe
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/TDM-GCC-64/bin/ar.exe rcu
CXX      := C:/TDM-GCC-64/bin/g++.exe
CC       := C:/TDM-GCC-64/bin/gcc.exe
CXXFLAGS :=  -g -O0 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/TDM-GCC-64/bin/as.exe


##
## User defined environment variables
##
CodeLiteDir:=C:\CodeLite
Objects0=$(IntermediateDirectory)/project1_analyser.c$(ObjectSuffix) $(IntermediateDirectory)/project1_main.c$(ObjectSuffix) $(IntermediateDirectory)/project1_mainTools.c$(ObjectSuffix) $(IntermediateDirectory)/project1_sorter.c$(ObjectSuffix) $(IntermediateDirectory)/project1_tools.c$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@$(MakeDirCommand) "./Debug"


$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/project1_analyser.c$(ObjectSuffix): project1/analyser.c $(IntermediateDirectory)/project1_analyser.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/kkash/Documents/SysProgramming/project1/project1/analyser.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/project1_analyser.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/project1_analyser.c$(DependSuffix): project1/analyser.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/project1_analyser.c$(ObjectSuffix) -MF$(IntermediateDirectory)/project1_analyser.c$(DependSuffix) -MM project1/analyser.c

$(IntermediateDirectory)/project1_analyser.c$(PreprocessSuffix): project1/analyser.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/project1_analyser.c$(PreprocessSuffix) project1/analyser.c

$(IntermediateDirectory)/project1_main.c$(ObjectSuffix): project1/main.c $(IntermediateDirectory)/project1_main.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/kkash/Documents/SysProgramming/project1/project1/main.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/project1_main.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/project1_main.c$(DependSuffix): project1/main.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/project1_main.c$(ObjectSuffix) -MF$(IntermediateDirectory)/project1_main.c$(DependSuffix) -MM project1/main.c

$(IntermediateDirectory)/project1_main.c$(PreprocessSuffix): project1/main.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/project1_main.c$(PreprocessSuffix) project1/main.c

$(IntermediateDirectory)/project1_mainTools.c$(ObjectSuffix): project1/mainTools.c $(IntermediateDirectory)/project1_mainTools.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/kkash/Documents/SysProgramming/project1/project1/mainTools.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/project1_mainTools.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/project1_mainTools.c$(DependSuffix): project1/mainTools.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/project1_mainTools.c$(ObjectSuffix) -MF$(IntermediateDirectory)/project1_mainTools.c$(DependSuffix) -MM project1/mainTools.c

$(IntermediateDirectory)/project1_mainTools.c$(PreprocessSuffix): project1/mainTools.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/project1_mainTools.c$(PreprocessSuffix) project1/mainTools.c

$(IntermediateDirectory)/project1_sorter.c$(ObjectSuffix): project1/sorter.c $(IntermediateDirectory)/project1_sorter.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/kkash/Documents/SysProgramming/project1/project1/sorter.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/project1_sorter.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/project1_sorter.c$(DependSuffix): project1/sorter.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/project1_sorter.c$(ObjectSuffix) -MF$(IntermediateDirectory)/project1_sorter.c$(DependSuffix) -MM project1/sorter.c

$(IntermediateDirectory)/project1_sorter.c$(PreprocessSuffix): project1/sorter.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/project1_sorter.c$(PreprocessSuffix) project1/sorter.c

$(IntermediateDirectory)/project1_tools.c$(ObjectSuffix): project1/tools.c $(IntermediateDirectory)/project1_tools.c$(DependSuffix)
	$(CC) $(SourceSwitch) "C:/Users/kkash/Documents/SysProgramming/project1/project1/tools.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/project1_tools.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/project1_tools.c$(DependSuffix): project1/tools.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/project1_tools.c$(ObjectSuffix) -MF$(IntermediateDirectory)/project1_tools.c$(DependSuffix) -MM project1/tools.c

$(IntermediateDirectory)/project1_tools.c$(PreprocessSuffix): project1/tools.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/project1_tools.c$(PreprocessSuffix) project1/tools.c


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


