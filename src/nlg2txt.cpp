#include <stdio.h>
#include <cstdint>
#include <cstdlib>
#define BUFLEN 80
using namespace std;

uint32_t reverseInt (int i) {
	unsigned char c1, c2, c3, c4;
    c1 = i & 255;
    c2 = (i >> 8) & 255;
    c3 = (i >> 16) & 255;
    c4 = (i >> 24) & 255;
	return ((int)c1 << 24) + ((int)c2 << 16) + ((int)c3 << 8) + c4;
}
float reverseFloat( const float inFloat )
{
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}

void ExitInvalidFile(){
	printf("Invalid file\n");
	exit(0);
}

int main(int argc, char const *argv[])
{
	FILE *nlgfile, *txtfile;
	char buf[BUFLEN];
	char txtfilename[255];
	// NLG
	uint32_t nlg_int[2];
	float nlg_value[1];
	int i;

	if (argc==1){
		printf("Usage: nlg2txt.exe <NLG>\nOr drag nlg file to exe\n");
		exit(0);
	}
	
	nlgfile = fopen(argv[1], "r");
	if (nlgfile==NULL) {
		printf("File open error\n");
		exit(0);
	}
	sprintf(txtfilename, "%s.txt",argv[1]);

	txtfile = fopen(txtfilename,"w");
	if (txtfile==NULL){
		printf("Cannot open file %s to write", txtfilename);
	}
	if (fgets(buf, BUFLEN,nlgfile)!=NULL){
		fprintf(txtfile,"%s", buf);
		if (fgets(buf, BUFLEN,nlgfile)!=NULL){
			fprintf(txtfile,"%s", buf);
			if (fread(nlg_int, sizeof(uint32_t), 2, nlgfile)<2){
				ExitInvalidFile();
			}
			nlg_int[0] = reverseInt(nlg_int[0])+1;
			nlg_int[1] = reverseInt(nlg_int[1]);
			fprintf(txtfile,"Variables count = %d, id = %d\n", nlg_int[0], nlg_int[1]);
			for (i=0; i<nlg_int[0]+1; i++){
				if (fread(nlg_value, sizeof(float), 1, nlgfile)>0){
					fprintf(txtfile,"%d\t%.9f\n",i,reverseFloat(nlg_value[0]));
				} else break;
			}
			
		} else {
			ExitInvalidFile();
		}
	} else {
		ExitInvalidFile();
	}
	fclose(txtfile);
	fclose(nlgfile);
	return 0;
}