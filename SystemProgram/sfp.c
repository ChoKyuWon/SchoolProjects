#include "sfp.h"
#include<stdlib.h>
#define TMax 0x7FFFFFFF
#define TMin 0x80000000
// 9 bit for frac, 6 bit for exp

union result {
	int res_i;
	float res_f;
};
sfp bits2sfp(char*);
int bits2int(char*, int, int);
void setNaN(sfp*);
void setINF(sfp*, int);
sfp makesfp(int sign, int exp, char* frac);

sfp int2sfp(int input){
	int sign = 0, exp = 0;
	sfp frac = 0;
	char *frac_c;
	int i;
	if (input < 0) {
		sign = 1;
		input = ~input;
		input++;
	}
	if (input == 0)
		return 0;

	sfp val=0;
	int tmp = input;
	while (tmp) {
		tmp = tmp >> 1;
		exp++;
	}
	tmp = input;
	exp -= 1;
	if (exp > 9) {
		tmp = tmp >> (exp - 9);
	}
	frac = tmp;
	if (frac == 0)
		return 1;
	frac_c = sfp2bits(frac);
	for (i = 0; i < 16; i++) {
		if (frac_c[i] == '1')
			break;
	}
	exp += 31;
	val = makesfp(sign, exp, frac_c + i + 1);
	free(frac_c);
	return val;
}

int sfp2int(sfp input){
	char *bits;
	bits = sfp2bits(input);
	int res = 0;
	int exp = 0;
	int frac = 0;
	int sign = 1;
	if (bits[0] == '1')
		sign = -1;

	exp = bits2int(bits, 1, 6);
	frac = bits2int(bits, 7, 15);
	if (exp == 63) {
		if (frac == 0) {
			if (sign == 0)
				return TMax;
			else
				return TMin;
		}
		else
			return TMin;
	}
	if (exp == 0) {
		return 0;
	}
	else {
		exp -= 31;
		bits[6] = '1';
		if (exp <= 9)
			res = bits2int(bits, 6, 6 + exp);
		else {
			frac |= 0x200;
			res = frac << (exp - 9);
		}
	}
	res *= sign;
	free(bits);
	return res;
}

sfp float2sfp(float input){
	char val[32];
	int i = 0;
	union result tmp;
	sfp res = 0;
	int exp = 0, frac = 0, sign = 0;
	tmp.res_f = input;

	if (tmp.res_f < 0) {
		sign = 1;
		tmp.res_i &= 0x7FFFFFFF;
	}
	for (i = 0; i < 32; i++) {
		val[i] = '0';
	}
	for (i = 31; i >= 0; i--) {
		if (tmp.res_i % 2 == 1) {
			val[i] = '1';
		}
		tmp.res_i = tmp.res_i >> 1;
	}
	for (i = 1; i < 9; i++) {
		exp += val[i] - 48;
		if (i == 8)
			break;
		exp = exp << 1;
	}
	if (exp == 0) {
		if (frac == 0)return 0;
		else
			res = makesfp(sign, 0, &val[9]);
	}
	else {
		exp -= 127;
		exp += 31;
	}
	res = makesfp(sign, exp, &val[9]);
	return res;
}

float sfp2float(sfp input){
	union result res;
	res.res_i = 0;
	char *bits = sfp2bits(input);
	int exp, frac;
	exp = bits2int(bits, 1, 6);
	frac = bits2int(bits, 7, 15);
	if (exp != 0) {
		exp += 96;
		exp = exp << 23;
	}
	frac = frac << 14;
	res.res_i |= exp;
	res.res_i |= frac;
	if (bits[0] == '1')
		res.res_i = res.res_i | 0x80000000;
	return res.res_f;
}

