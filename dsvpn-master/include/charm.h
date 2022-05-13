/*
Organization: Technical University of Kosice (TUKE),
Department: Department of Electronics and Multimedia Telecommunications (DEMT/KEMT), 
Faculties: Faculty of Electrical Engineering and Informatics (FEI),
Feld of study: 	Informatics,
Study program: Computer Networks,  
School year: 4., Ing., 2021/2022
Author of corrections:  Marek Rohac -- MR,
Last Update: 20.02.2022 -- add description
*/
#ifndef charm_H
#define charm_H 1

#include <stdint.h>
#include <stdlib.h>

void uc_state_init(uint32_t st[12], const unsigned char key[32], const unsigned char iv[16]);

void uc_encrypt(uint32_t st[12], unsigned char *msg, size_t msg_len, unsigned char tag[16]);

int uc_decrypt(uint32_t st[12], unsigned char *msg, size_t msg_len,
               const unsigned char *expected_tag, size_t expected_tag_len);

void uc_hash(uint32_t st[12], unsigned char h[32], const unsigned char *msg, size_t len);

void uc_memzero(void *buf, size_t len);

void uc_randombytes_buf(void *buf, size_t len);

#endif
