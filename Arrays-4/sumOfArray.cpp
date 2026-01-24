#include <iostream>
#include <vector>

using namespace std;

int main()
{
    int n;
    cout << "Enter size of array: ";
    cin >> n;

    vector<int> v(n+1,0);
    cout << "Enter elements of array: ";
    for(int i=1; i<=n; i++){
        cin >> v[i];
    }

    //calculate prefix sum
    for(int i=1; i<=n; i++){
        v[i] += v[i-1];
    }

    int q;
    cout << "Enter number of queries: ";
    cin >> q;

    while(q--){
        int l, r;
        cout << "Enter left and right indices: ";
        cin >> l >> r;

        int ans = 0;
        ans = v[r] - v[l-1];
        cout << ans << endl;
    }
}