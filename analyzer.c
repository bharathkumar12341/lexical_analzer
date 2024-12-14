/*
NAME:
DATE:
DESCRIPTION:
SAMPLE INPUT:
SAMPLE OUTPUT:
*/
#include "header.h"
extern int error_flag;
char* keyword[] = {"goto","char","int","float","double","break","continue","return","if","else","while","void","for","do","switch","case","default","typedef","struct","union","enum","extern","static","volatile","const","unsigned","signed","short","long","NULL","sizeof","auto","register"};

char operator[] = {'+','-','/','%','^','!','~','*',','};
char symbol[] = {'{','}','[',']'};
char special[] = {'(',')',';'};

char delim[] = {'\n','\t',' '};


void lexical_analyze(FILE* fptr)
{
	char line[150];
	while(fscanf(fptr,"%[^\n]\n",line) != EOF)
	{
		if(line[0] == '#') //strncmp(line,"#",1) == 0 )
		{
			continue;
		}
		check_type(line);
	}
}
void check_type(char* line)
{
	char* temp_line = line;
	char arr[50];
	while(*temp_line)
	{
		if( strchr(delim,*temp_line) != NULL)
		{
			temp_line++;
			continue;
		}
		int i = 0;
		if( *temp_line == '"')
		{
			char ch;
			int flag = 0;
			temp_line++;
			while(*temp_line && *temp_line != '"')
			{
				if(*temp_line == '\\')
				{
					ch = *(temp_line+1);
					if(ch != 'b' && ch != 'f' && ch != 'n' && ch != 'a' && ch != 'n' && ch != 'v' && ch != 't' && ch != '0')
					{
						printf("Escape sequence is not allowed as %c%c\n",*temp_line,ch);
						error_flag = 1;
						temp_line++;
						continue;
					}

				}
				if(*temp_line == ')' || *temp_line == ';')
				{
					printf("ERROR: closing of \" missing\n");
						error_flag = 1;
					break;

				}
				if(flag==1)
					break;
				arr[i++] = *temp_line++;
			}
			arr[i] = '\0';
			printf("%-20s is a string literal\n",arr);
			if(*temp_line == '"')
			{
				temp_line++;
			}
			continue;
		}
		if(*temp_line == 39)
		{
			temp_line++;
			arr[i++] = *temp_line++;
			if(*temp_line == 39)
			{
				printf("%-20c is a character constant\n",arr[0]);
				temp_line++;
				continue;
			}
			else
			{
				int flag = 0;
				while(*temp_line != 39 && *temp_line)
				{
					if(*temp_line == ';' || *temp_line == ')')
					{
						flag = 1;
					}
					arr[i++] = *temp_line++;
				}
				arr[i] = '\0';
				if(flag == 0)
				{
					printf("Can't assign %s into a character\n",arr);
						error_flag = 1;
				}
				else
				{
					printf("Missing of \' single quote\n");
						error_flag = 1;
				}
				if(*temp_line == 39)
				{
					temp_line++;
				}
				continue;
			}
		}
		if( is_operator(*temp_line) == 1)
		{
			printf("%-20c is an operator\n",*temp_line);
			temp_line++;
			continue;
		}
		else if(is_special(*temp_line) == 1)
		{
			printf("%-20c is a special character\n",*temp_line);
			temp_line++;
			continue;
		}
		else if(is_assignment(*temp_line) == 1)
		{
			printf("%-20c is a assignment operator\n",*temp_line);
			temp_line++;
			continue;
		}
		else if(is_symbol(*temp_line) == 1)
		{
			printf("%-20c is a symbol\n",*temp_line);
			temp_line++;
			continue;
		}
		else if( *temp_line == '<' || *temp_line == '>')
		{
			if( *(temp_line+1) == '=')
			{
				printf("%c%-20c is a relational operator\n",*temp_line,*(temp_line+1));
				temp_line++;
				temp_line++;
				continue;
			}
			printf("%-20c is a relational operator\n",*temp_line);
			temp_line++;
		}
		else if( *temp_line == '&')
		{
			if(*(temp_line+1) == '&')
			{
				printf("%c%-20c is a logcal operator\n",*temp_line,*(++temp_line));
				temp_line++;
				continue;
			}
			printf("%-20c is a bitwise operator\n",*temp_line);
			temp_line++;
		}
		else if( *temp_line == '|')
		{
			if(*(temp_line+1) == '|')
			{
				printf("%c%-20c is a logcal operator\n",*temp_line,*(++temp_line));
				temp_line++;
				continue;
			}
			printf("%-20c is a bitwise operator\n",*temp_line);
			temp_line++;
		}
		while( *temp_line && (!strchr(delim,*temp_line)) && ( !is_special(*temp_line)) && (!is_assignment(*temp_line)) && (!is_operator(*temp_line)) && (!is_symbol(*temp_line)))
		{
			arr[i++] = *temp_line++;
		}
		arr[i] = '\0';
		if( strlen(arr) > 0)
		{
			if( is_keyword(arr) == 0 && is_digit(arr) == 0 && is_bin(arr) == 0 && is_octal(arr) == 0 && is_hexa(arr) ==0 )
			{
				if(*temp_line == '[')
				{
					char temp[50];
					i=strlen(arr);
					if(*(temp_line+1) == ']')
					{
						strcpy(temp,arr);
						temp[i++] = *temp_line++;
						temp[i++] = *temp_line++;
						temp[i] = '\0';
						printf("%-20s is an array\n",temp);
					}
					else
					{
						char num[20];
						strcpy(num,arr);
						num[strlen(arr)] = '[';
						num[i+1] = '\0';
						int i=0;
						temp_line++;
						while(*temp_line != ']' && *temp_line)
						{
							temp[i++] = *temp_line;
							temp_line++;
						}
						temp[i] = '\0';	
						if(is_digit(temp) == 1)
						{
							temp[i++] = ']';
							temp[i] = '\0';
							strcat(num,temp);
							printf("%-20s is an array\n",num);
							temp_line++;
						}
					}
				}
				else
				{
					printf("%-20s is an identifer\n",arr);
				}
			}

		}
	}
}

