#ifndef ELITE_CRACK_H
#define ELITE_CRACK_H
void permutekey(uint8_t key[8], uint8_t dest[8]);
/**
 * Permutes  a key from iclass specific format to NIST format
 * @brief permutekey_rev
 * @param key
 * @param dest
 */
void permutekey_rev(uint8_t key[8], uint8_t dest[8]);
//Crack status, see below
#define CRACKED			0x0100
#define BEING_CRACKED	0x0200
#define CRACK_FAILED	0x0400

/**
 * Perform a bruteforce against a file which has been saved by pm3
 *
 * @brief bruteforceFile
 * @param filename
 * @param keytable an arrah (128 x 16 bit ints). This is where the keydata is stored.
 * OBS! the upper part of the 16 bits store crack-status,
 * @return
 */
int bruteforceFile(const char *filename, uint16_t keytable[]);
/**
 * @brief Same as bruteforcefile, but uses a an array of dumpdata instead
 * @param dump
 * @param dumpsize
 * @param keytable
 * @return
 */
int bruteforceDump(uint8_t dump[], size_t dumpsize, uint16_t keytable[]);

/**
  This is how we expect each 'entry' in a dumpfile to look
**/
typedef struct {
	uint8_t csn[8];
	uint8_t cc_nr[12];
	uint8_t mac[4];

}dumpdata;

/**
 * @brief Performs brute force attack against a dump-data item, containing csn, cc_nr and mac.
 *This method calculates the hash1 for the CSN, and determines what bytes need to be bruteforced
 *on the fly. If it finds that more than three bytes need to be bruteforced, it aborts.
 *It updates the keytable with the findings, also using the upper half of the 16-bit ints
 *to signal if the particular byte has been cracked or not.
 *
 * @param dump The dumpdata from iclass reader attack.
 * @param keytable where to write found values.
 * @return
 */
int bruteforceItem(dumpdata item, uint16_t keytable[]);
/**
 * Hash1 takes CSN as input, and determines what bytes in the keytable will be used
 * when constructing the K_sel.
 * @param csn the CSN used
 * @param k output
 */
void hash1(uint8_t csn[] , uint8_t k[]);

/**
 * From dismantling iclass-paper:
 *	Assume that an adversary somehow learns the first 16 bytes of hash2(K_cus ), i.e., y [0] and z [0] .
 *	Then he can simply recover the master custom key K_cus by computing
 *	K_cus = ~DES(z[0] , y[0] ) .
 *
 *	Furthermore, the adversary is able to verify that he has the correct K cus by
 *	checking whether z [0] = DES enc (K_cus , ~K_cus ).
 * @param keytable an array (128 bytes) of hash2(kcus)
 * @param master_key where to put the master key
 * @return 0 for ok, 1 for failz
 */
int calculateMasterKey(uint8_t first16bytes[], uint64_t master_key[] );

/**
 * @brief Test function
 * @return
 */
int testElite();

/**
	  Here are some pretty optimal values that can be used to recover necessary data in only
	  eight auth attempts.
// CSN                                        HASH1                                      Bytes recovered //
{ {0x00,0x0B,0x0F,0xFF,0xF7,0xFF,0x12,0xE0} , {0x01,0x01,0x00,0x00,0x45,0x01,0x45,0x45 } ,{0,1 }},
{ {0x00,0x13,0x94,0x7e,0x76,0xff,0x12,0xe0} , {0x02,0x0c,0x01,0x00,0x45,0x01,0x45,0x45} , {2,12}},
{ {0x2a,0x99,0xac,0x79,0xec,0xff,0x12,0xe0} , {0x07,0x45,0x0b,0x00,0x45,0x01,0x45,0x45} , {7,11}},
{ {0x17,0x12,0x01,0xfd,0xf7,0xff,0x12,0xe0} , {0x03,0x0f,0x00,0x00,0x45,0x01,0x45,0x45} , {3,15}},
{ {0xcd,0x56,0x01,0x7c,0x6f,0xff,0x12,0xe0} , {0x04,0x00,0x08,0x00,0x45,0x01,0x45,0x45} , {4,8}},
{ {0x4b,0x5e,0x0b,0x72,0xef,0xff,0x12,0xe0} , {0x0e,0x06,0x08,0x00,0x45,0x01,0x45,0x45} , {6,14}},
{ {0x00,0x73,0xd8,0x75,0x58,0xff,0x12,0xe0} , {0x0b,0x09,0x0f,0x00,0x45,0x01,0x05,0x45} , {9,5}},
{ {0x0c,0x90,0x32,0xf3,0x5d,0xff,0x12,0xe0} , {0x0d,0x0f,0x0a,0x00,0x45,0x01,0x05,0x45} , {10,13}},

**/


#endif
