#include "Queue.h"
#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
	Queue<int> q;
	Queue<int> tmp;
	int i = 0;

	for(i = 0; i < 9; i++)
	{
		q.push_queue(i+1,1);
	}

	//q.add_queue_length();

	// for(i = 0; i < 10; i++)
	// {
	// 	q.push_queue(i+17,1);
	// }
	// q.showdata();
	// //tmp.Copy_All_Node(&q);
	// q.delete_all_node();
	// //tmp.showdata();
	// 	q.showdata();

	// // q.delete_node(6);
	// // q.showdata();

	q.exchange(1);
	q.push_suitable_pos(65);
	q.exchange(5);
	q.push_suitable_pos(67);
	q.showdata();
	return 0;
}
