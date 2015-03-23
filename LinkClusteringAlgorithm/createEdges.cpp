#include<set>
#include <algorithm>    // std::random_shuffle
#include <ctime>        // std::time
#include <cstdlib>      // std::rand, std::srand
#define N 10
#define M 5
using namespace std;

set< pair<long, long> > pairs;
set< pair<long, long> > :: iterator it;

int main(){
        vector<long> nums;
        std::srand ( unsigned ( std::time(0) ) );
        long i=0;
        for(i=0; i<N; i++)
                nums.push_back(rand()%i);
        random_shuffle( nums.begin(), nums.end());
        long totaldone = 0;
        long num = 0;
        long leftInM = M;
        long totalNum;
        long i = 0;
        while(totaldone<N){
          if(num%M == M-1){
            totalNum = leftInM;
          }else if(leftInM){
            totalNum = rand()%leftInM;
          }else totalNum = 0;
          while(totalNum){
            if(nums[i] == -1 || nums[i] == num || 
                  pairs.count(make_pair(num, nums[i]))>0 || 
                  pairs.count(make_pair(nums[i], num))>0){
              i = (i+1)%N;
            }
            if(num<nums[i])
              pairs.count(make_pair(num, nums[i]));
            else
              pairs.count(make_pair(nums[i], num));
            nums[i] = -1;
            totalNum--;
            leftInM--;
          }
          num++;
          if(num%M == 0) leftInM = M;
        }
        for(it =pairs.begin(); it!=pairs.end(); it++)
          cout << (*it).first << " " << (*it).second << endl;
        return 0;
}
