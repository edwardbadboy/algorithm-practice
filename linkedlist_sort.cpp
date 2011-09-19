#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <queue>
/*适合单链表较快的排序是快速排序,归并排序和基数排序.
试给出三种排序算法的性能比较,统计它们的键值比较次数及总的运行时间.
对不同规模的问题进行试验,试求出当问题的规模超过多大时,基数排序算法的性能超过其余两个.
*/
using namespace std;

struct node{
	int key;
	node* next;
};

struct linked_list{
	node* head;
	node* tail;
};

__inline __int64 tickCount()
{
    __asm
    {
        RDTSC
    }
}

void push_tail(linked_list *l, node *n)
{
	if(l->head==NULL){
		l->head=n;
		l->tail=n;
		return;
	}
	l->tail->next=n;
	l->tail=n;
	return;
}

node *pop_head(linked_list *l)
{
	if(l->head==NULL){
		return NULL;
	}
	node *r=l->head;
	l->head = l->head->next;
	r->next=NULL;
	if(l->head==NULL){
		l->tail=NULL;
	}
	return r;
}

linked_list construct_random_list(long length,long key_limit)
{
	linked_list l;
	l.head=NULL;l.tail=NULL;

	long i=0;

	for(i=0; i<length; ++i){
		node *n=new node;
		n->next=NULL;
		n->key=key_limit*(rand()/(double)RAND_MAX);
		push_tail(&l,n);
	}
	return l;
}

void destroy_list(linked_list *l){
	for(node *n=pop_head(l); n!=NULL; n=pop_head(l)){
		delete n;
	}
	l->head=NULL;
	l->tail=NULL;
}

void print_list(const linked_list *l){
	int index=0;
	node *n=NULL;
	for(index=0, n=l->head; n!=NULL; n=n->next){
		cout<<index<<": "<<n->key;
		++index;
		if(index % 4==0){
			cout<<endl;
		}else{
			cout<<"\t";
		}
	}
	if(index % 4 != 0){
		cout<<endl;
	}
	return;
}

linked_list clone_list(const linked_list *l)
{
	linked_list c;
	c.head=NULL;c.tail=NULL;

	for(node *n=l->head; n!=NULL; n=n->next){
		node *new_node= new node;
		new_node->next=NULL;
		new_node->key=n->key;
		push_tail(&c,new_node);
	}

	return c;
}

void append_list(linked_list *l1,linked_list *l2)
{
	if(l2->head==NULL){
		return;
	}

	push_tail(l1,l2->head);
	l1->tail=l2->tail;
	return;
}

linked_list list_quick_sort(linked_list *l,long *compare_count){
	if(l->head==l->tail){
		return *l;
	}

	node *pivot=pop_head(l);

	linked_list less,greater;
	less.head=NULL;less.tail=NULL;
	greater.head=NULL;greater.tail=NULL;

	for(node *n=pop_head(l); n!=NULL; n=pop_head(l)){
		if(n->key <= pivot->key){
			push_tail(&less,n);
			++(*compare_count);
		}else if(n->key > pivot->key){
			push_tail(&greater,n);
			++(*compare_count);
		}
	}

	if(less.head!=less.tail){
		less=list_quick_sort(&less,compare_count);
	}
	push_tail(&less,pivot);
	if(greater.head!=greater.tail){
		greater=list_quick_sort(&greater,compare_count);
	}
	append_list(&less,&greater);
	return less;
}

linked_list list_merge(linked_list *l1,linked_list *l2,long *compare_count)
{
	if(l1->head==NULL){
		return (*l2);
	}

	linked_list l;l.head=NULL;l.tail=NULL;
	node *h1=pop_head(l1);
	node *h2=pop_head(l2);

	while(h1!=NULL && h2!=NULL){
		++(*compare_count);
		if(h1->key <= h2->key){
			push_tail(&l,h1);
			h1=pop_head(l1);
		}else{
			push_tail(&l,h2);
			h2=pop_head(l2);
		}
	}

	if(l1->head==NULL){
		++(*compare_count);
		append_list(&l,l2);
	}else if(l2->head==NULL){
		++(*compare_count);
		append_list(&l,l1);
	}

	return l;
}

