#include<set>
#include <vector>    // std::random_shuffle
#include <iostream>    // std::random_shuffle
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#define N 1000000
#define M 1000
using namespace std;

set< pair<long, long> > pairs;
set< pair<long, long> > :: iterator it;

int main(){
        vector<long> nums;
        std::srand ( unsigned ( std::time(0) ) );
        long i=0;
        for(i=0; i<N; i++)
                nums.push_back(rand()%N);
        random_shuffle( nums.begin(), nums.end());
        long totaldone = 0;
        long num = 0;
        long leftInM = N/M;
        long totalNum;
        i = 0;
        while(pairs.size()<N){
          if(num%M == M-1){
            totalNum = leftInM;
          }else if(leftInM){
            totalNum = rand()%leftInM;
          }else totalNum = 0;
         // cout << "totalNum=" << totalNum << endl;
          while(totalNum){
            if(nums[i] == -1 || nums[i] == num ||
                  pairs.count(make_pair(num, nums[i]))>0 ||
                  pairs.count(make_pair(nums[i], num))>0){
                        i = (i+1)%N;
                        continue;
            }
            if(num<nums[i])
              pairs.insert(make_pair(num, nums[i]));
            else
              pairs.insert(make_pair(nums[i], num));
            //  cout << nums[i] << " " << num << " - " << pairs.size()  << endl;
            nums[i] = -1;
            totalNum--;
            leftInM--;
          }
          num = (num+1)%N;
          if(num%M == 0) leftInM = N/M;
        }
        for(it =pairs.begin(); it!=pairs.end(); it++)
          cout << (*it).first << " " << (*it).second << endl;
        return 0;
}
