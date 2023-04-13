#include <cs50.h>
#include <stdio.h>

//declare funciton
bool valid_triangle (double a, double b, double c);

int main(void)
{
   double a = get_double("What is the first side length?: ");

   double b = get_double("What is the second side length?: ");

   double c = get_double("What is the third side length?: ");

   bool tri = valid_triangle (a, b, c);

   if (tri == true)
   {
      printf("TRUE\n");
   }

   else
   {
      printf("FALSE\n");
   }
}

bool valid_triangle (double a, double b, double c)
{
   bool t; //declare variable

   //main triangle definition

   if (a > 0 && b > 0 && c > 0 && a + b > c && b + c > a && a + c > b )
   {
      t = true;
   }
   else
   {
      t = false;
   }

   return t; //return true or false

}