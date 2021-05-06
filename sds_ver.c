#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gmp.h>
#include "elipt_cur.h"
#include "hash_lib.h"

//очистить все переменные, используемые gmp
void Clear_GMP (mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP, mpz_t xQ, mpz_t yQ)
{
	mpz_clear(p);
	mpz_clear(a);
	mpz_clear(b);
	mpz_clear(m);
	mpz_clear(q);
	mpz_clear(xP);
	mpz_clear(yP);
	mpz_clear(xQ);
	mpz_clear(yQ);
}

//получение параметров цифровой подписи
char GetParams (char* path, mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP)
{
	FILE* params;
	if ((params = fopen(path, "r")) == NULL)
	{
		printf("Error. File \"ds_params.sdsp\" not found. Generate or import parameters file\n");
		return -1;
	}
	mpz_inp_str (p, params, 16);
	mpz_inp_str (a, params, 16);
	mpz_inp_str (b, params, 16);
	mpz_inp_str (m, params, 16);
	mpz_inp_str (q, params, 16);
	mpz_inp_str (xP, params, 16);
	mpz_inp_str (yP, params, 16);
	gmp_printf("p = %Zx\n"
			"a = %Zx\n"
			"b = %Zx\n"
			"m = %Zx\n"
			"q = %Zx\n"
			"xP = %Zx\n"
			"yP = %Zx\n", p, a, b, m, q, xP, yP);
	fclose(params);
	return 0;
}

int GetUserKeys (mpz_t xQ, mpz_t yQ, int iteration, char* login)
{
	FILE *keys;
	char buffer[256];
	if ((keys = fopen("public_accounts.sdspa", "rb")) == NULL)
	{
		printf("Error reading accounts info");
		return -1;
	}
	if (feof(keys) == 0)
	{
		for (int i = 0; i<iteration; i++)
		{
			fgets(buffer, 256, keys);
			fgets(buffer, 256, keys);
			fgets(buffer, 256, keys);
		}
		if (feof(keys) != 0)
		{
			printf("End of file reached, no matches");
			return -1;
		}
		fgets(buffer, 256, keys);
		//узнать логин
	}
	else
	{
		printf("End of file reached, no matches");
		return -1;
	}
	
	for (int i=0; i<256; i++)
	{
		if (buffer[i] == 0x0A)
			buffer[i]=0;
		login[i]=buffer[i];
	}
	
	mpz_inp_str (xQ, keys, 16);
	mpz_inp_str (yQ, keys, 16);
	gmp_printf("xQ = %Zx\n"
			"yQ = %Zx\n", xQ, yQ);
	fclose(keys);
	return 0;
}

void GenerateHashFromFile(FILE *file, unsigned char *h)
{
	fseek(file, 0, SEEK_END);
	long fsize = ftell(file);
	fseek(file, 0, SEEK_SET);
	unsigned char *content = (unsigned char *)malloc(fsize + 1);
	fread(content, 1, fsize, file);
	
	h=hash256(content, fsize);
    printf("h: ");
    for (int i=0; i<32; i++)
        printf("%x ", h[i]);
	printf("\n");
	free(content);
}

