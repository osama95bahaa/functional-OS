void printString(char*);
void readString(char*);
void readSector(char* buffer, int sector);
void handleInterrupt21(int ax, int bx, int cx, int dx);
int mod(int a, int b);
void readFile(char* fileName, char* buffer);
void executeProgram(char* name, int segment);
void terminate();
void writeSector(char* buffer, int sector);
void deleteFile(char* name);
void writeFile(char* name, char* buffer, int secNum);
void copyFile(char* file1 , char* file2);
void printDir();



int main ()
{

//char line[80];
//char buffer[512];



//printString("Hello World\0");

//printString("\n\r");
//printString("Enter a line: \0");
//readString(line);
//printString("\n\r\0");
//printString(line);
//printString("\n\r\0");

//readSector(buffer, 30);
//printString(buffer);


// makeInterrupt21();
//interrupt(0x21,0,0,0,0);     this is my own interrupt
 

//interrupt(0x21,1,line,0,0);
//interrupt(0x21,0,line,0,0);
//interrupt(0x21,3,line,0,0);
//interrupt(0x21,2,line,0,0);



// readFile

//char buffer[13312]; /*this is the maximum size of a file*/
//makeInterrupt21();
//interrupt(0x21, 3, "messag\0", buffer, 0); /*read the file into buffer*/
//interrupt(0x21, 0, buffer, 0, 0); /*print out the file*/
//while(1);/*hang up*/



//executeFile 

//makeInterrupt21();
//interrupt(0x21, 4, "tstprg\0", 0x2000, 0);
//while(1);


//deleteFile
/*
char buffer[13312];
makeInterrupt21();
interrupt(0x21, 7, "messag\0", 0, 0); //delete messag
interrupt(0x21, 3, "messag\0", buffer, 0); // try to read messag
interrupt(0x21, 0, buffer, 0, 0); //print out the contents of buffer

*/
//writeFile
/*

int i=0;
char buffer1[13312];
char buffer2[13312];
buffer2[0]='h'; buffer2[1]='e'; buffer2[2]='l'; buffer2[3]='l';
buffer2[4]='o';
for(i=5; i<13312; i++) buffer2[i]=0x0;
makeInterrupt21();
interrupt(0x21,8, "testW\0", buffer2, 1); //write file testW
interrupt(0x21,3, "testW\0", buffer1, 0); //read file testW
interrupt(0x21,0, buffer1, 0, 0); // print out contents of testW
while(1);

*/
//while(1){


//}


//SHELL

makeInterrupt21();
interrupt(0x21,4,"shell\0" , 0x2000,0);


while(1)
{


}



}






void printString(char* chars)
{

int i = 0;
 for(; chars[i]!='\0' ; i++){
   interrupt(0x10,0xE*256+chars[i],0,0,0);
 }
}




void readString(char* line)
{

int index = 0;
char readchar= interrupt(0x16,0,0,0,0);

while((readchar != 0xd) && (index <80)){

interrupt(0x10,0xE*256 + readchar ,0,0,0);

if (readchar != 0x8)
{
	line[index]=readchar;
	index++;

}
else if (index > 0)
{
	index--;
}

readchar = interrupt(0x16,0,0,0,0);
}

line[index] = 0;

}



int mod(int x, int y) {
    while(x >= y) {
        x= x - y;
    }
    return x;
}



void readSector(char* buffer, int sector)
{

int relativeSector = mod(sector,18) + 1;
int head =  mod(( sector / 18 ),2);
int track = ( sector / 36 );

interrupt(0x13, 2*256 +1 , buffer, track*256 + relativeSector , head*256 + 0 );
}



void handleInterrupt21(int ax, int bx, int cx, int dx)
{
 //printString("this is my interrupt");

if (ax==0)
{

	printString(bx);
	printString("\n\r\0");
} 
else if (ax==1)
{
  readString(bx);
  printString("\n\r\0");
  	

}
else if (ax==2)
{
	readSector(bx,cx);

}
else if (ax ==3)
{
	readFile(bx,cx);
}
else if (ax ==4)
{
	executeProgram(bx,cx);
}
else if (ax ==5)
{
	terminate();
}
 else if (ax ==6)
 {
 	writeSector(bx,cx);
 }
 else if (ax ==7)
 {
 	deleteFile(bx);
 }
 else if (ax ==8)
 {
 	writeFile(bx,cx,dx);
 }
 else if (ax ==9)
 {
 	copyFile(bx,cx);
 }
 else if (ax ==10)
 {
 	printDir();
 }
 else	
	printString("invalid interrupt");
}







void readFile(char* fileName, char* buffer) {
	
	int i;
	int j;

	char directory[512];
	
	readSector(directory, 2);

	
	for(i=0; i<512; i+=32) {
		for(j=0; j<6; j++) {
			if(directory[i+j] != fileName[j])
				break;
		}
		if(j == 6)
			break;
	}

	if(j == 6){
	
		for(j = 0; j<26; j++) {
			if(directory[i+j+6] != 0){

				readSector(buffer + (j) * 512, directory[i+j+6]);
			}		
			else break;
		}
	}
	else return;
}




void executeProgram(char* name, int segment){

int i =0;
char buffer[13312];
readFile(name , buffer);
 
for (i = 0; i < 13312; i++)
{
	putInMemory(segment, i, buffer[i]);

}
launchProgram(segment);

}


