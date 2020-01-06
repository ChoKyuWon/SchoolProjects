.data
    zero:   .double 0.0
    x:      .double 1.0
    two:    .double 2.0
    test:   .double 4.0
.text

#register tale---------------
#   $a0 paramater
#   $s1 = i , loop counter
#   $f2 = x
#   $f4 = tmp
#   $f6 = 2(double type of 2)
#   $f12 = input
#----------------------------

main:
addi $a0, $zero, 0x4
jal sqrt
addi $v0,$zero,0x3
syscall

sqrt:
mtc1 $a0, $f12
cvt.d.w $f12, $f12
l.d $f12,test
l.d $f2, x
add $s1, $zero, $zero   #s1=i=0
l.d $f6, two

loop_sqrt:
div.d $f4, $f12, $f2    #f4=input/x          
add.d $f4, $f4, $f2     #f4= x+input/x
div.d $f2, $f4, $f6    #x=f4/2
addi $s1, $s1, 0x1      #i++
bne $s1, 10, loop_sqrt  #if i!=10, loop

#add.d $f12, $f8, 0.0
s.d $f2, x
l.d $f12, x
jr $ra                  #return