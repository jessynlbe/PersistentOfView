#include "../headers/bits.h"
#include "../headers/numbers.h"

int getBits(unsigned long pos , unsigned long number){
    return ( number >> pos) & 1UL;
}



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