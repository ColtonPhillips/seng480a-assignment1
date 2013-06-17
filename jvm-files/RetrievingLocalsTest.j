; This tests that storing one type into a local and then trying to use it as another fails

.class public RetrievingLocalsTest
.super java/lang/Object

.method public static main([Ljava/lang/String;)V
.limit stack 2
.limit locals 2

; First, store an int in v1
iconst_0
istore_1

; Then, try to retrieve its value as a float
fload_1

return

.end method
