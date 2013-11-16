Multiprocess External Sort
===========================

Our assignment was to design a program that can take in multiple files, sort each file, and write to a master file. We were instructed to build a tree with multiple process (or threads) and to have each process sort one file. Once all files are sorted, each process would send up the results to it's parent. The parent would sort the results of the two children and send the results to it's parent. This process should repeat until the master process has all the sorted files to write to the master file.

Things I Accomplished
---------------------
* Was able to set up and use source control (Github)
* Was able to create a simple process tree (One Parent, Two Children, and Four Grandchildren)
* Was able to read the contents of each file using multiple processes
* Was able to establish pipes for the processes
* Was able to write to pipes
* Was able to create a log file for debugging purposes

Wish List
---------
* Wasn't able to get reading from a pipe to work
* Wasn't able to sort the contents of each file (stuck on atoi)
* Wanted to provide a cool visualization of executing code


1.) Make directory
2.) cd into directory
3.) git init
4.) Make/modify files as needed
5.) git add 'filenamehere'
6.) git commit -m 'messagehere'
7.) git push origin master
