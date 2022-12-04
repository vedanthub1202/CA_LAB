/*
PRN: 2020BTEIT00045
Assignment 5 Computer Architecture 
*/

Assignment 5 Problem Statement

Assume you are appointed as advisor to a political party. Your job is to advise on spending the money on advertising various important policies during the election campaign.
The effect of policies on voters is given in the form of-  No. of thousands of voters that can be won(+) /lost(-) by spending Rs 1000 on advertisements on the policies 
( Urban Suburban Rural and Outsider) is provided in the following table.

1. Build Roads -2, 5, 3, 6
2. Infrastructure 8, 2, -5, -2
3. Farm subsidies 1, 3, 10, 4
4. Fuel price 10, 6, -2, 5

Explaination: 

Time Complexity: 
  if N*N is the size of the 2D array then 
  the time complexity of the algorithm is O(N^2).

Space Complexity: 
  if the N*N is the size of the 2D array
  the extra space we need to have other than the 2D array is the N the size of x matrix.

By using Parallel computing the time complexity can get less time, but the algo rithm may get complex.

##
There are two triangular matrix matrices one is a lower with all the diagonal element as 1
and another is a upper triangular matrix.

* Given a set of linear equations, first convert them into matrix form A X = C where A is the coefficient matrix, X is the variable matrix and C is the matrix of numbers 
on the right-hand side of the equations.

* Now, reduce the coefficient matrix A, i.e., the matrix obtained from the coefficients of variables in all the given equations such that for ‘n’ variables we have an nXn matrix, 
to row echelon form using Gauss Elimination Method. The matrix so obtained is U.

* To find L, we have two methods. The first one is to assume the remaining elements as some artificial variables, make equations using A = L U and solve them to find those artificial variables.
The other method is that the remaining elements are the multiplier coefficients because of which the respective positions became zero in the U matrix. (This method is a little tricky to 
understand by words but would get clear in the example below)

* Now, we have A (the nXn coefficient matrix), L (the nXn lower triangular matrix), U (the nXn upper triangular matrix), X (the nX1 matrix of variables) and C (the nX1 matrix of 
numbers on the right-hand side of the equations).

The given system of equations is A X = C. We substitute A = L U. Thus, we have L U X = C.
We put Z = U X, where Z is a matrix or artificial variables and solve for L Z = C first and then solve for U X = Z to find X or the values of the variables, which was required.
