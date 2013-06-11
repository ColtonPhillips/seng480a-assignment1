; This is a test to potential stack overflows are caught

.class public StackOverflowTest
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 0
  .limit locals 1

	iconst_0
  return

.end method
