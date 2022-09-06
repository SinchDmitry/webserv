#include <iostream>
#include <string>
#include <sstream>
#include <vector>

std::vector<std::string> split(const std::string &s, char delim) {
    std::stringstream ss(s);
    std::string item;
	std::vector<std::string> elems;
    while(std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

//int main() {
//	std::string a = "123,432,71283,1238";
//    std::vector<std::string> arr = split(a, ',');
//
//    for(const std::string& it : arr) {
//        std::cout << it << " ";
//	}
//	std::cout << std::endl;
//}


// #include <iostream>
// #include <string>
// #include <sstream>
// #include <vector>
// #include <algorithm>
// using namespace std;
 
// typedef int ValueType;
// typedef std::vector<ValueType> ValueTypeVector;
 
// ValueTypeVector& split(const std::string &s, char delim, ValueTypeVector &elems)
// {
//     std::stringstream ss(s);
//     std::string item;
//     while(std::getline(ss, item, delim))
//     {
//         elems.push_back(std::stoi(item));
//     }
//     return elems;
// }
 
 
// ValueTypeVector split(const std::string &str, char delim)
// {
//     ValueTypeVector elems;
//     split(str, delim, elems);
//     return elems;
// }
 
// int main()
// {
//     std::string a = "123,432,71283,1238";
//     ValueTypeVector arr = split(a, ',');
    
//     for(const ValueType& it : arr)
//         cout << it << " ";
// }