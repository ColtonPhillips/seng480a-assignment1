; This is a check that swap work properly

.class public HelloSwap
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 2
  .limit locals 3

	iconst_0
	fconst_0
	swap
	istore_1
	fstore_2
  return

.end method
