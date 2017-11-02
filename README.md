Assumptions
===========
* CSV files' rows are delimited by `'\n'`

Usage
=====
`./sorter -c *columnheader* [-d *dirpath*] [-o *outputpath*]`
* Sorts all the CSV files in *dirpath*  according to *columnheader* which can be cascaded
* If *dirpath* is not specified the current directory will be processed
* All sorted files will be in *outputpath* if specified, otherwise they will be in the same directory as the original file

Compile
-------
`make sorter`
 

Design
======
1. The initial process searches a directory
2. For every sub-directory the processes forks and the child processes that
3. For every CSV file the processes forks and the child sorts it

Extra Credit
------------
### Cascading
This was very simple to implement. I jsut recursivly used the functions I already made to cascade sort a smaller area of the CSV file.
### Printing Structure
This was kind of dificult. I created a memory management system for mapped memory. I created an algorithm that gives each PID a segregated memory so there are no clashing read/writes.