.data

arr: .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
        .double 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0
dot: .word 0, 0
         .word 2, 6
         .word 8, 4
         .word 7, 2
         .word 1, 6
         .word 4, 9
         .word 3, 2    
course: .word 0, 0, 0, 0, 0, 0
double_max: .double 999999.0



# d1 ~ d6 = s1 ~ s6
# f0 = dist_min, f2=dist
# f4, f6, f8, f10, f12 = dist1 ~ dist5

.text
main:
l.d $f0, double_max
# GetPathLenthData funciton excution
la $a0, arr
la $a1, dot
jal getPathLengthData

# for loop1
li $s1, 0
loop1:
addi $s1, $s1, 1
bge $s1, 7, exit_loop
sll $t0, $s1, 3
l.d $f2, arr($t0)                 # dist = arr[0][d1]
mov.d $f4, $f2                # dist1 = dist
c.le.d $f0, $f2 
bc1t loop1                    # if (dist > dist_min)    continue;	
# for loop2
li $s2, 0
loop2:
addi $s2, $s2, 1
beq $s2, $s1, loop2
bge $s2, 7, loop1
sll $t0, $s1, 3
sub $t0, $t0, $s1
add $t0, $t0, $s2
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d1][d2]
add.d $f2, $f2, $f4           # dist = dist + dist1
mov.d $f6, $f2                # dist2 = dist
c.le.d $f0, $f2 
bc1t loop2                    # if (dist > dist_min)    continue;
# for loop3
li $s3, 0
loop3:
addi $s3, $s3, 1
beq $s3, $s1, loop3
beq $s3, $s2, loop3
bge $s3, 7, loop2
sll $t0, $s2, 3
sub $t0, $t0, $s2
add $t0, $t0, $s3
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d2][d3]
add.d $f2, $f2, $f6           # dist = dist + dist2
mov.d $f8, $f2                # dist3 = dist
c.le.d $f0, $f2 
bc1t loop3                    # if (dist > dist_min)    continue;
# for loop4
li $s4, 0
loop4:
addi $s4, $s4, 1
beq $s4, $s1, loop4
beq $s4, $s2, loop4
beq $s4, $s3, loop4
bge $s4, 7, loop3
sll $t0, $s3, 3
sub $t0, $t0, $s3
add $t0, $t0, $s4
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d3][d4]
add.d $f2, $f2, $f8           # dist = dist + dist3
mov.d $f10, $f2               # dist4 = dist
c.le.d $f0, $f2 
bc1t loop4                    # if (dist > dist_min)    continue;
# for loop5
li $s5, 0
loop5:
addi $s5, $s5, 1
beq $s5, $s1, loop5
beq $s5, $s2, loop5
beq $s5, $s3, loop5
beq $s5, $s4, loop5
bge $s5, 7, loop4
sll $t0, $s4, 3
sub $t0, $t0, $s4
add $t0, $t0, $s5
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d4][d5]
add.d $f2, $f2, $f10          # dist = dist + dist4
mov.d $f12, $f2               # dist5 = dist
c.le.d $f0, $f2 
bc1t loop5                    # if (dist > dist_min)    continue;
# for loop6
li $s6, 0
loop6:
addi $s6, $s6, 1
beq $s6, $s1, loop6
beq $s6, $s2, loop6
beq $s6, $s3, loop6
beq $s6, $s4, loop6
beq $s6, $s5, loop6
bge $s6, 7, loop5
sll $t0, $s5, 3
sub $t0, $t0, $s5
add $t0, $t0, $s6
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d5][d6]
add.d $f2, $f2, $f12          # dist = dist + dist4
sll $t0, $s6, 3
l.d $f14, arr($t0)       
add.d $f2, $f2, $f14          # dist = dist + arr[d6][0]
c.le.d $f0, $f2
bc1t loop6                    # if (dist >= dist_min)    continue;
la $t0, course
sw $s1, 0($t0)
sw $s2, 4($t0)
sw $s3, 8($t0)
sw $s4, 12($t0)
sw $s5, 16($t0)
sw $s6, 20($t0)
mov.d $f0, $f2
j loop6
exit_loop:
la $v0, 1
la $a0, 1
syscall
la $t0, course
lw $a0, 0($t0)
addi $a0, $a0, 1
syscall
lw $a0, 4($t0)
addi $a0, $a0, 1
syscall
lw $a0, 8($t0)
addi $a0, $a0, 1
syscall
lw $a0, 12($t0)
addi $a0, $a0, 1
syscall
lw $a0, 16($t0)
addi $a0, $a0, 1
syscall
lw $a0, 20($t0)
addi $a0, $a0, 1
syscall
la $v0, 1
la $a0, 1
syscall
la $v0, 11
la $a0, 10
syscall
la $v0, 3
mov.d $f12, $f0
syscall
la $v0, 10
syscall