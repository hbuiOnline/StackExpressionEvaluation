//Han Bui
//ID#: 1858671
//COSC 2430 Data Structure
//Professor: Nouhad Rizk
//Homework 5

#include <iostream>
#include <string>
#include <fstream>
#include <cctype>
#include <assert.h>
#include <cassert>
#include <sstream>
#include "ArgumentManager.h"

using namespace std;

struct Stack
{
    char data;
    Stack * next;
};

class stackExpression
{
    
private:
    Stack * stackTop;
    int count = 0;
    string * compareStringLine;
    int * results;
    int resultLine = 0;
    
public:
    
    stackExpression()
    {
        stackTop = nullptr;
    }
    
    bool checkChar(string * checkVarString)
    {
        string first = checkVarString[0];
        int k = 0;
    
        while(k < first.length()){
            if ((first[k] >= 'a' && first[k] <= 'z') || (first[k] >= 'A' && first[k] <= 'Z')){
                return true;
            }
        }
        return false;
    }
    
    void parsing(string inputFile, ofstream &output)
    {
        string line;
        string simpleLine;
        
        bool valid = true;
        
        initializeStack();
        
        ifstream inFile;
        inFile.open(inputFile);
        if (!inFile.is_open()){ cout << "Cannot open the file.\n"; }
        
        
        while(getline(inFile, line, '\n'))
        {
            if(!line.empty())
            {
                if(!isBalanced(line))
                {
                    output << "Error at: " << (count+1) << endl;   //For output file into server
                    cout << "Error at: " << (count+1) << endl;
                    valid = false;
                }
                count++;
            }
        }
        
        if(count < 2)
        {
            if(isBalanced(line)){
                //For server
                output << "Yes" << endl;
                cout << "Yes" << endl;
            }
        }

    
        
        else
        {
            compareStringLine = new string[count];
            results = new int[count];
            int arrayLine = 0;
            
            if(valid)
            {
                for (int i = 0; i < count; i++) {
                    results[i] = 0;
                }
                
                inFile.clear();
                inFile.seekg(0, ios::beg);
                
                while (getline(inFile, line, '\n'))
                {
                    if(!line.empty())
                    {
                        if(valid && (arrayLine < count))
                        {
                            simpleLine = simplify(line);
                            evalDigits(simpleLine);
                            
                            //Evaluate and put it in compareStringLine to compare
//                            compareStringLine[arrayLine] = evaluateInfix(simpleLine);
                            compareStringLine[arrayLine] = cleanUpString(simpleLine);
                            resultLine++;
                            arrayLine++;
                        }
                    }
                }

                if(compareDigits(results) == "Yes" && (checkChar(compareStringLine))){
                    //To print output into output file server
                    output << compareChars(compareStringLine) << endl;
                    cout << compareChars(compareStringLine) << endl;
                }
                else {
                    output << compareDigits(results) << endl;
                    cout << compareDigits(results) << endl;
                }
            }
        
        }
        
    }
    
    string compareDigits(int* nums) {
        
        int first = nums[0];
//        int i = 1;
        for (int i = 0; i < count-1; i++)
        {
            int compare = nums[i];
//            cout << compare << endl;
            if (first != compare) {
                return "No";
                break;
            }
//            i++;
        }
    
        return "Yes";
    }
    