linked_list list_merge_sort(linked_list *l,long *compare_count)
{
	if(l->head==l->tail){
		return (*l);
	}
	queue<linked_list> q;
	for(node *n=pop_head(l); n!=NULL; n=pop_head(l)){
		linked_list t={NULL,NULL};
		push_tail(&t,n);
		q.push(t);
	}

	while(q.size()>=2){
		linked_list l1=q.front();q.pop();
		linked_list l2=q.front();q.pop();
		q.push(list_merge(&l1,&l2,compare_count));
	}

	linked_list result=q.front();
	q.pop();
	return result;
}

int list_find_max_key(linked_list *l,long *compare_count){
	int max_key=l->head->key;
	for(node *n=l->head->next; n!=NULL; n=n->next){
		if(n->key > max_key){
			max_key=n->key;
		}
		++(*compare_count);
	}
	return max_key;
}

int compute_times_for_key(int max_key,int radix){
	int times=0;
	while(max_key>0){
		max_key/=radix;
		++times;
	}
	return times;
}

int compute_digit_for_key(int key,int place,int radix){
	while(place>0){
		key/=radix;
		--place;
	}
	return key % radix;
}

linked_list list_lsd_radix_sort(linked_list *l,long *compare_count)
{
	if(l->head==l->tail){
		return (*l);
	}
	const int radix=10;
	int max_key=list_find_max_key(l,compare_count);
	int times=compute_times_for_key(max_key,radix);

	vector<linked_list> boxes;
	linked_list empty_list={NULL,NULL};
	linked_list result=(*l);
	boxes.assign(radix,empty_list);

	for(int i=0;i<times;++i){
		for(node *n=pop_head(&result); n!=NULL; n=pop_head(&result)){
			int digit=compute_digit_for_key(n->key,i,radix);
			push_tail(&(boxes[digit]),n);
			++(*compare_count);
		}
		for(int j=0; j<radix; ++j){
			append_list(&result,&(boxes[i]));
			boxes[i]=empty_list;
			++(*compare_count);
		}
	}
	return result;
}

static bool verify_sorted(const linked_list *l){
	if(l->head==l->tail){
		return true;
	}
	const node *pre=l->head;
	const node *next=l->head->next;
	for(; next!=NULL; pre=next,next=next->next){
		if(pre->key > next->key ){
			return false;
		}
	}
	return true;
}

template<class SortFunc>
static long long test_sort_algorithm(const linked_list *rand_source, SortFunc sort_function, string function_description)
{
	cout<<"testing "<<function_description<<endl;

	linked_list l=clone_list(rand_source);

	long compare_count=0;

	long long start_t=tickCount();
	sort_function(&l,&compare_count);
	long long end_t=tickCount();

	long long duration=(end_t-start_t)/1000000;
	cout<<duration<<"ms passed, compared for "<<compare_count<<" times"<<endl;

	bool ok=verify_sorted(&l);

	destroy_list(&l);

	if(ok){
		cout<<"sort ok"<<endl;
		return duration;
	}else{
		cout<<"sort failed"<<endl;
		return -1;
	}
}

int main(int argc, char* argv[])
{
	srand((unsigned int)time(NULL));

	linked_list l=construct_random_list(1000000,100000);
	//print_list(&l);
	cout<<endl;

	test_sort_algorithm(&l,list_quick_sort,"quick sort for linked list");
	test_sort_algorithm(&l,list_merge_sort,"merge sort for linked list");
	test_sort_algorithm(&l,list_lsd_radix_sort,"LSD radix sort for linked list");

	destroy_list(&l);

	system("pause");
	return 0;
}

