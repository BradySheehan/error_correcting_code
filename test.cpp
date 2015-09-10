#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;
int main(){
	ifstream myFileIn1;
	ifstream myFileIn2;
	myFileIn2.open("decoded.txt", ios::binary);
	myFileIn1.open("declaration.txt", ios::binary);
	unsigned char byte1;
	unsigned char byte2;
	while(myFileIn1 && myFileIn2){
		byte1 = myFileIn2.get();
		byte2 = myFileIn1.get();
		if(byte1 == byte2){
			//do nothing
		}
		else{ //ops an error occured!
			cout << "byte 1 = " << byte1 << " byte2 = " << byte2 << endl;
			break;
		}
	}
}