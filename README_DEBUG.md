bug: Identified data race in rb_tree_int_insert in map_utils.c
solution: create atomic funciton to get or insert element
helgrind report:

valgrind --tool=helgrind ./bin/test_rb_tree_multi_thread 

==24611== Helgrind, a thread error detector

==24611== Copyright (C) 2007-2017, and GNU GPL'd, by OpenWorks LLP et al.

==24611== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info

==24611== Command: ./bin/test_rb_tree_multi_thread

==24611== 

==24611== ---Thread-Announcement------------------------------------------

==24611== 

==24611== Thread #3 was created

==24611==    at 0x49BB8B7: clone (clone.S:62)

==24611==    by 0x49554C7: create_thread (pthread_create.c:297)

==24611==    by 0x4955F97: pthread_create@@GLIBC_2.34 (pthread_create.c:836)

==24611==    by 0x48929BB: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x108DF3: test_multithreaded_insertion (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108E93: main (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611== 

==24611== ---Thread-Announcement------------------------------------------

==24611== 

==24611== Thread #2 was created

==24611==    at 0x49BB8B7: clone (clone.S:62)

==24611==    by 0x49554C7: create_thread (pthread_create.c:297)

==24611==    by 0x4955F97: pthread_create@@GLIBC_2.34 (pthread_create.c:836)

==24611==    by 0x48929BB: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x108DF3: test_multithreaded_insertion (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108E93: main (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611== 

==24611== ---Thread-Announcement------------------------------------------

==24611== 

==24611== Thread #1 is the program's root thread

==24611== 

==24611== ----------------------------------------------------------------

==24611== 

==24611==  Lock at 0x4A8E490 was first observed

==24611==    at 0x4894560: pthread_mutex_init (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x108F5B: rb_tree_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x1094E3: rb_tree_int_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108D87: test_multithreaded_insertion (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108E93: main (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==  Address 0x4a8e490 is 16 bytes inside a block of size 80 alloc'd

==24611==    at 0x48873E0: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x108EAF: rb_tree_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x1094E3: rb_tree_int_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108D87: test_multithreaded_insertion (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108E93: main (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==  Block was alloc'd by thread #1

==24611== 

==24611== Possible data race during read of size 8 at 0x4A8E480 by thread #3

==24611== Locks held: none

==24611==    at 0x109214: rb_tree_at (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x10953F: rb_tree_int_insert (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108D03: stress_test_worker (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x4892B93: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x495595B: start_thread (pthread_create.c:447)

==24611==    by 0x49BB8DB: thread_start (clone.S:79)

==24611== 

==24611== This conflicts with a previous write of size 8 by thread #2

==24611== Locks held: 1, at address 0x4A8E490

==24611==    at 0x1098FC: left_rotate (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x109763: insert_fixup (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x1091D7: rb_tree_insert (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x109587: rb_tree_int_insert (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108D03: stress_test_worker (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x4892B93: ??? (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x495595B: start_thread (pthread_create.c:447)

==24611==    by 0x49BB8DB: thread_start (clone.S:79)

==24611==  Address 0x4a8e480 is 0 bytes inside a block of size 80 alloc'd

==24611==    at 0x48873E0: malloc (in /usr/libexec/valgrind/vgpreload_helgrind-arm64-linux.so)

==24611==    by 0x108EAF: rb_tree_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x1094E3: rb_tree_int_create (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108D87: test_multithreaded_insertion (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==    by 0x108E93: main (in /home/ubuntu/project/Threaded-Text-Analyzer/build/bin/test_rb_tree_multi_thread)

==24611==  Block was alloc'd by thread #1

==24611== 

Running multithreaded stress test ( 10 threads)... PASSED

==24611== 

==24611== Use --history-level=approx or =none to gain increased speed, at

==24611== the cost of reduced accuracy of conflicting-access information

==24611== For lists of detected and suppressed errors, rerun with: -s

==24611== ERROR SUMMARY: 9 errors from 1 contexts (suppressed: 54077 from 13)