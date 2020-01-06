.data

arr: .double 0.000977, 6.324555, 8.944272, 7.280110, 6.082763, 9.848858, 3.605551
        .double 6.324555, 0.000977, 6.324555, 6.403124, 1.000000, 3.605551, 4.123106
        .double 8.944272, 6.324555, 0.000977, 2.236068, 7.280110, 6.403124, 5.385165
        .double 7.280110, 6.403124, 2.236068, 0.000977, 7.211102, 7.615773, 4.000000
        .double 6.082763, 1.000000, 7.280110, 7.211102, 0.000977, 4.242640, 4.472136
        .double 9.848858, 3.605551, 6.403124, 7.615773, 4.242640, 0.000977, 7.071068
        .double 3.605551, 4.123106, 5.385165, 4.000000, 4.472136, 7.071068, 0.000977

course: .word 0, 0, 0, 0, 0, 0
double_max: .double 999999.0

# d1 ~ d6 = s1 ~ s6
# f0 = dist_min, f2=dist
# f4, f6, f8, f10, f12 = dist1 ~ dist5

.text
main:
l.d $f0, double_max

# GetPathLenthData funciton excution

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