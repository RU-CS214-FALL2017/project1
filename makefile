flags = -std=gnu11 -Wall
dir = project1
sourceFiles = $(dir)/main.c $(dir)/analyser.c $(dir)/sorter.c $(dir)/tools.c $(dir)/mainTools.c $(dir)/memTools.c $(dir)/forkTools.c
allFiles = $(sourceFiles) $(dir)/analyser.h $(dir)/sorter.h $(dir)/tools.h $(dir)/mainTools.h $(dir)/memTools.h $(dir)/forkTools.h

sorter: $(allFiles)
	gcc $(flags) $(sourceFiles) -o sorter

debug: $(allFiles)
	gcc $(flags) -g $(sourceFiles) -o debugSorter