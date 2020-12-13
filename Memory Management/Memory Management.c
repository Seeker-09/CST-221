//
//  Memory Management.c
//  CST-221
//
//  Created by David Mundt on 12/8/20.
//

#include <stdio.h>

void toBinary(int);
void toHex(int);
void transform(int);

int main() {
    int number;
    
    printf("Enter the number you wish to be converted: \n");
    scanf("%d", &number);
    
    toBinary(number);
    toHex(number);
    transform(number);
    
    return 0;
}

// Function to convert decimal to binary
void toBinary(int number) {
    // Array to store the binary number of 32 bits
    int binaryNumber[32];
    
    // Binary array counter
    int i = 0; // reason for using the while loop is because I will need 'i' later
    while(number > 0) {
        binaryNumber[i] = number % 2;
        number = number / 2;
        i ++;
    }
    
    printf("\nThe number in binary is: ");
    // Reversing for the least significant bit first
    for(int j = i - 1; j >= 0; j--) {
        printf("%d", binaryNumber[j]);
    }
}

// Function to convert decimal to hexadecimal
void toHex(int number) {
    // Character array for storing the hexadecimal
    char hexaDecimalNumber[100];
    
    // Hexadecimal array counter
    int i = 0; // Like before, I will need the i for reversing
    while(number != 0) {
        int temp = 0; // Tempory variable to store remaider
        
        temp = number % 16;
        
        if(temp < 10) {
            hexaDecimalNumber[i] = temp + 48;
            i++;
        }
        else {
            hexaDecimalNumber[i] = temp + 55;
            i++;
        }
        
        number = number / 16;
    }
    
    // Printing hexadecimal array in reverse order
    printf("\nThe number in hex is: ");
    for(int j = i - 1; j >= 0; j--) {
        printf("%c", hexaDecimalNumber[j]);
    }
}

// Function that uses bitwise operations to transoform the original number
//    and ouput the new results in binary and hex
void transform(int number) {
    // Never ended up getting this to work properly :(
    number = (number << 16) & 0x00ff;
    number = number | 0x07FF;
    
    int biNumber = number;
    int hexNumber = number;
    printf("\nThe transformed number in Decimal is: %d", number);
    
    toBinary(biNumber);
    toHex(hexNumber);
}
