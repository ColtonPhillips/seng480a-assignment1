; This tests that types are checked when popping values off the stack

.class public PopTypeTest
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 1
  .limit locals 1

	iconst_0
	fstore_0
  return

.end method
