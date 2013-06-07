-seng480a-assignment1
=====================

JVM Verification


README File for MyJVM
=====================

The C source code files in this directory implement a Java Virtual Machine
which supports only a subset of Java.

The limitations include:

1.  Gross inefficiency:
    (a) a simple interpreter loop with a switch statement is used.
    (b) a good JVM would translate the format of the bytecode for each
        method so that information does not have to be painfully extracted
        from the constant pool and so that operands are nicely aligned
        on word boundaries, as appropriate.
    (c) the opcodes which require class resolution (getstatic, getfield,
        invokespecial, etc.) are re-executed in full every time they are
        encountered.  The original Java interpreter from Sun would replace
        these ops with their 'quick' versions during execution.  (Now that
        the JVM JIT-compiles any method that gets executed more than a
        few times, the usefulness of the quick-opcodes has gone away.)

2.  Minimal checking.
    No access permissions are checked, no bytecode verification, ...
    Therefore illegal Java programs may execute without any errors
    being reported.

3.  No access to any Java class libraries (including java/lang/Object)
    or classes held in JAR files.
    Special cases:
    (a) The String type is supported for assignment, for
        printing via the print/println methods, and its
        length() and charAt() methods are supported.
    (b) The StringBuilder type is supported for these methods
            constructor:  StringBuilder()
            append(X):  with X having int, float, double, String
            toString()
        because the Java compiler uses these methods to implement
        string concatenations such as "val = " + val + "\n"  ;
    (c) The print and println methods of java/io/PrintStream are supported
        for printing of strings, ints, floats and doubles to standard
        output (i.e. via the System.out class variable).
    (d) The parseInt, parseFloat and parseDouble static methods of
        the Integer, Float and Double classes are supported so that
        command line arguments can be parsed.

4.  No exception handling.

5.  No threads.

6.  Several opcodes are unimplemented.
    These are:
        checkcast   (which is simply ignored)
        drem
        frem
        instanceof
        invokeinterface
        monitorenter
        monitorexit
        multianewarray
        wide
        breakpoint
    All except checkcast will cause the interpreter to halt.

7.  Most opcodes are untested.  Many bugs are undoubtedly present.

8.  No garbage collection is performed.  Any program which repeatedly creates
    new instances of classes will eventually run out of space in the Java heap.


---------

Author:

   Nigel Horspool, nigelh@uvic.ca

   May 2008; updated May 2010; updated May 2013.