    string compareChars(string* str) {
        
        bool same = false;
        bool exit = false;
        string first = str[0];
        //    cout << "First: " << first << endl;
        string compare;
        int k = 0;
        int j = 0;
        int i = 1;
        
        while (i < count) {
            compare = compareStringLine[i];
            while (k < compare.length()) {
                //checks to see if characters are equal, then only computes if they're alphabetical
                if (first[k] == compare[j] &&
                    ((first[k] >= 'a'&& first[k] <= 'z') || (first[k] >= 'A' && first[k] <= 'Z')) &&
                    ((compare[j] >= 'a'&& compare[j] <= 'z') || (compare[j] >= 'A' && compare[j] <= 'Z')))
                {
                    //comparing first and subtracting from another
                    if ((k == 0 && j == 0) || (k == 0 && (compare[j - 1] == '+')) || (j == 0 && (first[k - 1] == '+')) || (first[k - 1] == compare[j - 1]))  {
                        same = true;
                        k++;
                        j = 0;
                    }
                    
                    else if (first[k - 1] != compare[j - 1]) { //breaking the if if preceding signs are not equal
                        same = false;
                        exit = true;
                        break;
                    }
                }
                else {
                    
                    j++;
                    if (j == compare.length()) {
                        k++;
                        j = 0;
                    }
                }
            }
            
            if (k == compare.length()) {
                //cout << "Line completed, i: " << i << " and k: " << k << endl;
                k = 0;
                i++;
            }
            
            if (exit) {
                break;
            }
        }
        
        if (!same) {
            return "No";
        }
        else {
            return "Yes";
        }
    };
    
    string evaluateInfix (string str)
    {
        
        string temp = str;
        int sumDigit = 0;
        
        for (int i = 0; i < temp.length(); i++)
        {
            if (isdigit(temp[i]))
            {
                if(temp[i-1] != '+' || temp[i-1] != '-')
                    continue;
                
                if(temp[i-1] == '-')
                    sumDigit -= temp[i] - 48;
//                else if(temp[i] > 0)
                    
                else if(temp[i-1] == '+')
                    sumDigit += temp[i] - 48;
                
                temp = temp.substr(0,i-1) + temp.substr(i+1, temp.length());
                i--;
                
            }
        }
        
        if(sumDigit >= 0){
//            cout << temp + "+" + to_string(sumDigit) << endl;
            return temp + "+" + to_string(sumDigit);
        }
        else{
//            cout << temp + "+" + to_string(sumDigit) << endl;
            return temp + to_string(sumDigit);
        }
    }
    
    void evalDigits(string str) {
        
        int posNum1, posOp1, num1;
        string ns;
        
        for (int j = 0; j < str.length(); j++)
        {
            if (isdigit(str[j]) && j == 0) {
                posNum1 = j;
                num1 = (int(str[posNum1]) - 48);
                results[resultLine] = results[resultLine] + num1;
            }
            
            else if (isdigit(str[j])) {
                posNum1 = j;
                posOp1 = posNum1 - 1;
                
                num1 = (int(str[posNum1]) - 48);
                
                if (str[posOp1] == '+') {
                    results[resultLine] += num1;
                }
                else if (str[posOp1] == '-') {
                    results[resultLine] += num1;
                }
            }
        }
    
    };
    
    
    string simplify(string str)
    {
        int len = str.length();
        
        string newString;
        string res;
        
        newString.resize(str.length());
        res.resize(str.length());
        
        int index = 0, i = 0;
        
        // create empty stack
        push(0);
        
        while (i < str.length()) {
            if (str[i] == '+')
            {
                // If top is 1, flip the operator
                if (top() == 1)
                    res[index++] = '-';
                
                // If top is 0, append the same operator
                if (top() == 0)
                    res[index++] = '+';
            }
            
            else if (str[i] == '-')
            {
                if (top() == 1)
                    res[index++] = '+';
                
                else if (top() == 0)
                    res[index++] = '-';
            }
        
            
            else if ( (str[i] == '(' || str[i] == '[' || str[i] == '{') && i > 0)
            {
                if (str[i - 1] == '-')
                {
                    // x is opposite to the top of stack
                    int x = (top() == 1) ? 0 : 1;
                    push(x);
                }
                // push value equal to top of the stack
                else if (str[i - 1] == '+'){
                    push(top());
                }
                
            }
            // If closing and open parentheses pop the stack once
            else if ((str[i] == '(' || str[i] == '[' || str[i] == '{') || (str[i] == ')' || str[i] == ']' || str[i] == '}') )
            {
                pop();
            }
            
            // copy the character to the result
            else
                res[index++] = str[i];
            
            i++;
        }
        
        int pos = -1;
        
        for(int j = 0; j < str.length()-1; j++)
        {
            if( (res[j] == '-')  && (res[j+1] == '+'))
            {
                pos = j;
                break;
            }
            
        }
        
        if(pos != -1)
        {
            for(int j = pos; j < str.length()-1 ; j++)
            {
                res[j] = res[j+1];
            }
        }
        
        if(res[0] == '+')
        {
            for (int i = 1; i < str.length(); i++)
            {
                newString[i-1] = res[i];
                //cout << newString[i-1];
            }
            newString.resize(index);
            newString = cleanUpString(newString);
            return newString;
        }
        
        res.resize(index);
        res = cleanUpString(res);
        return res;
        
    }//End of simplify func
    
