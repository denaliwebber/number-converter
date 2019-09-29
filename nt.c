#include <math.h>
#include <stdio.h>
#include <string.h>

#define MAX_VALUE_STR 32
#define MAX_UNSIGNED_INT 4294967295
#define MAX_BIT_WIDTH 32

struct nt_info 
{
    char value_str[32];
    unsigned int value;
    int bit_width;
    int range_start;
    int range_end;
    int type;
};

void nt_info_init(struct nt_info* nt_data)
{
    nt_data->value_str[0]='\0';
    nt_data->value=0;
    nt_data->bit_width=32;
    nt_data->range_start=0;
    nt_data->range_end=31;
    nt_data->type=0;
}

unsigned int power(double x, double y)
{
    unsigned int total=1;

    for (int i=0; i<y; i++)
    {
        total*=x;
    }

    return total;
}

unsigned int signed_int_to_unsigned(char*);

unsigned int str_to_unsigned_int(char*);

//error checking functions

int error_unsigned_bit_size(char* s)
{
    int i; unsigned int m=1; unsigned int sum=0; int len; unsigned int x; char c;
    len=strlen(s);

    for (i=len-1; i>=0; i--)
    {
        if ((m==1000000000 && (s[i]>=53 || (s[i]==52 && sum>=294967296))))
        {
            return -1;
        }

        c=s[i];
        x=((int)c)-48;
        x*=m;
        sum+=x;
        m*=10;
    }

    return 1;
}

int error_check_input(char* s)
{
    //binary
    if (s[1]==98 && s[0]==48)
    {

        for (int i=2; i<strlen(s); i++)
        {
            if(s[i]!=48 && s[i]!=49)
            {
                printf("Error: %s contains characters other than 0 and 1\n", s);
                return -1;
            }
        }

        s+= 2;

        if(strlen(s)>32)
        {
            printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }

        return 2;
    }

    //hex
    else if(s[1]==120 && s[0]==48)
    {
        for(int i=2; i<strlen(s); i++)
        {
            if((s[i]<48 || s[i]>57) && (s[i]<65 || s[i]>70))
            {
                printf("Error: %s contains characters other than 0-9 and A-F\n", s);
               return -1;
            }
        }

        s+= 2;

        if(strlen(s)>8)
        {
            printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }

        return 3;
    } 

    //signed int
    else if(s[0]==45)
    {
        for (int i=1; i<strlen(s); i++)
        {
            if(s[i]<48 || s[i]>57)
            {
                printf("Error: %s contains characters other than 0-9\n", s);
                return -1;
            }
        }

        if (strlen(s)>11)
        {
        	printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }
        
        unsigned int u=str_to_unsigned_int(s+1);

        if (u>2147483648)
        {
            printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }

        return 1;
    }

    //unsigned
    else
    {
        for (int i=0; i<strlen(s); i++)
        {
            if(s[i]<48 || s[i]>57)
            {
                printf("Error: %s contains characters other than 0-9\n", s);
                return -1;
            }
        }

        if (strlen(s)>10)
        {
        	printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }

        if (error_unsigned_bit_size(s)==-1)
        {
            printf("Error: %s cannot fit into 32 bits\n", s);
            return -1;
        }

        return 0;
    }
}

int error_check_bit_width(int bit_width)
{
    if (bit_width%4!=0)
    {
        printf("Error: Bit width must be a multiple of 4\n");
        return -1;
    }

    if (bit_width<0 || bit_width>32)
    {
        printf("Error: Bit width must be between the range 0-32\n");
        return -1;
    }
}

int error_check_range(int start, int end)
{
    if (start<0 || end>31)
    {
        printf("Error: Range must be between 0-32\n");
        return -1;
    }

    if (start>end)
    {
        printf("Error: Range start must be larger than range end\n");
        return -1;
    }
}

int parse_command_line(int argc, char** argv, struct nt_info* nt_data)
{

    char bit_wid[10]="-b";
    char range[10]="-r";

    for (int i=1; i<argc; i++)
    {
        if (strcmp(argv[i], bit_wid)==0)
        {
            i++;
            int bit_width=str_to_unsigned_int(argv[i]);

            if (error_check_bit_width(bit_width)!=-1)
            {
                nt_data->bit_width = bit_width;
            }

            else
                return -1;
        }

        else if(strcmp(argv[i], range)==0)
        {
            i++;
            char comma[2]=",";
            char* s=argv[i];

            char* r=strtok(s, comma);

            int r_start=str_to_unsigned_int(r);

            r=strtok(NULL, comma);
            int r_end=str_to_unsigned_int(r);

            if (error_check_range(r_start, r_end)!=-1)
            {
                nt_data->range_start=r_start;
                nt_data->range_end=r_end;
            }

            else
                return -1;
        }

        else
        {
            int check=error_check_input(argv[i]);

            if (check!=-1)
            {
                strcpy(nt_data->value_str, argv[i]);
                nt_data->type=check;
            }

            else
                return -1;
        }
    }
}

//conversion functions

