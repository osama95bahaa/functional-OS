int compareStr(char* pointer , char* fileName , int lengthOfString);

int main ()
{

char command[80];
char buffer[13312];
int i;
char file1[6];
char file2[6];



while (1)
{

	interrupt(0x21,0,"SHELL> \0",0,0);		
	interrupt(0x21,1,command,0,0);

	if (compareStr(command,"view",4) ==1)
	{
		interrupt(0x21,3,command+5,buffer,0);
		interrupt(0x21,0,buffer,0,0);
	} 
	else if (compareStr(command , "execute" ,7 ) ==1)
	{
		interrupt(0x21,4,command+8,0x2000,0);
	}
	else if (compareStr(command , "delete" , 6)==1)
	{
		interrupt(0x21,7,command+7,0,0);
	}
	else if (compareStr(command , "copy" ,4)==1)
	{		
			for (i = 0; i < 6; i++)
			{
				file1[i] = command[i+5];
				file2[i] = command[i+12];
			}
	
		interrupt(0x21,9,file1,file2,0);
	}
	else if (compareStr(command , "dir" ,3 ) ==1)
	{
		interrupt(0x21,10,0,0,0);
	}
	
 	else{
			interrupt(0x21,0,"Bad Command!!! \n\r\0",0,0);		
 	
		}		
}

}



int compareStr(char* pointer , char* fileName , int lengthOfString)
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
