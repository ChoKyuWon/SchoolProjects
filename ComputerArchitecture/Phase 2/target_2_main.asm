.data:
	dot:	.word 0 0
		.word 0 0
		.word 2 6 
		.word 8 4
		.word 7 2
		.word 1 6
		.word 4 9
		.word 3 2

	arr:	.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0
		.double 0 0 0 0 0 0 0 0

	dist:	.double 0
	min:	.double 99999
	r:		.word 1 0 0 0 0 0 7
	save_root:	.word 0 0 0 0 0 0 0

.text
sqrt:
li.d $f2, 1.0  #f2=x=1, x=float
add $s1, $zero, $zero   #s1=i=0

loop_sqrt:
div.d $f4, $a0, $f2     #f4=input/x          
add.d $f4, $f4, $a0     #f4= x+input/x
srl $f2, $f4, 0x1       #x=f4/2
addi $s1, $s1, 0x1      #i++
bne $s1, 10, loop_sqrt  #if i!=10, loop

add $f0, $zero, $s1     #f0=x
jr $ra                  #return


#---------------------------------------
# $a0 = arr[7][7]
# $a1 = dot[7][2]


# $s0 = i
# $s1 = j
# $s2 = i or j
# $s3 = dot


# $t0 = 7
# $t1 = dot[i][0]
# $t2 = dot[i][1]

# $t3 = dot[j][0]
# $t4 = dot[j][1]

# $f0 = sqrt (double)
#----------------------------------------------------




getPathLengthData:
	addi	$sp, $sp, -12
	sw	$ra, 0($sp)			# return address 
	sw	$a0, 4($sp)			# $a0 
	sw	$a1, 8($sp)			# $a1

	li	$s0, 0				# i
	li	$t0, 7				# 


Loop1:
	li	$s1, 0				#
	
Loop2:

	#-----i-----------

	la	$s3, 0($a1)			# $s3 = $a1

	sla	$s2, $s0, 3			# $s2 = i * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (i * 8)

	lw	$t1, 0($s3)			# $t1 = dot[i][0]
	lw	$t2, 4($s3)			# $t2 = dot[i][1]


	#-----j-----------

	la	$s3, 0($a1)			# $s3 = $a1

	sla	$s2, $s1, 3			# $s2 = j * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (j * 8)

	lw	$t3, 0($s3)			# $t3 = dot[j][0]
	lw	$t4, 4($s3)			# $t4 = dot[j][1]

	#-----x, y----

	sub	$t3, $t3, $t1			# x_length = dot[j][0] - dot[i][0] (x2 - x1)
	sub	$t4, $t4, $t2			# y_length = dot[j][1] - dot[i][1] (y2 - y1)

	mul	$t1, $t3, $t3			# $t1 = x_length * x_length
	mul	$t2, $t4, $t4			# $t2 = y_length * y_length

	add	$t1, $t1, $t2			# $t1 = $t1 + $t2   ($t1 = temp)

	#-----sqrt call----

	addi	$sp, $sp, -4
	sw	$a0, 0($sp)			#

	add	$a0, $zero, $t1			# $t1(temp)
	jal	sqrt				# call sqrt(temp)

	lw	$a0, 0($sp)			# $a0
	addi	$sp, $sp, 4		

	#-----arr[i][j] = sqrt----
	
	li	$t1, 56				# $t1 = 8 * 7
	mul	$t2, $s0, $t1			# $t2 = i * 8 * 7
	
	sla	$t1, $s1, 3			# $t1 = j * 8

	la	$s3, 0($a0)			# $s3 = $a0

	add	$s3, $t2, $t1			# $s3 = arr + (i * 8 * 7) + (j * 8)
	
	sw.d	$f0, 0($s3)			# arr[i][j] = sqrt

	#-----for-------------


	addi	$s1, $s1, 1			# j++
	beq	$s1, $t0, Loop1			# 
	j	Loop2

Loop_End:
	addi	$s0, $s0, 1			# i++
	bne	$s0, $t0, Loop1			# 

	lw	$ra, 0($sp)			# return address
	lw	$a0, 4($sp)			# $a0
	lw	$a1, 8($sp)			# $a1 
	addi	$sp, $sp, 12

	jr	$ra				#

main:
la $a0, arr
la $a1, dot
j getPathLengthData


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
addi $t0, $zero, 0x2	#t0=a, a=2
l.s $s0, dist			#s0=dist
la $s1, r				#s1=r
add.d $s0, $zero, $zero	#dist=0
sw $t0, 0x4($s1)		#r[1]=a
la $s2, arr				#s2=arr

.loop2
addi $t1, $zero, 0x2



	for (int a = 2; a <= 6; a++){
		dist = 0;
		r[1] = a;
		dist = arr[r[0]][r[1]];
		//depth 2;
		for (int b = 2; b <= 6; b++){
			r[2] = b	;
	if (a == b){
				continue;
			}
			dist = arr[r[0]][r[1]] + arr[r[1]][r[2]];
			if (dist > min) continue;
			//depth 3
			for (int c = 2; c <= 6; c++){
				r[3] = c;
				if (a == c || b == c){
					continue;
				}
				dist = arr[r[0]][r[1]] + arr[r[1]][r[2]] + arr[r[2]][r[3]];
				if (dist > min) continue;
				//depth 4
				for (int d = 2; d <= 6; d++){
					r[4] = d;
					if (a == d || b == d || c == d){
						continue;
					}
					dist = arr[r[0]][r[1]] + arr[r[1]][r[2]] + arr[r[2]][r[3]] + arr[r[3]][r[4]];
					if (dist > min) continue;
					//depth 5
					for (int e = 2; e <= 6; e++){
						r[5] = e;
						if (a == e || b == e || c == e|| d == e){
							continue;
						}
						dist = arr[r[0]][r[1]] + arr[r[1]][r[2]] + arr[r[2]][r[3]] + arr[r[3]][r[4]] + arr[r[4]][r[5]] + arr[r[5]][r[6]];
						if (dist < min){
							for (int i = 0; i < 7; i++){
								save_route[i] = r[i];
								min = dist;
							}
						}
					}
				}
			}
		}
	}
	for (int i = 0; i < 7; i++) {
		printf("%d ",save_route[i]);
	}
	printf("\n%f", min);
}
