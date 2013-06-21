; This tests that the type merger works

.class public TypeMergeTest
.super java/lang/Object

.method public static main()V
.limit stack 2
.limit locals 2

iconst_0
ifeq Else
	iconst_0
	istore_1
	goto Complete
Else:
	fconst_0
	fstore_1
Complete:
return

.end method
