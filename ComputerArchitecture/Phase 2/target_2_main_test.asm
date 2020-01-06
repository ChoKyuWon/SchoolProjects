.data
	dot:.word 0, 0
		.word 2, 6 
		.word 8, 4
		.word 7, 2
		.word 1, 6
		.word 4, 9
		.word 3, 2

arr: 	.double 0.000977, 6.324555, 8.944272, 7.280110, 6.082763, 9.848858, 3.605551
        .double 6.324555, 0.000977, 6.324555, 6.403124, 1.000000, 3.605551, 4.123106
        .double 8.944272, 6.324555, 0.000977, 2.236068, 7.280110, 6.403124, 5.385165
        .double 7.280110, 6.403124, 2.236068, 0.000977, 7.211102, 7.615773, 4.000000
        .double 6.082763, 1.000000, 7.280110, 7.211102, 0.000977, 4.242640, 4.472136
        .double 9.848858, 3.605551, 6.403124, 7.615773, 4.242640, 0.000977, 7.071068
        .double 3.605551, 4.123106, 5.385165, 4.000000, 4.472136, 7.071068, 0.000977
		#plz erase and put 0.0 on real_test

	r:		.word 0, 0, 0, 0, 0
	save_root:	.word 0, 0, 0, 0, 0, 0, 0
	double_max: .double 999999.0

.text
#sqrt:
#getPathLengthData:

main:
#la $a0, arr
#la $a1, dot
#j getPathLengthData
# erase 3 # above

#s1=d1
#s2=d2
#s3=d3
#s4=d4
#s5=d5

li $s0, 0         	  # s0<=dist  (value of 0)
l.d $f0, double_max

# for loop1
li $s1, 0
loop1:
addi $s1, $s1, 1
bge $s1, 6, exit_loop
sll $t0, $s1, 3
l.d $f2, arr($t0)           # dist = arr[0][d1]
mov.d $f4, $f2	     # dist1 = dist
c.le.d $f0, $f2
bc1t loop1		     # if (dist > dist_min) continue

# for loop2
li $s2, 0
loop2:
addi $s2, $s2, 1
bge $s2, 6, loop1
beq $s1, $s2, loop2     # continue
sll $t0, $s1, 3
sub $t0, $t0, $s1	#calculate row
add $t0, $t0, $s2
sll $t0, $t0, 3
l.d $f2, arr($t0)	#dist = arr[d1][d2]
add.d $f2, $f2, $f4	#dist = dist + dist1
mov.d $f6, $f2	#dist2 = dist
c.le.d $f0, $f2
bc1t loop2	#if (dist > dist_min) contiunue;

# for loop3
li $s3, 0
loop3:
addi $s3, $s3, 1
bge $s3, 6, loop2
beq $s3, $s1, loop3
beq $s3, $s2, loop3
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
bge $s4, 6, loop3
beq $s4, $s1, loop4
beq $s4, $s2, loop4
beq $s4, $s3, loop4
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
bge $s5, 6, loop4
beq $s5, $s1, loop5
beq $s5, $s2, loop5
beq $s5, $s3, loop5
beq $s5, $s4, loop5
sll $t0, $s4, 3
sub $t0, $t0, $s4
add $t0, $t0, $s5
sll $t0, $t0, 3
l.d $f2, arr($t0)                 # dist = arr[d4][d5]
add.d $f2, $f2, $f10          # dist = dist + dist4
mov.d $f12, $f2               # dist5 = dist
sll $t0, $s5, 3
sub $t0, $t0, $s5
addi $t0, $t0, 6
sll $t0, $t0, 3
l.d $f2, arr($t0)			  # dist = arr[d5][6]
add.d $f2, $f12, $f2		  # dist = dist + dist5
c.le.d $f0, $f2
bc1t loop5                    # if (dist > dist_min)    continue;
sw $s1, r
sw $s2, r+4
sw $s3, r+8
sw $s4, r+12
sw $s5, r+16
mov.d $f0, $f2
j loop5

exit_loop:
la $v0, 1
la $a0, 1
syscall
lw $a0, r
addi $a0, 1
syscall
lw $a0, r+4
addi $a0, 1
syscall
lw $a0, r+8
addi $a0, 1
syscall
lw $a0, r+12
addi $a0, 1
syscall
lw $a0, r+16
addi $a0, 1
syscall
la $v0, 1
la $a0, 7
syscall
la $v0, 11
la $a0, 10
syscall
la $v0, 3
mov.d $f12, $f0
syscall
la $v0, 10
syscall