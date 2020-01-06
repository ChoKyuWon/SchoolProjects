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

	x: .double 2.0



.text


main:
	addi	$sp, $sp, -4
	sw	$ra, 0($sp)

	la	$a0, arr
	la	$a1, dot

	jal	getPathLengthData

	lw	$ra, 0($sp)
	jr	$ra




#-------레지스터 표---------------------------------
# $a0 = arr[7][7] (점 사이 거리) (double형)
# $a1 = dot[7][2] (점 좌표)	(int형)


# $s0 = i
# $s1 = j
# $s2 = i or j를 주소 접근하는데 쓰일 값
# $s3 = dot의 임시 주소 값


# $t0 = 7 (상수)
# $t1 = dot[i][0]
# $t2 = dot[i][1]

# $t3 = dot[j][0]
# $t4 = dot[j][1]

# $f0 = sqrt의 반환값 (double형)
#----------------------------------------------------




getPathLengthData:
	addi	$sp, $sp, -12
	sw	$ra, 0($sp)			# return address 저장
	sw	$a0, 4($sp)			# $a0 저장
	sw	$a1, 8($sp)			# $a1 저장

	li	$s0, 0				# i = 0 초기화
	li	$t0, 7				# 7이라는 값을 $t0 레지스터에 할당


Loop1:
	li	$s1, 0				# j = 0 초기화
	
	
Loop2:

	#-----i 계산------------

	la	$s3, 0($a1)			# $s3 = $a1의 주소(dot의 주소)

	sll	$s2, $s0, 3			# $s2 = i * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (i * 8)

	lw	$t1, 0($s3)			# $t1 = dot[i][0]
	lw	$t2, 4($s3)			# $t2 = dot[i][1]


	#-----j 계산------------

	la	$s3, 0($a1)			# $s3 = $a1의 주소(dot의 주소)

	sll	$s2, $s1, 3			# $s2 = j * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (j * 8)

	lw	$t3, 0($s3)			# $t3 = dot[j][0]
	lw	$t4, 4($s3)			# $t4 = dot[j][1]

	#-----x, y길이 계산-----

	sub	$t3, $t3, $t1			# x_length = dot[j][0] - dot[i][0] (x2 - x1)
	sub	$t4, $t4, $t2			# y_length = dot[j][1] - dot[i][1] (y2 - y1)

	mul	$t1, $t3, $t3			# $t1 = x_length * x_length
	mul	$t2, $t4, $t4			# $t2 = y_length * y_length

	add	$t1, $t1, $t2			# $t1 = $t1 + $t2   ($t1 = temp)

	#-----sqrt 함수 call----

	addi	$sp, $sp, -4
	sw	$a0, 0($sp)			# 스택에 $a0주소 저장(arr 주소)

	add	$a0, $zero, $t1			# 함수 인자로 $t1(temp)
	jal	sqrt				# call sqrt(temp)

	lw	$a0, 0($sp)			# $a0에 다시 arr 주소 불러옴
	addi	$sp, $sp, 4		

	#-----arr[i][j] = sqrt의 결과값----
	
	li	$t1, 56				# $t1 = 8 * 7
	mul	$t2, $s0, $t1			# $t2 = i * 8 * 7
	
	sll	$t1, $s1, 3			# $t1 = j * 8

	la	$s3, 0($a0)			# $s3 = $a0의 주소(arr의 주소)

	add	$t3, $t2, $t1			# $t3 = (i * 8 * 7) + (j * 8)
	add	$s3, $s3, $t3			# $s3 = arr + (i * 8 * 7) + (j * 8)
	
	s.d	$f0, 0($s3)			# arr[i][j] = sqrt의 결과 값

	#-----for문-------------


	addi	$s1, $s1, 1			# j++
	beq	$s1, $t0, Loop_End		# j가 7되면 Loop_End로
	j	Loop2

Loop_End:

	addi	$s0, $s0, 1			# i++
	bne	$s0, $t0, Loop1			# i가 7되면 for문 끝

	lw	$ra, 0($sp)			# return address 복구
	lw	$a0, 4($sp)			# $a0 복구
	lw	$a1, 8($sp)			# $a1 복구
	addi	$sp, $sp, 12

	jr	$ra				# 원 함수로

#----------일단 2.0값이 다 나오도록 설정함----------------
sqrt:
	li.d	$f0, 2.0
	jr	$ra