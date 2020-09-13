#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include<stdio.h>
int main()
{
    int t,a,b;
    scanf("%d",&t);
    while(t--)
    {
        scanf("%d%d",&a,&b);
        a=a%4;
        b=b%4;
        if((a==0) || (a==3) || (b==0) || (b==3))
            printf("First\n");
        else
            printf("Second\n");
            
    }
    return 0;
}
