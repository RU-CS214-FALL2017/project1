dir = project1
sourceFiles = $(dir)/main.c $(dir)/analyser.c $(dir)/sorter.c $(dir)/tools.c $(dir)/mainTools.c $(dir)/free.c
allFiles = $(sourceFiles) $(dir)/analyser.h $(dir)/sorter.h $(dir)/tools.h $(dir)/mainTools.h $(dir)/free.h

sorter: $(allFiles)
	gcc -std=gnu11 $(sourceFiles) -o sorter -Wall

debug: $(allFiles)
	gcc -g -std=gnu11 $(sourceFiles) -o sorter -Wall