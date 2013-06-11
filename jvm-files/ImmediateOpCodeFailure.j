; This is a test to ensure that immediate op codes will fail if we do not have enough locals

.class public ImmediateOpCodeFailure
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 2
  .limit locals 0

	iload_1	
  return

.end method
