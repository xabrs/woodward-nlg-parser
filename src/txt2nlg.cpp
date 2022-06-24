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
	char nlgfilename[255];
	// NLG
	uint32_t nlg_int[2];
	float nlg_value[500];
	float tmp_float;
	int i, count,x,y, ID;

	if (argc==1){
		printf("Usage: txt2nlg.exe <NLG_TXT>\nOr drag txt file to exe\n");
		exit(0);
	}
	
	txtfile = fopen(argv[1], "r");
	if (txtfile==NULL) {
		printf("File open error\n");
		exit(0);
	}
	sprintf(nlgfilename, "%s.nlg",argv[1]);

	nlgfile = fopen(nlgfilename,"wb");
	if (txtfile==NULL){
		printf("Cannot open file %s to write", nlgfilename);
	}
	if (fgets(buf, BUFLEN,txtfile)!=NULL){
		fprintf(nlgfile,"%s", buf);
		if (fgets(buf, BUFLEN,txtfile)!=NULL){
			fprintf(nlgfile,"%s", buf);
			fscanf(txtfile,"%*s%*s%*s%d,%*s%*s%d\n", &count, &ID);
			while (fscanf(txtfile, "%d\t%f",&x,&nlg_value[i])==2 || !feof(txtfile) || i<count){
				i++;
			}
			if (i!=count){
				printf("Variables count not equal\n");
				count = i;
			}
			count = reverseInt(count-1);
			ID = reverseInt(ID);
			fwrite(&count,4,1, nlgfile);
			fwrite(&ID,4,1, nlgfile);
			count = i;
			for ( i = 0; i < count; i++){
				tmp_float = reverseFloat(nlg_value[i]);
				fwrite(&tmp_float,4,1, nlgfile);
			}
			printf("count=%d\n",count);
			
		} else {
			ExitInvalidFile();
		}
	} else {
		ExitInvalidFile();
	}
	fclose(txtfile);
	fclose(txtfile);
	return 0;
}