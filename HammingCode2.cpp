
/* 
*  This program was written by Brady Sheehan 
*  This is an error correcting Hamming Code
*  using an (8,4) parity matrix.
*/

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

const char* encode(const char* fileName){
	const char* resultFileName = "encoded.txt";
	ifstream myFileIn;
	ofstream myFileOut (resultFileName, ios::binary);
	myFileIn.open(fileName, ios::binary);
	const unsigned char codewords[] = {0x00,0x1E,0x2D,0x33,0x4B,0x55,0x66,0x78,0x87,0x99,0xAA,0xB4,0xCC,0xD2,0xE1,0xff}; //codewords
	unsigned char byte = myFileIn.get();
	while(myFileIn){
		unsigned char hb2 = byte << 4;
		myFileOut << codewords[(byte >> 4)] << codewords[(hb2 >> 4)]; //shift to get 4 data bits and use as indicies to find codewords
		byte = myFileIn.get();
	}
	myFileIn.close();
	myFileOut.close();
	return resultFileName;
}

void decode(const char* fileName){
	const char* resultFileName = "decoded.txt";
	ifstream myFileIn;
	ofstream myFileOut (resultFileName, ios::binary);
	myFileIn.open(fileName, ios::binary);
	srand(time(NULL)); //set the seet for rand();
	while(myFileIn){
		unsigned char byte1 = myFileIn.get();
		if(!myFileIn){
			break; //fix for the end of line character, thanks Dr. Drozdek
		}
		unsigned char byte2 = myFileIn.get();
		// //Corrupt bits for demonstration only
		   //This simulates the file getting corrupted
 	//   	byte1 ^= (1 << (rand()%8)); //flips one of the bits in the 0-8th position semi-randomly
 	//   	byte2 ^= (1 << (rand()%8));
		// //end corrupt bits 

		unsigned char bits1[8];
		unsigned char bits2[8];
		for(int i = 0, j = 7; i < 8; i++, j--){
			bits1[i] = (byte1 >> j) & 1; //extract bit from byte1 in position i
			bits2[i] = (byte2 >> j) & 1; //extract bit from byte2 in position i
		}
		unsigned char s1 = bits1[3] ^ bits1[4] ^ bits1[5] ^ bits1[6]; //calculating syndrome bits from equations
		unsigned char s2 = bits1[1] ^ bits1[2] ^ bits1[5] ^ bits1[6];
		unsigned char s3 = bits1[0] ^ bits1[2] ^ bits1[4] ^ bits1[6];
		unsigned char ss1 = bits2[3] ^ bits2[4] ^ bits2[5] ^ bits2[6]; //calculating syndrome bits from equations
		unsigned char ss2 = bits2[1] ^ bits2[2] ^ bits2[5] ^ bits2[6];
		unsigned char ss3 = bits2[0] ^ bits2[2] ^ bits2[4] ^ bits2[6];
		bool s4 = bits1[0] ^ bits1[1] ^ bits1[2] ^ bits1[3] ^ bits1[4] ^ bits1[5] ^ bits1[6] ^ bits1[7];
		bool ss4 = bits2[0] ^ bits2[1] ^ bits2[2] ^ bits2[3] ^ bits2[4] ^ bits2[5] ^ bits2[6] ^ bits2[7];
		unsigned char s1s2s3 = (s1 << 2) | (s2 << 1) | s3; //concatenation
		unsigned char s1s2s3s4 = (s1s2s3 << 1) | s4; //concatenation
		unsigned char ss1s2s3 = (ss1 << 2) | (ss2 << 1) | ss3; //concatenation
		unsigned char ss1s2s3s4 = (ss1s2s3 << 1) | ss4; //concatenation
		//do checks and corrections for byte1
		if(s1s2s3s4 == 0){
			//cout << "Don't need to fix anything" << endl;
		}
		else if(s4 == 1 && s1s2s3 == 0){ //eighth bit is corrupted
			byte1 ^= (1 << 0); 
		} 
		else if(s4 == 1 && s1s2s3 != 0){
			s1s2s3 = 8 - s1s2s3;
			byte1 ^= (1 << s1s2s3);
		}
		else if(s4 == 0 && s1s2s3 != 0){
			cout << "two errors occured!" << endl;
		} //end bits1 check
		if(ss1s2s3s4 == 0){ //start bits2 check
			//cout << "Don't need to fix anything" << endl;
		}
		else if(ss4 == 1 && ss1s2s3 == 0){ //eighth bit is corrupted
			byte2 ^= (1 << 0);
		}
		else if(ss4 == 1 && ss1s2s3 != 0){
			ss1s2s3 = 8 - ss1s2s3;
			byte2 ^= (1 << ss1s2s3);
		}
		else if(ss4 == 0 && ss1s2s3 != 0){
			cout << "two errors occured!" << endl;
		}//end bits2 check
		unsigned char hb1 = byte1 >> 4; //get rid of syndrome
		unsigned char byteCombo = (hb1 << 4) | (byte2 >> 4); //concatenates the 2 half bytes back into one byte
		myFileOut << byteCombo; 
	}
	myFileIn.close();
	myFileOut.close();
}
//TEST
int main(){
	const char* fileName = encode("declaration.txt");
	decode(fileName);
	return 0;
}