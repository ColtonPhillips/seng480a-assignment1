; This tests that the stack height is the same in all branches

.class public BranchHeightTest
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
  .limit stack 2
  .limit locals 2

	iconst_0
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	ifeq Fallthrough
		iconst_1
Fallthrough:
	iconst_2
  return

.end method
