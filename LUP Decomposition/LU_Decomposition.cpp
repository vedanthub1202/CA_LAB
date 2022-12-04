///PRN: 2020BTEIT00045


#include <bits/stdc++.h>
using namespace std;

const int MAX = 100;

void luDecomposition(int A[][MAX],int B[], int n)
{
	int lower[n][n], upper[n][n];
	memset(lower, 0, sizeof(lower));
	memset(upper, 0, sizeof(upper));

	// Decomposing matrix into Upper and Lower
	// triangular matrix
	for (int i = 0; i < n; i++)
	{
		// Upper Triangular
		for (int k = i; k < n; k++)
		{
			// Summation of L(i, j) * U(j, k)
			int sum = 0;
			for (int j = 0; j < i; j++)
				sum += (lower[i][j] * upper[j][k]);

			// Evaluating U(i, k)
			upper[i][k] = A[i][k] - sum;
		}

		// Lower Triangular.
		for (int k = i; k < n; k++)
		{
			if (i == k)
				lower[i][i] = 1; // Diagonal as 1
			else
			{
				// Summation of L(k, j) * U(j, i)
				int sum = 0;
				for (int j = 0; j < i; j++)
					sum += (lower[k][j] * upper[j][i]);

				// Evaluating L(k, i)
				lower[k][i]
					= (A[k][i] - sum) / upper[i][i];
			}
		}
	}

	// setw is for displaying nicely
	cout << setw(6)
		<< "    Lower Triangular"
		<< setw(32)
		<< "Upper Triangular" << endl;

	// Displaying the result :
	for (int i = 0; i < n; i++)
	{
		// Lower
		for (int j = 0; j < n; j++)
			cout << setw(6) << lower[i][j] << "\t";
		cout << "\t";

		// Upper
		for (int j = 0; j < n; j++)
			cout << setw(6) << upper[i][j] << "\t";
		cout << endl;
	}
    //For calculating y
    int ans1[n];
    for(int i = 0;i<n;i++){
        int sum = 0;
        for(int j = 0;j<i;j++){
            sum += lower[i][j]*ans1[j];
        }
        if(lower[i][i]!=0){
            ans1[i] = (B[i]-sum)/lower[i][i];
        }
        else{
            ans1[i] = 0;
        }
    }
    //For calculatig x
    double ans2[n];
    for(int i = n-1;i>=0;i--){
        int sum = 0;
        for(int j = n-1;j>i;j--){
            sum += upper[i][j]*ans2[j];
        }
        if(upper[i][i]!=0){
            ans2[i] = float((ans1[i]-sum)/upper[i][i]);
        }
        else{
            ans2[i] = 0;
        }
    }
    for(int i = 0;i<n;i++){
        cout<<ans2[i]<<" "<<endl;
    }
}

// Driver code
int main()
{
	int mat[][MAX]
		= { { 1,-2,3}, { 2,1,3 }, { -3,2,-2} };
    int B[3] = {7,4,-10};
	luDecomposition(mat, B, 3);
	return 0;
}