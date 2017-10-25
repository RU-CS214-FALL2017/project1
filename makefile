dir = porject1
sourceFiles = $(dir)/main.c $(dir)/analyser.c $(dir)/sorter.c $(dir)/tools.c $(dir)/mainTools.c
allFiles = $(sourceFiles) $(dir)/analyser.h $(dir)/sorter.h $(dir)/tools.h $(dir)/mainTools.h

sorter: $(allFiles)
	gcc -std=gnu11 $(sourceFiles) -o sorter -Wall