void terminate()
{
	//while(1){

	//}

	interrupt(0x21,4,"shell\0" , 0x2000,0);

}



void writeSector(char* buffer, int sector)
{

int relativeSector = mod(sector,18) + 1;
int head =  mod(( sector / 18 ),2);
int track = ( sector / 36 );

interrupt(0x13, 3*256 +1 , buffer, track*256 + relativeSector , head*256 + 0 );
}



void deleteFile(char* name)
{

int i;
int j;
int k;

char directory[512];
char map[512];

readSector(map,1);
readSector(directory,2);

for (i = 0; i < 512; i+=32)
{
	for (j = 0; j < 6; j++)
	{
		//btcheck 3l bytes lw la2t en e name da msh zy el da5ely b-break
		if(directory[i+j] != name[j])
			break;
	}
	//h-check lw j b 6 yb2a el name hwa hwa f hd5ol a-delete it  ama lw el j msh b 6 harg3 ll loop el kbeera w aroo7 el entry el b3daha
	if (j == 6)
	{
		//ha5aly awl byte b zero 
		directory[i] = 0;
		//3ayz aloop 3shan a5aly el bytes el 26 dol b zeroes
		for (k = 0; k < 26; k++)
		{
			if(directory[i+k+6] != 0x00)
			{
				map[i+k+7]=0x00;

			}
			else break;
		}


	}

	
}

	writeSector(map,1);
	writeSector(directory,2);

}


void writeFile(char* name, char* buffer, int secNum)
{



	char map[512];
	char directory[512];
	int entryNum;
	int free = 0;
	int nameLength = 0;
	int j;
	int k;
	int sectorNum;
	char thisSec[512];
	
	



	readSector(map,1);
	readSector(directory,2);
	
	//loop 3la el 16 entry el 3andy 
	 // check lw awl byte f kol entry kant b 0 yb2a la2t mkan else mfeesh mkan 
	while(entryNum<16){

		if (directory[32*entryNum] != 0x00){
			entryNum++;	
		}
		else {
			free = 1;
			break;
		}
		
	}
	//lw mfeesh fl 16 entry el 3andy haga fadya ha-return
	if (free == 0){
		printString("The directory is full . Cannot write the file on it.");
		return;
	}

	//Copy the name to that directory entry.  If the name is less than 6 bytes, fill in the remaining bytes with 0x00
	// ageeb el length bta3 el name 
	while(name[nameLength] != '\0' && name[nameLength] != 0x0){
		nameLength++;
	}
	//a7ot fl directory el name el da5ely
	j=0;
	while(j<nameLength){
		directory[32*entryNum+j] = name[j];
		j++;
	}

	// check lw el name el da5ely kan a2al mn 6 bytes , hakamel el ba2y zeroes
	if (nameLength < 6){
		
		for(j=0;j<6-nameLength;j++){
			
			directory[32*entryNum+j+nameLength] = 0x0;
		}
	}

	//For each sector making up the file:
	k=0;
	while(k < secNum){

		//Find a free sector by searching through the Map for a 0x00
		
		for (sectorNum = 0;map[sectorNum] != 0x0 ; sectorNum++)
		{
			
		}

		if (sectorNum!=26)
		{
					
		//Set that sector to 0xFF in the Map
		//Add that sector number to the file's directory entry
		//Write 512 bytes from the buffer holding the file to that sector

		map[sectorNum] = 0xFF;
		directory[32*entryNum+6+k] = sectorNum;
			for(j=0;j<512;j++){
			
				thisSec[j] = buffer[j*(k+1)];
			}
			writeSector(thisSec,sectorNum);
	k++;
		}
		else
		{
			printString("Not enough space in directory entry for file\n");
			return;

		}

	}

	writeSector(map,1);
	writeSector(directory,2);
}


void copyFile(char* file1 , char* file2)
{

int i=0;
int j=0;
char buffer[13312];
char directory[512];
char fileName[6];
int k=0;
int numOfSectors=0;

readSector(directory,2);


for (i = 0; i < 512; i+=32)

{
	for (j = 0; j<6; j++)
	{
			fileName[j] = directory[i+j];	
	}


	if (compareStr(fileName,file1,6) == 1)
	{
		
		//ageeb number of sectors
		for (k = i+6 ; directory[k]!=0; k++)
		{
			numOfSectors++;

		}

			readFile(file1,buffer);
			writeFile(file2,buffer,numOfSectors);
	}

	
}

}









int compareStr(char*pointer , char* fileName , int lengthOfString)
{

int i =0;

for (i = 0; i < lengthOfString; i++)
{
	if (pointer[i]!=fileName[i])
	{
		return 0;
	}
}
return 1;
}


void printDir()
{
	int i;
	int j;
	char dirName[9];
	char directory[512];
	//int k;
	//int numOfSectors;
		

	readSector(directory,2);

	for(i=0;i<512;i+=32)
	{
		for(j=0;j < 6; j++)
		{
			if (directory[i] == 0 )
			{
				break;
			}
			if(directory[i+j]!= 0x00)
			{
				dirName[j]=directory[i+j];
			}
			
			else dirName[j] = ' ';
		}

		/*for (k = i+6 ; directory[k]!=0; k++)
		{
			numOfSectors++;
		}*/

		dirName[6]='\r';
		dirName[7]='\n';
		dirName[8]='\0';
		

		printString(dirName);
		//printString(numOfSectors);
		
		
		dirName[0]=0x00;
	}
		writeSector(directory,2);

}

