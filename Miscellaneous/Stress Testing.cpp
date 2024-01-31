#include<bits/stdc++.h>
using namespace std;

// the following script is for unix systems, for windows please change accordingly

const int ITER = 100; // select the number of iterations

// populate and save the correct, wrong and generator files
int32_t main() {
  string correct = "g++-11 -o ./correct ./correct.cpp";
  system(correct.c_str()); // compiling the correct solution
  string wrong = "g++-11 -o ./wrong ./wrong.cpp"; 
  system(wrong.c_str()); // compiling the wrong solution
  string generator = "g++-11 -o ./generator ./generator.cpp";
  system(generator.c_str()); // compiling the generator

  auto st = clock();
  for (int t = 1; t <= ITER; t++) {
    cerr << "Trying for the " << t << "-th time :((\n";

    string in = "input.txt";
    string generator = "./generator > ./input.txt";
    system(generator.c_str()); // generating a case

    string correct_solution = "./correct < ./input.txt > ./correct_solution.txt";
    system(correct_solution.c_str());

    string wrong_solution = "./wrong < ./input.txt > ./wrong_solution.txt";
    system(wrong_solution.c_str());

    ifstream correct_file;
    correct_file.open("correct_solution.txt"); //opening correct solution file
    string correct_output = "", line;
    while (getline(correct_file, line, '.')) { // reading every line including spaces and newlines
      correct_output += line;
    }

    ifstream wrong_file;
    wrong_file.open("wrong_solution.txt"); //opening wrong solution file
    string wrong_output = "";
    while (getline(wrong_file, line, '.')) { // reading every line including spaces and newlines
      wrong_output += line;
    }

    if (correct_output != wrong_output) {
      cout << "MISMATCH FOUND while running test case " << t << "\n\n";
      cerr << "MISMATCH FOUND while running test case " << t << "\n\n";
      cout << "Check the input.txt file to find the case, correct_solution.txt to find the correct output and similarly check the wrong_solution.txt\n";
      cerr << "Time taken = " << 1.0 * (clock() - st) / CLOCKS_PER_SEC << "s\n";
      exit(0);
    }

    correct_file.close();
    wrong_file.close();
  }
  cout << "No cases found!\n";
  cerr << "No cases found!\n";
  cerr << "Time taken = " << 1.0 * (clock() - st) / CLOCKS_PER_SEC << "s\n";
  return 0;
}