sfp sfp_add(sfp a, sfp b){
	sfp res = 0;
	int i;
	char *s1, *s2;
	s1 = sfp2bits(a);
	s2 = sfp2bits(b);
	int sign_a = 0, sign_b = 0;
	if (a >= 0x8000) {
		sign_a = 1;
	}
	if (b >= 0x8000) {
		sign_b = 1;
	}
	int exp_a, exp_b, frac_a, frac_b;
	int sign = 0, exp, frac;
	char inf_a = 0, inf_b = 0, nan_a = 0, nan_b = 0;
	exp_a = bits2int(s1, 1, 6);
	exp_b = bits2int(s2, 1, 6);
	frac_a = bits2int(s1, 7, 15);
	frac_b = bits2int(s2, 7, 15);

	if (exp_a == 0x3F) {
		if (frac_a == 0)
			inf_a = 1;
		else {
			setNaN(&res);
			return res;
		}
	}
	if (exp_b == 0x3F) {
		if (frac_b == 0) {
			inf_b = 1;
			if (inf_a == 1) {
				if (sign_a != sign_b) {
					setNaN(&res);
					return res;
				}
				else {
					setINF(&res, sign_a);
					return res;
				}
			}
			else {
				setINF(&res, sign_b);
				return res;
			}
		}
		else {
			setNaN(&res);
			return res;
		}
	}
	else {
		if (inf_a == 1) {
			setINF(&res, sign_b);
			return res;
		}
	}

	if (exp_a == 0) {
		exp_a = -30;
	}
	else {
		exp_a -= 31;
		frac_a |= 0x200;
		frac_a <<= 1;
	}
	if (exp_b == 0) {
		exp_b = -30;
	}
	else {
		exp_b -= 31;
		frac_b |= 0x200;
		frac_b <<= 1;
	}
	if (exp_a > exp_b) {
		frac_b >>= (exp_a - exp_b);
		exp = exp_a;
	}
	else {
		frac_a >>= (exp_b - exp_a);
		exp = exp_b;
	}
	exp += 31;
	if (sign_a == 1)
		frac_a = ~frac_a + 1;
	if (sign_b == 1)
		frac_b = ~frac_b + 1;
	frac = frac_a + frac_b;
	if (frac > 0x80000000) {
		frac = ~frac + 1;
		sign = 1;
	}
	if (frac % 2 == 1) {
		int tmp = 2;
		int flag = 0;
		flag = tmp & frac;
		if (flag != 0) {
			frac += 1;
		}
	}
	char frac_c[13];
	for (i = 0; i < 11; i++) {
		frac_c[i] = '0';
	}
	for (i = 0; i < 13; i++) {
		int tmp = 0x1000;
		int flag;
		tmp >>= i;
		flag = frac & tmp;
		if (flag > 0)
			frac_c[i] = '1';
		else
			frac_c[i] = '0';
	}
	if (frac_c[0] == '1') {
		exp += 2;
		res = makesfp(sign, exp, &frac_c[1]);
	}
	else if (frac_c[1] == '1') {
		exp += 1;
		res = makesfp(sign, exp, &frac_c[2]);
	}
	else {
		res = makesfp(sign, exp, &frac_c[3]);
	}
	free(s1);
	free(s2);
	return res;
}

char* sfp2bits(sfp result){
	char *val;
	val = (char*)malloc(sizeof(char) * 17);
	int i = 0;
	for (i = 0; i < 17; i++) {
		val[i] = '0';
	}
	for (i = 15; i >= 0; i--) {
		if (result % 2 == 1) {
			val[i] = '1';
		}
		result = result >> 1;
	}
	val[16] = 0;
	/*sfp test = 0;
	test = bits2sfp(val);*/
	return val;
}

sfp bits2sfp(char* bits) {
	int i = 0;
	sfp ret = 0;
	for (i = 0; i < 16; i++) {
		if (bits[i] == '1') {
			ret+=1;
		}
		if (i == 15)break;
		ret = ret << 1;
	}
	return ret;
}

int bits2int(char* bits, int start, int end) {
	int i, res = 0;
	for (i = start; i <= end; i++) {
		if (bits[i] == '1') {
			res++;
		}
		if (i == end)break;
		res = res << 1;
	}
	return res;
}

sfp makesfp(int sign, int exp, char* frac) {
	int i = 0;
	sfp res = 0;
	char bits[16] = { 0, };
	bits[0] = sign + 48;
	for (i = 6; i >= 1; i--) {
		bits[i] = exp % 2 + 48;
		exp /= 2;
	}
	for (i = 7; i <16; i++) {
		bits[i] = frac[i-7];
	}
	res = bits2sfp(bits);
	return res;
}
void setNaN(sfp* input) {
	char bits[9] = { 0, };
	bits[0] = '1';
	*input = makesfp(0, 0x3F, bits);
}
void setINF(sfp* input, int sign) {
	char bits[9] = { 0, };
	*input = makesfp(sign, 0x3F, bits);
}