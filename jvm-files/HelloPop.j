; This is a check that pops work properly

.class public HelloPop
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 2
  .limit locals 2

	iconst_0
	pop
	iconst_0
	iconst_0
	pop2
  return

.end method