unsigned int str_to_unsigned_int(char* s)
{
    int i; unsigned int m=1; unsigned int sum=0; int len; unsigned int x; char c;
    
    len=strlen(s);

    for (i=len-1; i>=0; i--)
    {
        c=s[i];
        x=((int)c)-48;
        x*=m;
        sum+=x;
        m*=10;
    }

    return sum;
}

unsigned int signed_int_to_unsigned(char* s)
{
    int v; 
    unsigned int u;
    s++;

    v=str_to_unsigned_int(s);
    v=v*-1;
    u=(unsigned int)v;

    return u;
}

unsigned int binary_to_unsigned(char* arg)
{
    unsigned int num=0;
    int tracker=0;
    
    for (int i=strlen(arg)-1; i>=2; i--)
    {
        if(arg[i]==49)
            num=num+power(2, tracker);
        tracker++;
    }
    
    return num;
}

int str_to_hex(char s)
{
    char* characters="0123456789ABCDEF";
    
    //returns a pointer to the where s occurs in characters
    char* p=strchr(characters, s);

    //p minus characters then returns the index (or the numerical value of 0-F)
    return p-characters;
}

unsigned int hex_to_unsigned(char* s)
{
    int tracker=0;
    unsigned int sum=0;

    for (int i=strlen(s)-1; i>=2; i--)
    {
        int num=str_to_hex(s[i]);
        sum+=(power(16, tracker)*num);
        tracker++;
    }

    return sum;
}

void normalize_input_value(struct nt_info* nt_data)
{
    switch(nt_data->type)
    {
        case 0:
            nt_data->value=str_to_unsigned_int(nt_data->value_str);
            break;
        case 1:
            nt_data->value=signed_int_to_unsigned(nt_data->value_str);
            break;
        case 2:
            nt_data->value=binary_to_unsigned(nt_data->value_str);
            break;
        case 3:
            nt_data->value=hex_to_unsigned(nt_data->value_str);
            break;
    }
}

//output conversions

void unsigned_to_computer_binary(unsigned int value, int bit_width, char* s)
{
    s[bit_width]='\0';

    for (int i=bit_width-1; i>=0; i--)
    {
        s[i]=(value%2)+'0';
        value=value/2;
    }
}

void unsigned_to_hex(unsigned int value, int bit_width, char* s)
{
    //bit_width is divided by 4 because it holds 4 times the info as a bin bit
    s[bit_width/4]='\0';

    for (int i=(bit_width/4)-1; i>=0; i--)
    {
        if (value%16>=0 && value%16<=9)
        {
            s[i]=(value%16)+'0';
        }

        else
        {
            s[i]=((value%16)-10)+'A';
        }

        value=value/16;
    }
}

void unsigned_to_str(unsigned int value, int bit_width, char* s)
{
    if (bit_width<MAX_BIT_WIDTH)
        value=value & (power(2, bit_width)-1);

    else
        value=value & MAX_UNSIGNED_INT;

    s[10]='\0';

    for (int i=9; i>=0; i--)
    {
        s[i]=(value%10)+'0';
        value=value/10;
    }

    int i=0;

    while (s[i]==48)
    {
        i++;
    }

    char* p=s+i;

    if (s[i]=='\0')
        strcpy(s, s+i-1);

    else
        strcpy(s, s+i);
}

void unsigned_to_signed(unsigned int value, int bit_width, char* s)
{
    if (value>(power(2, bit_width-1)-1))
    {
        value=((~value)+1);
        strcpy(s, "-");
    }

    else
        strcpy(s, "");

    char p[12];

    unsigned_to_str(value, bit_width, p);

    strcat(s, p);
}

void output_conversions(struct nt_info* nt_data)
{
    //human binary output
    char p[nt_data->bit_width+1];

    unsigned_to_computer_binary(nt_data->value, nt_data->bit_width, p);

    for (int i=0; i<nt_data->bit_width; i++)
    {
        if (i%4==0 && i!=0)
            printf(" ");

        printf("%c", p[i]);
    }

    printf(" (base 2)\n");

    //computer binary
    printf("0b%s (base 2)\n",p);

    //hex
    char h[(nt_data->bit_width/4)+1];

    unsigned_to_hex(nt_data->value, nt_data->bit_width, h);
    printf("0x%s (base 16)\n", h);

    //unsigned
    char u[11];

    unsigned_to_str(nt_data->value, nt_data->bit_width, u);
    printf("%s (base 10 unsigned)\n", u);

    //signed
    char sig[12];

    unsigned_to_signed(nt_data->value, nt_data->bit_width, sig);

    printf("%s (base 10 signed)\n", sig);
}

void range(struct nt_info* nt_data)
{
    if (nt_data->range_start!=0 || nt_data->range_end!=31)
    {
        int len = (nt_data->range_end-nt_data->range_start)+1;
        nt_data->value = nt_data->value >> nt_data->range_start;
        nt_data->value = nt_data->value & ((1 << len)-1);
    }
}

int main(int argc, char **argv)
{
    if (argc<2)
        printf("Not enough arguements\n");

    else
    {
        struct nt_info nt_data;

        nt_info_init(&nt_data);

        if (parse_command_line(argc, argv, &nt_data)!=-1)
        {
            normalize_input_value(&nt_data);

            range(&nt_data);

            output_conversions(&nt_data);
        }
    }

    return 0;
}