int is_keyword(char* token)
{
	for(int i = 0;i<32;i++)
	{
		if(strcmp(token,keyword[i]) == 0)
		{
			printf("%-20s is a keyword\n",token);
			return 1;
		}
	}
	return 0;
}
int is_digit(char* token)
{
	if(token[0] == '0' && strlen(token) > 1)
	{
		return 0;


	}
	int flag = 0,count = 0,char_flag = 0,digit_flag = 0;
	for(int i = 0; i < strlen(token) ;i++)
	{
		if(token[i] == 'f' && flag)
		{
			digit_flag++;

		}
		else if( ((token[i] >= 'a' && token[i] <= 'z' ) || (token[i] >= 'A' && token[i] <='Z')) && digit_flag)
		{
			if( token[2] != 'b' && token[2] != 'x' )
			{
				char_flag = 1;
			}
		}
		else if(  (token[i] >= '0' && token[i] <= '9'))
		{
			digit_flag++;
		}
		else if(token[i] == '.')
		{
			flag = 1;
			count++;
			digit_flag++;
		}
	}
	if(strlen(token) == digit_flag && flag == 0)
	{
		printf("%-20s is a integer constant\n",token);
		return 1;
	}
	else if( strlen(token) == digit_flag && flag == 1 && count < 2)
	{
		printf("%-20s is a float constant\n",token);
		return 1;
	}
	else if( char_flag == 1 || count > 1)
	{
		printf("ERROR: can't initialize a number as %s\n",token);
						error_flag = 1;
		return 1;
	}

	return 0;

}
int is_operator(char token)
{
	for(int i = 0;i<9;i++)
	{
		if(token == operator[i])
			return 1;
	}
	return 0;

}
int is_assignment(char ch)
{
	if(ch == '=')
		return 1;
	return 0;
}
int is_symbol(char ch)
{
	for(int i=0;i<4;i++)
	{
		if(ch == symbol[i])
			return 1;
	}
	return 0;

}
int is_special(char ch)
{
	for(int i=0;i<3;i++)
	{
		if(ch == special[i])
			return 1;
	}
	return 0;
}
int is_bin(char* number)
{
	if(strncmp(number,"0b",2) == 0)
	{
		for(int i=2;i < strlen(number);i++)
		{
			if(number[i] != '0' && number[i] != '1')
			{
				printf("Can't initialize binary number as %s\n",number);
						error_flag = 1;
				return 1;
			}
		}
		printf("%s is a binary number\n",number);
		return 1;

	}
	return 0;
}
int is_octal(char* number)
{
	if(number[0] == '0' &&  number[1] != 'x')
	{
		for(int i=0;i<strlen(number);i++)
		{
			if( (!(number[i] >= '0' && number[i] <= '7')) || (number[i] >= 'a' && number[i] <= 'z') || (number[i] >='A' && number[i] <= 'Z'))
			{
				printf("ERROR: can't initialize octal value as %s\n",number);
						error_flag = 1;
				return 1;
			}
		}
		printf("%-20s is an octal value\n",number);
		return 1;
	}
	return 0;
}
int is_hexa(char* number)
{
	if(strncmp(number,"0x",2) == 0)
	{
		for(int i = 2;i<strlen(number);i++)
		{
			if( (!(number[i] >= '0' && number[i] <= '9')) && (!(number[i] >= 'A' && number[i] <= 'F')) && (!(number[i] >='a' && number[i] <= 'f')))
			{
				printf("ERROR: can't initialize %s as hexadecimal value\n",number);
						error_flag = 1;
				return 1;
			}
		}
		printf("%-20s is a hexadecimal value\n",number);
		return 1;
	}
	return 0;
}
