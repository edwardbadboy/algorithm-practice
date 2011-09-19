#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

template<typename T> //Williams
void shift_down(T* a, int i, int n)
{
	for(int bc = 2*i; bc <= n; bc = 2*i){
		if(bc < n && a[bc] < a[bc+1]){
			bc = bc + 1;
		}
		if(a[i] < a[bc]){
			std::swap(a[i], a[bc]);
			i = bc;
		}else{
			break;
		}
	}
}

template<typename T> //Williams
void my_shift_down(T* a, int i, int n)
{
	T temp=a[i];//���Ȱ�����Ҫ�³���ֵ��������

	for(int bc = 2*i; bc <= n; bc = 2*i){
		if(bc < n && a[bc] < a[bc+1]){
			bc = bc + 1;
		}
		if(temp < a[bc])//��temp�����ĺ��ӱȽ�{
			a[i]=a[bc];//��������ֱ�������ĺ��Ӹ��ǵ�ǰ����
			i = bc;
		}else{
			break;
		}
	}

	a[i]=temp;//��temp�ƶ�����Ӧ��λ��

}

template<typename T>
void heap_sort(T* first, T* last)
{
	int const n = last - first;
	T* const a = first - 1;
	int i = n/2;
	for( ; i > 0; --i)
		my_shift_down(a,i,n);//shift_down(a, i, n);
	for(i = n; i > 1; )
	{
		swap(a[1], a[i]);
		my_shift_down(a,1,--i);//shift_down(a, 1, --i);
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

template<class T>
static void print(const T *first, const T *last){
	for(;first!=last;++first){
		printf("%d\n",*first);
	}
}

int main(int argc, char* argv[])
{
	const int count=30;
	int *rand_numbers=new int[count];

	for(int i=0;i<count;++i){
		rand_numbers[i]=i;
	}
	random_shuffle(rand_numbers, rand_numbers+count);
	print(rand_numbers, rand_numbers+count);

	heap_sort(rand_numbers,rand_numbers+count);

	cout<<(verify_sorted(rand_numbers,rand_numbers+count)?"sort ok":"sort failed")<<endl;
	print(rand_numbers, rand_numbers+count);

	delete rand_numbers;

	system("pause");
	return 0;
}

