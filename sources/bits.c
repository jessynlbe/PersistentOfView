#include "../headers/bits.h"
#include "../headers/numbers.h"

int getBits(unsigned long pos , unsigned long number){
    return ( number >> pos) & 1UL;
}


// This function allows you to place the "drawings" of the numbers
// in a 32x32 table (size of the clock) at the desired positions. 
// startBits : Bit with which we start to copy the table
// offH / offW : Shift in width and height of the table to be included in the final table
void fillTab(int startBits , int offH , int offW ,const uint16_t tab[11]){
    int idxH = offH;
    int idxW = offW;
    for(int j = 0 ; j < startBits ; j++){
        for(int i = 0 ; i < startBits ; i++){
            
            int bit = getBits(i , tab[j] );
            if(bit == 1){
                final_b[idxH] |= (1UL << idxW);
            }
            idxW++;
        }

        idxW = offW;
        idxH++;
    }

}

// Polar to cartesian conversion
// rad : radius of the circle
// angle : angle of the circle
// size : size of tab In which we want to get the value
// value : Value allowing to decide if we want to recover the x or the y
int convertPolarToCartesian(int rad , int angle , int size , int value){
    float theta = (float) angle * (M_PI / 180.0);
    int x = (int) floorf(rad * sinf(theta));
    int y = (int) floorf(rad * cosf(theta));
    if(value == 0){
        return x+size;
    }
    else{
        return y+size;
    }
}

// Convert map coordinates to coordinates for an array
// x : Card position x
// y : Card position y
// size : Tab size
// value : Value allowing to decide if we want to recover the x or the y
int convertCartToArr(int x , int y , int size , int value){
    if(value == 0){
        return x;
    }
    else{
        return (size-1) - y;
    }
}

int getBitsTab(uint32_t *tab , int x , int y){
    return getBits(32-x-1 , tab[y]);
}


// Calculates the values of the leds for each column of the display with a polar to cartesian conversion
// nbLeds : number of leds / polar circle radius
// nbSlots : Number of columns that cut the clock
// tab : Table containing the time display to be converted to remove distortion
// size : Table size tab
// arr : Table in which we write the values of each led for each column
void getLeds(int nbLeds , int nbSlots , uint32_t *tab , int size , uint8_t *arr){
    int rows = nbSlots;
    int cols = nbLeds;

    int offsetAngle = (int) (360/nbSlots);

    int idx = 0;
    for(int angle = 0 ; angle < 360 ; angle += offsetAngle){
        for(int rad = 0 ; rad < nbLeds ; rad++){
            int x = convertPolarToCartesian(rad, angle, nbLeds , 0);
            int y = convertPolarToCartesian(rad, angle, nbLeds , 1);

            int xx = convertCartToArr(x,y,size,0);
            int yy = convertCartToArr(x,y,size,1);

            if(xx < 0){
                xx = 0;
            }

            if(yy < 0){
                yy = 0;
            }

            if(xx >= size){
                xx = size-1;
            }

            if(yy >= size){
                yy = size -1;
            }

            if(rad >= 0 && rad < 8){
                int bit = getBitsTab(tab , xx ,yy);
                if(bit == 1){
                    arr[idx] |= ( 1UL << rad);
                }
            }
            else{
                int bit = getBitsTab(tab , xx , yy );
                if(bit == 1){
                    arr[idx+1] |= ( 1UL << rad-8);
                }
            }
        }
        idx +=2;
    }

}


uint32_t final_b[32] = {
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000001,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,

        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,

        
        
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000,
        0b00000000000000000000000000000000
    };