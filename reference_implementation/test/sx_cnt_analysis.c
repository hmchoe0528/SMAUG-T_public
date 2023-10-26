#include "io.h"
#include "kem.h"
#include "parameters.h"
#include "rng.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define m_size 10

void sx_cnt_json(void);
void injection_check(void);

int main(void) {
    time_t mytime = time(NULL);
    char *time_str = ctime(&mytime);
    time_str[strlen(time_str) - 1] = '\0';
    printf("\n*** %s with mode %d starts at %s\n", "SMAUG KEM", SMAUG_MODE,
           time_str);

    sx_cnt_json();
    // injection_check();

    return 0;
}

void sx_cnt_json(void) {
    size_t count = 1;
    const size_t iteration = 100000;

    uint8_t entropy_input[48] = {0};
    for (size_t i = 0; i < 48; ++i) {
        entropy_input[i] = i;
    }
    randombytes_init(entropy_input, NULL, 256);

    secret_key sk_temp;
    uint16_t cnt_analysis[MODULE_RANK][LWE_N] = {0};
    for (size_t i = 0; i < iteration; ++i) {
        uint8_t pk[PUBLICKEY_BYTES] = {0};
        uint8_t sk[KEM_SECRETKEY_BYTES] = {0};

        crypto_kem_keypair(pk, sk);
        load_from_string_sk(&sk_temp, sk, 0);

        // printf("s(x)'s cnt_arr: [");
        for (size_t j = 0; j < MODULE_RANK; ++j) {
            uint16_t temp = sk_temp.cnt_arr[j];
            (cnt_analysis[j][temp])++;
            // printf("%hu, ", temp);
            // printf("cnt_analysis[%lu][%hu] = %hu\n", j, temp,
            //        cnt_analysis[j][temp]);
        }
        // printf("\b\b]\n");
    }

    int flag = 0;
    char file_path[100] = {0};
    char name1[] = "./smaug";
    char name2[10] = {0};
    char name3[] = ".json";

    sprintf(name2, "%d", SMAUG_MODE);
    strcat(file_path, name1);
    strcat(file_path, name2);
    strcat(file_path, name3);

    FILE *file = NULL;
    file = fopen(file_path, "w");
    if (file == NULL) {
        printf("Fail to open file\n");
        return;
    }
    fprintf(file, "[\n");
    for (size_t i = 0; i < MODULE_RANK; ++i) {
        fprintf(file, "\t{");
        for (size_t j = 0; j < LWE_N; ++j) {
            if (cnt_analysis[i][j] != 0) {
                if (flag == 0) {
                    fprintf(file, "\"%lu\": %hu", j, cnt_analysis[i][j]);
                    flag++;
                } else
                    fprintf(file, ", \"%lu\": %hu", j, cnt_analysis[i][j]);
            }
        }
        flag = 0;
        if (i == MODULE_RANK - 1)
            fprintf(file, "}\n");
        else
            fprintf(file, "},\n");
    }
    fprintf(file, "]\n");
    fclose(file);
}

void injection_check(void) {
    size_t section[5] = {0};
    size_t upper_bound = 0;

#if SMAUG_MODE == 1
    upper_bound = 0x100;
#elif SMAUG_MODE == 3
    upper_bound = 0x300;
#elif SMAUG_MODE == 5
    upper_bound = 0x500;
#endif

    for (size_t j = 0; j < upper_bound; ++j) {
        size_t temp = j % (LWE_N * MODULE_RANK);
        if (0 <= temp && temp < 256)
            section[0]++;
        else if (256 <= temp && temp < 512)
            section[1]++;
        else if (512 <= temp && temp < 768)
            section[2]++;
        else if (768 <= temp && temp < 1024)
            section[3]++;
        else if (1024 <= temp && temp < 1280)
            section[4]++;
        else {
            printf("somehting wrong\n");
            return;
        }
    }

    printf("For modulus %d, [%lu", LWE_N * MODULE_RANK, section[0]);
    for (size_t j = 1; j < 5; ++j)
        printf(", %lu", section[j]);
    printf("]\n");
}