    string cleanUpString(string str)
    {
        string newString;
        string number = "";
        
        for (int k = 0; k < str.length(); k++) { //possibly +1
            if (k == 0 && !isdigit(str[k]) && (str[k] != '+' && str[k] != '-')) {
                newString += str[k];
            }
            
            else if (str[k] == '+' || str[k] == '-')
            {
                continue;
            }
            
            else if ((k != 0 && (str[k] >= 'a'&& str[k] <= 'z')) || (str[k] >= 'A' && str[k] <= 'Z')) {
                newString += str[k - 1];
                newString += str[k];
            }
            
            //Read until the number and add to the end of the string
            else if((str[k] >= '0' && str[k] <= 9) && k != 0){
                number += str[k-1];
                number += str[k];
            }
            
        }
        
        return newString;
    }
    
    //isBalanced borrowed from: https://www.geeksforgeeks.org/check-for-balanced-parentheses-in-an-expression/
    bool isBalanced(string expr)
    {
        char x;
        bool b = true;
        
        // Traversing the Expression
        for (int i=0; i<expr.length()+1; i++)
        {
            if (expr[i]=='('|| expr[i]=='[' ||expr[i]=='{')
            {
                // Push the element in the stack
                push(expr[i]);
            }
            
            switch (expr[i])
            {
                case ')':
                    // Store the top element in a
                    x = top();
                    pop();
                    if (x=='{' || x=='[')
                    {
                        b = false;
                    }
                    break;
                    
                case '}':
                    // Store the top element in b
                    x = top();
                    pop();
                    if (x=='(' || x=='[')
                    {
                        b = false;
                    }
                    break;
                    
                case ']':
                    
                    // Store the top element in c
                    x = top();
                    pop();
                    if (x =='(' || x == '{')
                    {
                        b = false;
                    }
                    break;
            }//End Switch
        }//End for loop
        
        while(!isEmpty())
        {
            b = false;
            pop();
        }
        
        if(b == false){
            return false;
            
        }
        else{
            return true;
        }
        
    }
    
    void initializeStack()
    {
        Stack * temp;
        
        while(stackTop != nullptr)
        {
            temp = stackTop;
            stackTop = stackTop->next;
            delete temp;
        }
    }
    
    
    char top()
    {
        if(stackTop != NULL)
            return stackTop->data;
        else
            return 0;
    }
    
    bool isFull()
    {
        return false;
    }
    
    bool isEmpty()
    {
        return(stackTop == nullptr);
    }
    
    void pop()
    {
        Stack * temp;
        if(stackTop!=nullptr)
        {
            temp = stackTop;
            stackTop = stackTop->next;
            delete temp;
        }

    }
    
    void push(char value)
    {
        Stack * temp = new Stack;
        temp->data = value;
        temp->next = stackTop;
        stackTop = temp;
        
    }

    
    
};

int main(int argc, char * argv[]) {
    
    ArgumentManager am(argc, argv);
    const string inputFileName = am.get("input");
    const string outputFileName = am.get("output");
    
//    string outputFileName = "output3.txt";
    stackExpression evaluator;
    ofstream output(outputFileName);
    
    evaluator.parsing("input4.txt", output);
//    evaluator.parsing(inputFileName, output);

    return 0;
}

//compareStringLine[numLine] = evaluateInfix(simpleLine);
//Input 4    //Only numbers
//Input 10   // - in front of the first varialbe
//Input 17   //compare right, but keep printing pop 5 times
//intput 18 // where is only one expression

