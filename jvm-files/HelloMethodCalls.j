; This is a test to potential stack overflows are caught

.class public HelloMethodCalls
.super java/lang/Object

.method public static doFoo(II)I
	.limit stack 2
	.limit locals 2
	iconst_1
	iconst_0
	iadd
	ireturn
.end method

.method public static main([Ljava/lang/String;)V
	.limit stack 2
	.limit locals 3
	iconst_0
	iconst_1
	invokestatic	HelloMethodCalls/doFoo(II)I
	pop
	return
.end method
