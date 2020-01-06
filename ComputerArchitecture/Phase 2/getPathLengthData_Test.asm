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




#-------�������� ǥ---------------------------------
# $a0 = arr[7][7] (�� ���� �Ÿ�) (double��)
# $a1 = dot[7][2] (�� ��ǥ)	(int��)


# $s0 = i
# $s1 = j
# $s2 = i or j�� �ּ� �����ϴµ� ���� ��
# $s3 = dot�� �ӽ� �ּ� ��


# $t0 = 7 (���)
# $t1 = dot[i][0]
# $t2 = dot[i][1]

# $t3 = dot[j][0]
# $t4 = dot[j][1]

# $f0 = sqrt�� ��ȯ�� (double��)
#----------------------------------------------------




getPathLengthData:
	addi	$sp, $sp, -12
	sw	$ra, 0($sp)			# return address ����
	sw	$a0, 4($sp)			# $a0 ����
	sw	$a1, 8($sp)			# $a1 ����

	li	$s0, 0				# i = 0 �ʱ�ȭ
	li	$t0, 7				# 7�̶�� ���� $t0 �������Ϳ� �Ҵ�


Loop1:
	li	$s1, 0				# j = 0 �ʱ�ȭ
	
	
Loop2:

	#-----i ���------------

	la	$s3, 0($a1)			# $s3 = $a1�� �ּ�(dot�� �ּ�)

	sll	$s2, $s0, 3			# $s2 = i * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (i * 8)

	lw	$t1, 0($s3)			# $t1 = dot[i][0]
	lw	$t2, 4($s3)			# $t2 = dot[i][1]


	#-----j ���------------

	la	$s3, 0($a1)			# $s3 = $a1�� �ּ�(dot�� �ּ�)

	sll	$s2, $s1, 3			# $s2 = j * 8
	add	$s3, $s3, $s2			# $s3 = $s3 + (j * 8)

	lw	$t3, 0($s3)			# $t3 = dot[j][0]
	lw	$t4, 4($s3)			# $t4 = dot[j][1]

	#-----x, y���� ���-----

	sub	$t3, $t3, $t1			# x_length = dot[j][0] - dot[i][0] (x2 - x1)
	sub	$t4, $t4, $t2			# y_length = dot[j][1] - dot[i][1] (y2 - y1)

	mul	$t1, $t3, $t3			# $t1 = x_length * x_length
	mul	$t2, $t4, $t4			# $t2 = y_length * y_length

	add	$t1, $t1, $t2			# $t1 = $t1 + $t2   ($t1 = temp)

	#-----sqrt �Լ� call----

	addi	$sp, $sp, -4
	sw	$a0, 0($sp)			# ���ÿ� $a0�ּ� ����(arr �ּ�)

	add	$a0, $zero, $t1			# �Լ� ���ڷ� $t1(temp)
	jal	sqrt				# call sqrt(temp)

	lw	$a0, 0($sp)			# $a0�� �ٽ� arr �ּ� �ҷ���
	addi	$sp, $sp, 4		

	#-----arr[i][j] = sqrt�� �����----
	
	li	$t1, 56				# $t1 = 8 * 7
	mul	$t2, $s0, $t1			# $t2 = i * 8 * 7
	
	sll	$t1, $s1, 3			# $t1 = j * 8

	la	$s3, 0($a0)			# $s3 = $a0�� �ּ�(arr�� �ּ�)

	add	$t3, $t2, $t1			# $t3 = (i * 8 * 7) + (j * 8)
	add	$s3, $s3, $t3			# $s3 = arr + (i * 8 * 7) + (j * 8)
	
	s.d	$f0, 0($s3)			# arr[i][j] = sqrt�� ��� ��

	#-----for��-------------


	addi	$s1, $s1, 1			# j++
	beq	$s1, $t0, Loop_End		# j�� 7�Ǹ� Loop_End��
	j	Loop2

Loop_End:

	addi	$s0, $s0, 1			# i++
	bne	$s0, $t0, Loop1			# i�� 7�Ǹ� for�� ��

	lw	$ra, 0($sp)			# return address ����
	lw	$a0, 4($sp)			# $a0 ����
	lw	$a1, 8($sp)			# $a1 ����
	addi	$sp, $sp, 12

	jr	$ra				# �� �Լ���

#----------�ϴ� 2.0���� �� �������� ������----------------
sqrt:
	li.d	$f0, 2.0
	jr	$ra