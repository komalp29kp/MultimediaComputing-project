
/*Assignment 1 #, Pawar , Tiny C */
#include <stdio.h>
int main()
{
    float R,G,B;
    float C,M,Y,K,W,Rf,Gf,Bf,max;
    printf("Enter the values of Red(R),Green(G),Blue(B): ");/*Here we print the values of red green and blue color*/
    scanf("%f,%f,%f",&R,&G,&B); /*Reading the values given*/
    if (R<0||R>255)
    {
        printf("Enter R within limit(0-255)\n"); /* values of R with in the range*/
        scanf("%f",&R);
    }
    if (G<0||G>255)
    {
        printf("Enter G within limits(0-255)\n"); /* values of G with in the range*/
        scanf("%f",&G);
    }
    if (B<0||B>255)
    {
        printf("Enter B within limits(0-255)\n"); /* values of B with in the range*/
        scanf("%f",&B);
    }
    printf("\nR,G,B: %f,%f,%f\n",R,G,B);
    if (R == 0 && G == 0 && B == 0)
    {
        printf("\nThe value of Cyan: 0\n");
        printf("\nThe value of Magenta: 0\n");
        printf("\nThe value of Yellow: 0\n");
        printf("\nThe value of Black: 1\n");
    }
    else
    {
        Rf = R / 255;
        Gf = G / 255;
        Bf = B / 255;
        max = Rf;
        if (max<Gf)
            max = Gf;
        if (max<Bf)
            max = Bf;
        W = max;
        printf("\nWhite: %f\n\n", W*100);
        C = ((W-Rf)/W)*100;
        M = ((W-Gf)/W)*100;
        Y = ((W-Bf)/W)*100;
        K = (1- W)*100;
        printf("The percentage of Cyan: %f\n", C);
        printf("The percentage of Magenta: %f\n", M);
        printf("The percentage of Yellow: %f\n", Y);
        printf("The percentage of Black: %f\n", K);
    }
}