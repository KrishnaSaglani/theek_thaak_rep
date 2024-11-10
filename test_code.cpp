// #include <iostream>
// using namespace std;

// int main(int argc, char *argv[]) {
//     if (argc != 2) {
//         cerr << "Please provide the capacity as an argument" << endl;
//         return 1;
//     }

//     int capacity = atoi(argv[1]);

//     // Just an example of producing three numbers based on capacity
//     cout << capacity * 2 << endl;  // Output 1
//     cout << capacity / 2 << endl;  // Output 2
//     cout << capacity + 10 << endl; // Output 3

//     return 0;
// }

#include <iostream>
#include <iomanip>  // To control the output format (decimal places)

using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        cerr << "Invalid number of arguments" << endl;
        return 1;
    }

    // Get the capacity from the argument
    double capacity = atoi(argv[1]);  // Convert the input to a double


    // int capacity;
    // cin>>capacity;

    // Set the output to show 2 decimal places
    cout << fixed << setprecision(2);

    // Output decimal values
    cout << capacity * 2.5 << endl;  // Example output: multiply by 2.5
    cout << capacity / 3.0 << endl;  // Example output: divide by 3
    cout << capacity - 1.5 << endl;  // Example output: subtract 1.5

    return 0;
}

