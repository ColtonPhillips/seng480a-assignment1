; This is a test to potential stack underflows are caught

.class public StackUnderflowTest
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 2
  .limit locals 1

	pop
  return

.end method
