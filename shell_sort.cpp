#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <vector>
#include <iostream>
#include <string>

/*实现pdf里面13-21页中提到的所有增量序列的Shell排序.
自己设计一个增量序列,随机生成各种大小的数组,统计运行时间,从中比较出最好的增量序列.*/

using namespace std;


template<class T>
void shell_sort_one_time(T* first, long n, long gap){
	for(long i = gap; i < n; ++i){
		for(long j = i - gap; j >= 0; j -= gap){
			if(first[j+gap] < first[j]){
				std::swap(first[j], first[j+gap]);
			}else{
				break;
			}
		}
	}
}

static long const primes [] ={
	0, 1, 5, 13, 31,
	73, 199, 571, 1487, 3919,
	8861, 30013, 89899, 250051, 700079,
	1999891, 5000111, 13055113, 30000167, 89933143,
	240000151, 600000019, 1204945031, (1<<31)-1
};

template<class T>
void prime_shell_sort(T* first, T* last)
{
	long const n = last - first;
	long h = 2;
	for( ; primes[h] < n/3; ++h)
		;
	for(int gap = primes[--h]; 0 < gap; gap = primes[--h]){
		shell_sort_one_time(first,n,gap);
	}
}

template<class T>
void h3p1_shell_sort(T *first, T *last)
{
	long const n = last - first;
	long gap = 1;
	for(; (gap *= 3) < n; ++gap)
		;
	for(gap /= 3; 0 < gap; gap = (gap-1)/3){
		shell_sort_one_time(first,n,gap);
	}
}

template<class T>
void double_shell_sort(T *first, T *last)
{
	long const n = last - first;
	long gap = 1;
	for(; (gap *= 2) < n; ++gap)
		;
	for(gap /= 2; 0 < gap; gap = gap/2){
		shell_sort_one_time(first,n,gap);
	}
}

template<class T>
void square_shell_sort(T *first, T *last)
{
	long const n = last - first;
	long i=1;
	long gap;
	for(gap=i*i; gap < n; ++i){
		gap=i*i;
	}
	for(--i,gap=i*i; 0 < gap; --i,gap = i*i){
		shell_sort_one_time(first,n,gap);
	}
}

template<class T>
static void print_elements(const T *first, const T *last){
	for(const T *p=first; p!=last; ++p){
		printf("%d\n",(*p));
	}
}

template<class T>
static bool verify_sorted(const T *first, const T *last){
	if(first==last){
		return true;
	}
	const T *pre=first;
	const T *next=pre+1;
	for(; next!=last; pre=next,++next){
		if((*pre)>(*next)){
			return false;
		}
	}
	return true;
}

__inline __int64 tickCount()
{
    __asm
    {
        RDTSC
    }
}

template<class T, class SortFunc>
static long long test_sort_algorithm(const T *rand_source, long count, T *numbers, SortFunc sort_function,string function_description){
	cout<<"testing "<<function_description<<endl;
	cout<<"sorting "<<count<<" random numbers"<<endl;
	copy(rand_source, rand_source+count, numbers);
	long long start_t=tickCount();
	sort_function(numbers,numbers+count);
	long long end_t=tickCount();
	long long duration=(end_t-start_t)/1000000;
	cout<<duration<<"ms passed"<<endl;
	bool ok=verify_sorted(numbers,numbers+count);
	if(ok){
		cout<<"sort ok"<<endl;
		return duration;
	}else{
		cout<<"sort failed"<<endl;
		return -1;
	}
}

bool test_sort_algorithms(long number_count)
{
	int *rand_numbers=NULL;
	int *to_sort=NULL;

	cout<<"testing size "<<number_count<<endl;
	rand_numbers=(int*)malloc(number_count*sizeof(int));// new int[number_count];
	if(rand_numbers==NULL){
		cerr<<"cannot allocate for"<<number_count*sizeof(int)<<" bytes of memory!"<<endl;
		return false;
	}
	to_sort=(int*)malloc(number_count*sizeof(int));//new int[number_count];
	if(rand_numbers==NULL){
		cerr<<"cannot allocate for"<<number_count*sizeof(int)<<" bytes of memory!"<<endl;
		return false;
	}

	for(long i=0;i<number_count;++i){
		rand_numbers[i]=i;
	}
	random_shuffle(rand_numbers, rand_numbers+number_count);

	
	long long r=0;
	r=test_sort_algorithm(rand_numbers,number_count,to_sort,
		prime_shell_sort<int>,"shell sort with prime sequence");
	if(r==-1){
		free(rand_numbers);
		free(to_sort);
		return false;
	}

	r=test_sort_algorithm(rand_numbers,number_count,to_sort,
		h3p1_shell_sort<int>,"shell sort with H(i+1)=3*H(i)+1");
	if(r==-1){
		free(rand_numbers);
		free(to_sort);
		return false;
	}

	r=test_sort_algorithm(rand_numbers,number_count,to_sort,
		double_shell_sort<int>,"shell sort with H(i+1)=2*H(i)");
	if(r==-1){
		free(rand_numbers);
		free(to_sort);
		return false;
	}

	r=test_sort_algorithm(rand_numbers,number_count,to_sort,
		square_shell_sort<int>,"shell sort with 1 4 9 16 25 36...");
	if(r==-1){
		free(rand_numbers);
		free(to_sort);
		return false;
	}

	free(rand_numbers);
	free(to_sort);
	//delete [] rand_numbers;
	//delete [] to_sort;
	return true;
}

int main(int argc, char* argv[])
{

	long limit=0;
	long times=0;

	cerr<<"input size limit"<<endl;
	cin>>limit;
	if(limit<=0){
		cout<<"bad limit"<<endl;
		return 0;
	}

	cerr<<"input times"<<endl;
	cin>>times;
	if(times<=0){
		cout<<"bad times value"<<endl;
		return 0;
	}

	cerr<<"please close other applications before the test"<<endl;
	cerr<<"press any key to start"<<endl;
	system("pause");
	cout<<endl;

	time_t now=time(NULL);
	srand((unsigned int)now);
	for(long i=0;i<times;++i){
		int r=rand();
		long count=1+(long)(limit*(r/(RAND_MAX+1.0)));
		cerr<<"the "<<i<<"th time"<<endl;
		if(test_sort_algorithms(count)==false){
			cout<<endl;
			break;
		}
		cout<<endl;
	}

	cerr<<"press any key to close the application"<<endl;
	system("pause");
	return 0;
}