void CheckDS(mpz_t p, mpz_t a, mpz_t b, mpz_t m, mpz_t q, mpz_t xP, mpz_t yP, mpz_t xQ, mpz_t yQ, FILE *file)
{
	char result = 0;
	int keysfstat = 0;
	int it = 0;
	char login[256];
	mpz_t e, alpha, nu, s, r, z1, z2, xC, yC, xC1, yC1, R;
	unsigned char *h;
	mpz_init(e);
	mpz_init(alpha);
	mpz_init(nu);
	mpz_init(s);
	mpz_init(r);
	mpz_init(z1);
	mpz_init(z2);
	mpz_init(xC);
	mpz_init(yC);
	mpz_init(R);
	while (result == 0)
	{
		keysfstat = GetUserKeys(xQ, yQ, it, login);
		if (keysfstat != 0)
		{
			return;
		}
		it++;
		/*mpz_set_str(xQ, "7F2B49E270DB6D90D8595BEC458B50C58585BA1D4E9B788F6689DBD8E56FD80B", 16);
		mpz_set_str(yQ, "26F1B489D6701DD185C8413A977B3CBBAF64D1C593D26627DFFB101A87FF77DA", 16);
		gmp_printf("xQ = %Zx\n"
				"yQ = %Zx\n", xQ, yQ);*/
		
		//получение хеш-кода
		/*GenerateHashFromFile(file, h);
		//получить альфа, число, двоичным представлением которого является h
		mpz_import(alpha, 32, 1, 1, 1, 0, h);
		gmp_printf("alpha = %Zx\n", alpha);
		//а ещё тут надо добавить получение s и r
		//получить e
		mpz_mod(e, alpha, q);*/
		mpz_set_str(e, "2DFBC1B372D89A1188C09C52E0EEC61FCE52032AB1022E8E67ECE6672B043EE5", 16);
		mpz_set_str(s, "1456C64BA4642A1653C235A98A60249BCD6D3F746B631DF928014F6C5BF9C40", 16);
		mpz_set_str(r, "41AA28D2F1AB148280CD9ED56FEDA41974053554A42767B83AD043FD39DC0493", 16);
		gmp_printf("r = %Zx\n"
				"s = %Zx\n", r, s);
		gmp_printf("e = %Zx\n", e);
		//получить ню
		Revers(q, e, nu);
		gmp_printf("nu = %Zx\n", nu);
		//z1=s*nu (mod q)
		mpz_mul(z1, s, nu);
		mpz_mod(z1, z1, q);	
		//z2=-r*nu (mod q)
		mpz_mul(z2, r, nu);
		mpz_neg(z2, z2);
		mpz_mod(z2, z2, q);
		gmp_printf("z1 = %Zx\n"
				"z2 = %Zx\n", z1, z2);

		//C=z1*P+z2*Q
		mpz_init(xC1);
		mpz_init(yC1);
		PointMul(p, a, xP, yP, z1, xC1, yC1);
		PointMul(p, a, xQ, yQ, z2, xC, yC);
		PointSum(p, a, xC1, yC1, xC, yC, xC, yC);
		mpz_clear(xC1);
		mpz_clear(yC1);
		gmp_printf("xC = %Zx\n"
				"yC = %Zx\n", xC, yC);

		mpz_mod(R, xC, q);
		if (mpz_cmp(R, r) == 0)
		{
			printf("You Win!\n");
			printf("Owner is %s\n", login);
			result = 1;
		}
		else
			printf("Next\n");
	}
	if (result == 0)
		printf("You lost!\n");
	mpz_clear(e);
	mpz_clear(alpha);
	mpz_clear(nu);
	mpz_clear(s);
	mpz_clear(r);
	mpz_clear(z1);
	mpz_clear(z2);
	mpz_clear(xC);
	mpz_clear(yC);
	mpz_clear(R);
	free(h);
}

int main(int argc, char** argv)
{
	//аргументов нет
	if (argv[1] == NULL)
	{
		printf("Welcome to Simple DS digital signature verifier app\n");
		printf("Digital signature algorythm standart is GOST 34.10-2018\n");
		printf("Hash-functions generator standert is GOST 34.11-2018\n");
		printf("Use \"-h\" to open list of options\n");
		return 0;
	}
	//аргумент -h, вывод гайда
	if (strcmp(argv[1], "-h") == 0 && argc == 2)
	{
		printf("-h				Get help;\n");
		printf("-ds <path to file>	Generate digital signature for chosen file using current parameters;\n");
		printf("-p <path to file>		Use signature parameters and table from .sdsp file;\n");
		printf("Parameters used by default at the start of this program are contained in ds_params.spsd\n");
		return 0;
	}
	//-h с лишними параметрами
	else if (strcmp(argv[1], "-h") == 0 && argc != 2)
	{
		printf("Option \"-h\" does not support any other arguments");
		return 0;
	}

	mpz_t p, a, b, m, q, xP, yP, xQ, yQ;
	mpz_init(p);
	mpz_init(a);
	mpz_init(b);
	mpz_init(m);
	mpz_init(q);
	mpz_init(xP);
	mpz_init(yP);
	mpz_init(xQ);
	mpz_init(yQ);
	//статус открытого файла
	char parfstatus = 0;
	//получение параметров из файла
	parfstatus += GetParams("ds_params.sdsp", p, a, b, m, q, xP, yP);

	//проверка подписи
	if (strcmp(argv[1], "-ds") == 0 && argc == 3)
	{
		if (parfstatus == 0)
		{
			FILE *target;
			//открытие целевого файла
			if ((target = fopen(argv[2], "r+b")) == NULL)
			{
				printf("Error reading target file");
				Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
				return 0;
			}
			//генерация подписи, добавление к файлу
			CheckDS(p, a, b, m, q, xP, yP, xQ, yQ, target);
			fclose(target);
		}
		//если параметры не получены
		else
			printf("Get the parameters first!");
		//очистить все переменные, используемые gmp
		Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
		return 0;
	}
	//кол-во параметров неверно
	else if (strcmp(argv[1], "-ds") == 0 && argc != 3)
	{
		printf("Incorrect number of arguments");
		Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
		return 0;
	}

	//параметры из файла
	else if (strcmp(argv[1], "-p") == 0 && argc == 3)
	{
		printf("It doesnt work right now");
		Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
		return 0;
	}
	else if (strcmp(argv[1], "-p") == 0 && argc != 3)
	{
		printf("Incorrect number of arguments");
		Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
		return 0;
	}

	//параметр не найден
	printf("No such parameter. Use \"-h\" to open list of options");
	//очистить все переменные, используемые gmp
	Clear_GMP(p, a, b, m, q, xP, yP, xQ, yQ);
	return 0;
}