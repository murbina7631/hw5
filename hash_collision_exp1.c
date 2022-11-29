#include <stdio.h>
#include <string.h>
#include <openssl/evp.h>
#include <time.h>

// function to grab a random string
char* random_string(int length)
{
	char charset[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* rand_string = malloc(length + 1);
	
	for (int i = 0; i < length; i++) {
		int key = rand() % (int) (sizeof(charset) - 1);
		rand_string[i] = charset[key];
	}
	rand_string[length] = '\0';
	
	//printf("Random string: %s\n", rand_string);
	return rand_string;
}

int main(int argc, char *argv[])
{
	// NOTE: Needed for EVP_get_digestbyname() to work
    OpenSSL_add_all_algorithms();
	// NOTE: Added for true randomizer
	srand(time(NULL));
	
	// initializing variables
	int length = 10;					// length of message strings
	int bit_similarity = 3;				// looking at first 24 bytes in hash
	int matching = 0;					// dictate whether hash found or not
	int trials = 0;						// count number of trials run
	
    EVP_MD_CTX *mdctx;
    const EVP_MD *md;
    unsigned int md_len, i;

    if (argv[1] == NULL) {
        printf("Usage: mdtest digestname\n");
        exit(1);
    }

    md = EVP_get_digestbyname(argv[1]);
    if (md == NULL) {
        printf("Unknown message digest %s\n", argv[1]);
        exit(1);
    }
	
	while (matching == 0) {
		trials++;
		
		char mess1[length];
		strcpy(mess1, random_string(length));
		char mess2[length];
		strcpy(mess2, random_string(length));
		unsigned char md_value1[EVP_MAX_MD_SIZE];
		unsigned char md_value2[EVP_MAX_MD_SIZE];
		unsigned int md_len, i;

		// hash the first message
		mdctx = EVP_MD_CTX_create();
		EVP_DigestInit_ex(mdctx, md, NULL);
		EVP_DigestUpdate(mdctx, mess1, strlen(mess1));
		EVP_DigestFinal_ex(mdctx, md_value1, &md_len);
		EVP_MD_CTX_destroy(mdctx);
		
		// hash the second message
		mdctx = EVP_MD_CTX_create();
		EVP_DigestInit_ex(mdctx, md, NULL);
		EVP_DigestUpdate(mdctx, mess2, strlen(mess2));
		EVP_DigestFinal_ex(mdctx, md_value2, &md_len);
		EVP_MD_CTX_destroy(mdctx);
		
		// comparing first message hash to the second message hash
		matching = 1;
		for (i = 0; i < bit_similarity; i++) {
			if (md_value1[i] != md_value2[i]) {
				matching = 0;
			}
		}
		
		if (matching == 1) {
			// print the messages
			printf("Message 1: %s\n", mess1);
			printf("Digest of message 1 is: ");
			for (i = 0; i < md_len; i++)
				printf("%02x", md_value1[i]);
			printf("\n");
			
			printf("Message 2: %s\n", mess2);
			printf("Digest of message 2 is: ");
			for (i = 0; i < md_len; i++)
				printf("%02x", md_value2[i]);
			printf("\n\n");
		}

		EVP_cleanup();
	}
		
	printf("Trials run: %d\n", trials);
    exit(0);
}