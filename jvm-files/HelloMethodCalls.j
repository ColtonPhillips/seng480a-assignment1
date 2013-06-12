; This is a test to potential stack overflows are caught

.class public HelloMethodCalls
.super java/lang/Object

.method public static doFoo(I)I
	.limit stack 2
	.limit locals 2
	iload_0
	iconst_1
	iadd
	ireturn
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 2
	.limit locals 2
	iconst_2
	invokestatic	HelloMethodCalls/doFoo(I)I
	pop
	return
.end method
